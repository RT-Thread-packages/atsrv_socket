#include <at.h>

extern int msh_exec(char *cmd, rt_size_t length);
static at_result_t at_ota_setup(const char *args)
{
    char url[256];
    char cmd[256+sizeof("http_ota ")];
    int len = 0;
    if(at_req_parse_args(args, "=%*[\"]%[^\"]%*[\"]", url) > 0)
    {
        len = sprintf(cmd, "http_ota %s", url);
        msh_exec(cmd, len);
    }
    else
    {
        return AT_RESULT_PARSE_FAILE;
    }
    return AT_RESULT_OK;
}

AT_CMD_EXPORT("AT+OTA", "=<url>", NULL, NULL, at_ota_setup, NULL);

//AT+OTA="http://172.16.88.86/image2_all_ota1.rbl"