#include <at.h>

#define AT_SERVER_IP_GETIP          2       // Connected ap and got ip address
#define AT_SERVER_IP_READY          3       // TCP/UDP transport established 
#define AT_SERVER_IP_DISCONNECTED   4       // Disconnect the network 
#define AT_SERVER_IP_NOT_CONNECTED  5       // Ap not connected 

static at_result_t at_ipstatus_query(void)
{
    at_server_printfln("AT+CIPSTATUS");
    return AT_RESULT_OK;
}

static at_result_t at_ipstatus_exec(void)
{
    int status = AT_SERVER_IP_NOT_CONNECTED;

    if(rt_wlan_is_ready())
    {
        status = AT_SERVER_IP_GETIP;
    }

    at_server_printfln("+STATUS:%d", status);
    return AT_RESULT_OK;
}

AT_CMD_EXPORT("AT+CIPSTATUS", NULL, NULL, at_ipstatus_query, NULL, at_ipstatus_exec);
