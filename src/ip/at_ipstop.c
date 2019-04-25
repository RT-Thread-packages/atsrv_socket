#include <at.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>

extern int ipmode;
extern int socket_fd_table[5];
extern int single_connect_fd;

static at_result_t at_ipstop_exec(void)
{
    if(ipmode)
    {
        return AT_RESULT_FAILE;
    }

    if(single_connect_fd != -1)
    {
        closesocket(single_connect_fd);
        single_connect_fd = -1;
    }
    return AT_RESULT_OK;
}

static at_result_t at_ipstop_setup(const char * args)
{
    int linkid = -1;
    if(!ipmode)
    {
        return AT_RESULT_FAILE;
    }
    
    if(at_req_parse_args(args, "=%d", &linkid) > 0)
    {
        if(linkid < 0 || linkid > 4)
        {
            return AT_RESULT_FAILE;
        }

        if(socket_fd_table[linkid] != -1)
        {
            closesocket(socket_fd_table[linkid]);
            socket_fd_table[linkid] = -1;
        }
    }
    else
    {
        return AT_RESULT_PARSE_FAILE;
    }
    return AT_RESULT_OK;
    
}

AT_CMD_EXPORT("AT+CIPCLOSE", "=<link id>", NULL, NULL, at_ipstop_setup, at_ipstop_exec);
