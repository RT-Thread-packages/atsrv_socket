#include <at.h>

static at_result_t at_mode_query(void)
{
	  at_server_printfln("===AT+CWMODE? TODO == \r\n L:%d, F:%s, IN %s", __LINE__, __FUNCTION__, __FILE__); 
    at_server_printfln("+CWMODE:1");
    return AT_RESULT_OK;
}

static at_result_t at_mode_setup(const char *args)
{
    int mode = 0;
    if(at_req_parse_args(args, "=%d", &mode) > 0)
    {
        //TO DO:
			  at_server_printfln("===AT+CWMODE= TODO == \r\n L:%d, F:%s, IN %s", __LINE__, __FUNCTION__, __FILE__);
        return AT_RESULT_OK;
    }
    else
    {
        return AT_RESULT_PARSE_FAILE;
    }
    return AT_RESULT_OK;
}

AT_CMD_EXPORT("AT+CWMODE", "=<mode>", NULL, at_mode_query, at_mode_setup, NULL);
