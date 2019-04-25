#include <at.h>

static at_result_t at_ipstatus_query(void)
{
    at_server_printfln("AT+CIPSTATUS");
    return AT_RESULT_OK;
}

static at_result_t at_ipstatus_exec(void)
{
    int status = 5;
    if(rt_wlan_is_ready())
    {
        status = 2;
    }
    else if(rt_wlan_is_connected())
    {
        status = 4;
    }
    at_server_printfln("+STATUS:%d", status);
    return AT_RESULT_OK;
}

AT_CMD_EXPORT("AT+CIPSTATUS", NULL, NULL, at_ipstatus_query, NULL, at_ipstatus_exec);
