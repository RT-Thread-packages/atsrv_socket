#include <at.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <rtthread.h>

#define DBG_ENABLE
#define DBG_SECTION_NAME  "[AT/IP]"
#define DBG_LEVEL         DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>

extern at_server_t at_get_server(void);

extern int ipmode;
int socket_fd_table[5] = {-1, -1, -1, -1, -1};
rt_mutex_t out_mutex;

static at_result_t at_ipstart_query(void)
{
    at_server_printfln("AT+CIPSTART=<type>,<remoteIP>,<remote port>[,<TCP keepalive>]");
    return AT_RESULT_OK;
}

static at_result_t at_ipstart_exec(void)
{
    at_server_printfln("AT+CIPSTART=<type>,<remoteIP>,<remote port>[,<TCP keepalive>]");
    return AT_RESULT_OK;
}

int single_connect_fd = -1;

static void receiver_thread_entry(void * args)
{
    static rt_uint8_t recv_data[4096];
    int bytes_received = 0;
    at_server_t at_server = at_get_server();
    int socket_fd = ((int)args) == -1 ? single_connect_fd : socket_fd_table[((int)args)];
	
	out_mutex = rt_mutex_create("atserver", RT_IPC_FLAG_FIFO);
	
    while (1)
    {
        bytes_received = recv(socket_fd, recv_data, sizeof(recv_data) - 1, 0);
        if (bytes_received < 0)
        {
            closesocket(socket_fd);
            if(((int)args) == -1)
            {
                single_connect_fd = -1;
            }
            else
            {
                socket_fd_table[((int)args)] = -1;
            }
            LOG_E("\nreceived error,close the socket.\r\n");
            break;
        }
        else if (bytes_received == 0)
        {
            LOG_I("\nReceived warning,recv function return 0.\r\n");
            continue;
        }

        recv_data[bytes_received] = '\0';
        rt_mutex_take(out_mutex, RT_WAITING_FOREVER);
        at_server_printf("+IPD,%d,%d:", ((int)args), bytes_received, recv_data);
        rt_device_write(at_server->device, 0, recv_data, bytes_received);
        rt_mutex_release(out_mutex);
    }
}

static void start_receiver_thread(int linkid)
{
    rt_thread_t tid;
    tid = rt_thread_create("ssrt", receiver_thread_entry, (void *)linkid, 1024, 18, 20);
    if(tid)
    {
        rt_thread_startup(tid);
        return;
    }
    LOG_E("Receive thread startup failed");
}

static int at_fd_connect(struct sockaddr_in local_sockaddr, struct sockaddr_in remote_sockaddr, int istcp)
{
    int socket_fd = -1;
    socket_fd = socket(local_sockaddr.sin_family, istcp ? SOCK_STREAM : SOCK_DGRAM, istcp ? IPPROTO_TCP : IPPROTO_UDP);
    if(socket_fd == -1)
    {
        goto __error;
    }
    if((bind(socket_fd, (struct sockaddr *)&local_sockaddr, sizeof(local_sockaddr)) != 0))
    {
        LOG_E("Socket bind error");
        goto __error;
    }    
    else
    {
        if((connect(socket_fd, (struct sockaddr *)&remote_sockaddr, sizeof(remote_sockaddr)) != 0))
        {
            LOG_E("Connect error");
            goto __error;
        }
    }
    return socket_fd;

__error:
    if(socket_fd != -1)
    {
        closesocket(socket_fd);
        socket_fd = -1;
    }
    return -1;
}

static at_result_t at_ipstart_setup(const char * args)
{
    int result = 5;
    static char connection_type[10];
    static char remote_addr[128];
    int port = 0;
    int alive = 0;
    int udp_local_port = 0;
    int udp_mode = 0;
    struct hostent* host_info = NULL;
    struct sockaddr_in local_sockaddr;
    struct sockaddr_in remote_sockaddr;
    int is_tcp = 0;
    int linkid = 0;

    memset(&local_sockaddr,0,sizeof(local_sockaddr));
    memset(&remote_sockaddr,0,sizeof(remote_sockaddr));

    if(ipmode)
    {
        if(at_req_parse_args(args, "=%d,%*[\"]%[^\"]%*[\"],%*s", &linkid, connection_type) > 0)
        {
            if(linkid < 0 || linkid > 4)
            {
                return AT_RESULT_FAILE;
            }
            if(socket_fd_table[linkid] != -1)
            {
                at_server_printfln("ALREADY CONNECTED");
                return AT_RESULT_NULL;
            }
            if(strcmp("TCP", connection_type) == 0)
            {
                is_tcp = 1;
                if((result = at_req_parse_args(args, "=%d,%*[\"]%[^\"]%*[\"],%*[\"]%[^\"]%*[\"],%d,%d", &linkid, connection_type, remote_addr, &port, &alive)) <= 0)
                {
                    return AT_RESULT_PARSE_FAILE;
                }
            }
            else
            {
                if((result = at_req_parse_args(args, "=%d,%*[\"]%[^\"]%*[\"],%*[\"]%[^\"]%*[\"],%d,%d,%d", &linkid, connection_type, remote_addr, &port, &udp_local_port, &udp_mode)) <= 0)
                {
                    return AT_RESULT_PARSE_FAILE;
                }
            }
            host_info = gethostbyname(remote_addr);
            if(host_info == NULL || host_info->h_addr_list[0] == NULL)
            {
                return AT_RESULT_FAILE;
            }
            local_sockaddr.sin_family = host_info->h_addrtype;
            local_sockaddr.sin_len = host_info->h_length;

            remote_sockaddr.sin_port = htons(port);
            remote_sockaddr.sin_len = host_info->h_length;
            remote_sockaddr.sin_family = host_info->h_addrtype;
            remote_sockaddr.sin_addr = *((struct in_addr *)(host_info->h_addr_list[0]));//IPV6 TO DO:
            socket_fd_table[linkid] = at_fd_connect(local_sockaddr, remote_sockaddr, is_tcp);
            if(socket_fd_table[linkid] != -1)
            {
                at_server_printfln("%d,CONNECT",linkid);
                start_receiver_thread(linkid);
            }
        }
        else
        {
            return AT_RESULT_PARSE_FAILE;
        }
    }
    else
    {
        if(at_req_parse_args(args, "=%*[\"]%[^\"]%*[\"],%*s", connection_type) > 0)
        {
            if(single_connect_fd != -1)
            {
                at_server_printfln("ALREADY CONNECTED");
                return AT_RESULT_NULL;
            }
            if(strcmp("TCP", connection_type) == 0)
            {
                is_tcp = 1;
                if((result = at_req_parse_args(args, "=%*[\"]%[^\"]%*[\"],%*[\"]%[^\"]%*[\"],%d,%d", connection_type, remote_addr, &port, &alive)) <= 0)
                {
                    return AT_RESULT_PARSE_FAILE;
                }
            }
            else
            {
                if((result = at_req_parse_args(args, "=%*[\"]%[^\"]%*[\"],%*[\"]%[^\"]%*[\"],%d,%d,%d", connection_type, remote_addr, &port, &udp_local_port, &udp_mode)) <= 0)
                {
                    return AT_RESULT_PARSE_FAILE;
                }
            }
            host_info = gethostbyname(remote_addr);
            if(host_info == NULL || host_info->h_addr_list[0] == NULL)
            {
                return AT_RESULT_FAILE;
            }
            local_sockaddr.sin_family = host_info->h_addrtype;
            local_sockaddr.sin_len = host_info->h_length;

            remote_sockaddr.sin_port = htons(port);
            remote_sockaddr.sin_len = host_info->h_length;
            remote_sockaddr.sin_family = host_info->h_addrtype;
            remote_sockaddr.sin_addr = *((struct in_addr *)(host_info->h_addr_list[0]));//IPV6 TO DO:
            single_connect_fd = at_fd_connect(local_sockaddr, remote_sockaddr, is_tcp);
            if(single_connect_fd != -1)
            {
                at_server_printfln("CONNECT");
                start_receiver_thread(-1);
            }
        }
        else
        {
            return AT_RESULT_PARSE_FAILE;
        }
    }
    return AT_RESULT_OK;
}

AT_CMD_EXPORT("AT+CIPSTART", "=[<link id>,]<type>,<remoteIP>,<remote port>[,<TCP keepalive>][,<UDP local port>[,<UDP mode>]]", NULL, at_ipstart_query, at_ipstart_setup, at_ipstart_exec);
