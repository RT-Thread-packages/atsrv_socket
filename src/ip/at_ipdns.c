#include <at.h>
#include <netdb.h>
#include <sys/socket.h>

static at_result_t at_ipdns_setup(const char *args)
{
    char domain[128];
    struct hostent* host_info;
    int i = 0;
    if(at_req_parse_args(args, "=%*[\"]%[^\"]%*[\"]", domain) > 0)
    {
        host_info = gethostbyname(domain);
        if(host_info == NULL)
        {
            at_server_printfln("DNS Fail");
            return AT_RESULT_FAILE;
        }
        while(host_info->h_addr_list[i] != NULL)
        {
            at_server_printfln("+CIPDOMAIN:%s", inet_ntoa(*(struct in_addr*)host_info->h_addr_list[i]));
            i++;
        }
        
    }
    else
    {
        return AT_RESULT_PARSE_FAILE;
    }
    return AT_RESULT_OK;
}

AT_CMD_EXPORT("AT+CIPDOMAIN", "=<domain name>", NULL, NULL, at_ipdns_setup, NULL);
