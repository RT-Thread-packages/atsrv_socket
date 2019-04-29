#include <at.h>
#define AT_VERSION "0.0.3"

static at_result_t at_version_exec(void)
{
    at_server_printfln(AT_VERSION);
    at_server_printfln("%s %s", __DATE__, __TIME__);
    return AT_RESULT_OK;
}

AT_CMD_EXPORT("AT+GMR", NULL, NULL, NULL, NULL, at_version_exec);
