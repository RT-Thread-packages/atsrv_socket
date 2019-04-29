#include <at.h>
#include <rtdevice.h>

static at_result_t at_disconnect_query(void)
{
    at_server_printfln("AT+CWQAP");
    return AT_RESULT_OK;
}

static at_result_t at_disconnect_exec(void)
{
    rt_wlan_disconnect();
    return AT_RESULT_OK;
}

AT_CMD_EXPORT("AT+CWQAP", NULL, NULL, at_disconnect_query, NULL, at_disconnect_exec);
