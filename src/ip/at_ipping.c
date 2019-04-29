#include <at.h>
#include <netdb.h>
#include <sys/socket.h>

extern rt_err_t at_ping(char* target_name, rt_size_t size);
static at_result_t at_ipping_setup(const char *args)
{
    char domain[128];
    struct hostent* host_info;
    int i = 0;
    if(at_req_parse_args(args, "=%*[\"]%[^\"]%*[\"]", domain) > 0)
    {
	    at_server_printfln("===AT+PING TODO == \r\n L:%d, F:%s, IN %s", __LINE__, __FUNCTION__, __FILE__);
//        if(at_ping(domain, 0) != RT_EOK)
//        {
//            return AT_RESULT_FAILE;
//        }
    }
    else
    {
        return AT_RESULT_PARSE_FAILE;
    }
    return AT_RESULT_OK;
}

AT_CMD_EXPORT("AT+PING", "=<domain name>", NULL, NULL, at_ipping_setup, NULL);
