#include <at.h>

int ipmode = 0;

static at_result_t at_ipmode_query(void)
{
    at_server_printfln("+CIPMUX:%d", ipmode);
    return AT_RESULT_OK;
}

extern int msh_exec(char *cmd, rt_size_t length);
static at_result_t at_ipmode_setup(const char *args)
{
    if(at_req_parse_args(args, "=%d", &ipmode) > 0)
    {
        
    }
    else
    {
        return AT_RESULT_PARSE_FAILE;
    }
    return AT_RESULT_OK;
}

AT_CMD_EXPORT("AT+CIPMUX", "=<mode>", NULL, at_ipmode_query, at_ipmode_setup, NULL);
