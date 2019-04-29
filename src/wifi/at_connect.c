#include <at.h>
#include <rtdevice.h>

#define SSID_NAME_LENGTH_MAX 32
#define PASSWORD_LENGTH_MAX 32

static at_result_t at_connect_setup(const char *args)
{
    char ssid[SSID_NAME_LENGTH_MAX], pass[PASSWORD_LENGTH_MAX];
    if(at_req_parse_args(args, "=%*[\"]%[^\"]%*[\"],%*[\"]%[^\"]%*[\"]", ssid, pass) > 0)
    {
        if(rt_wlan_connect(ssid, pass) == RT_EOK)
        {
            return AT_RESULT_OK;
        }
        else
        {
            return AT_RESULT_FAILE;
        }
    }
    else
    {
        return AT_RESULT_PARSE_FAILE;
    }
}

AT_CMD_EXPORT("AT+CWJAP", "=<ssid>,<pass>", NULL, NULL, at_connect_setup, NULL);
