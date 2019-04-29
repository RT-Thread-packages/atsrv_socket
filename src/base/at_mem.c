#include <at.h>

static at_result_t at_sysram_exec(void)
{
    rt_uint32_t total,used;

    rt_memory_info(&total, &used, RT_NULL);
    at_server_printfln("+SYSRAM:%d", total - used);
    return AT_RESULT_OK;
}

AT_CMD_EXPORT("AT+SYSRAM", NULL, NULL, NULL, NULL, at_sysram_exec);
