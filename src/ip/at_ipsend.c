#include <at.h>
#include <netdb.h>
#include <sys/socket.h>

extern int ipmode;
extern int single_connect_fd;
extern int socket_fd_table[5];
extern at_server_t at_get_server(void);

static rt_err_t get_char_timeout(rt_tick_t timeout, char * chr)
{
    char ch;
    rt_err_t result;
    at_server_t at_server = at_get_server();
    while (rt_device_read(at_server->device, 0, &ch, 1) == 0)
    {
        rt_sem_control(at_server->rx_notice, RT_IPC_CMD_RESET, RT_NULL);
        if((result = rt_sem_take(at_server->rx_notice, timeout)) != RT_EOK)
        {
            return result;
        }
    }
    if(at_server->echo_mode)
    {
        at_server_printf("%c", ch);
    }
    *chr = ch;
    return RT_EOK;
}

static at_result_t at_ipsend_exec(void)
{
    uint8_t * sendbuffer = NULL;
    uint8_t * psendbuffer;
    rt_err_t result = RT_EOK;
    int length = 0;
    if(ipmode)
    {
        return AT_RESULT_FAILE;
    }

    if(single_connect_fd == -1)
    {
        return AT_RESULT_FAILE;
    }

    sendbuffer = malloc(2048);
    at_server_printf("\r\n>");
    while(1)
    {
        psendbuffer = sendbuffer;
        length = 2048;
        while(length)
        {
            result = get_char_timeout(rt_tick_from_millisecond(20), psendbuffer);
            if(result != RT_EOK)
            {
                break;
            }
            length--;
            psendbuffer++;
        }
        if(rt_strncmp("+++", sendbuffer, 3) == 0)
        {
            free(sendbuffer);
            sendbuffer = NULL;
            break;
        }
        if((psendbuffer - sendbuffer) > 0)
        {
            send(single_connect_fd, sendbuffer, psendbuffer - sendbuffer, 0);
        }
    }
    return AT_RESULT_NULL;
}



static at_result_t at_ipsend_setup(const char *args)
{
    int length = 0;
    int send_bytes = 0;
    uint8_t * sendbuffer = NULL;
    uint8_t * psendbuffer;
    int linkid = -1;
    int socket_fd = -1;
    if(ipmode)
    {
        if(at_req_parse_args(args, "=%d,%d", &linkid, &length) <= 0)
        {
            return AT_RESULT_PARSE_FAILE;
        }
        socket_fd = socket_fd_table[linkid];
    }
    else
    {
        if(at_req_parse_args(args, "=%d", &length) <= 0)
        {
            return AT_RESULT_PARSE_FAILE;
        }
        socket_fd = single_connect_fd;
    }

    
    send_bytes = length;

    if(length > 0)
    {
        if(socket_fd == -1)
        {
            return AT_RESULT_FAILE;
        }
        
        sendbuffer = malloc(length);
        if(sendbuffer == NULL)
        {
            at_server_printfln("SEND FAIL");
            return AT_RESULT_NULL;
        }
        psendbuffer = sendbuffer;
        at_server_printfln("\r\nOK");
        at_server_printf(">");
        while(length)
        {
            if(get_char_timeout(rt_tick_from_millisecond(1000), psendbuffer) != RT_EOK)
            {
                at_server_printfln("SEND FAIL");
                free(sendbuffer);
                return AT_RESULT_NULL;
            }
            length--;
            psendbuffer++;
        }
        at_server_printfln("\r\nRecv %d bytes", send_bytes);
        if(send(socket_fd, sendbuffer, psendbuffer - sendbuffer, 0) == psendbuffer - sendbuffer)
        {
            free(sendbuffer);
            sendbuffer = NULL;
            
            at_server_printfln("SEND OK");
            return AT_RESULT_NULL;
        }
        else
        {
            free(sendbuffer);
            sendbuffer = NULL;
            at_server_printfln("SEND FAIL");
            return AT_RESULT_NULL;
        }
    }
    return AT_RESULT_OK;
}

AT_CMD_EXPORT("AT+CIPSEND", "=[<linkid>,]<length>", NULL, NULL, at_ipsend_setup, at_ipsend_exec);
