#include <rtdevice.h>
#include <at.h>

static int enc_parse(struct rt_wlan_info * info)
{
    switch(info->security)
    {
    case SECURITY_OPEN:
        return 0;
    case SECURITY_WEP_PSK:
    case SECURITY_WEP_SHARED:
        return 1;
    case SECURITY_WPA_TKIP_PSK:
    case SECURITY_WPA_AES_PSK:
        return 2;
    case SECURITY_WPA2_AES_PSK:
    case SECURITY_WPA2_TKIP_PSK:
        return 3;
    case SECURITY_WPA2_MIXED_PSK:
        return 4;
    }
}

static int pairwise_cipher_parse(struct rt_wlan_info * info)
{
    if(info->security == SECURITY_OPEN)
    {
        return 0;
    }
    else if(info->security & WEP_ENABLED)
    {
        if(info->security & SHARED_ENABLED)
        {
            return 2;
        }
        return 1;
    }
    else if((info->security & TKIP_ENABLED) && (info->security & AES_ENABLED))
    {
        return 5;
    }
    else if(info->security & TKIP_ENABLED)
    {
        return 3;
    }
    else if(info->security & AES_ENABLED)
    {
        return 4;
    }
    return 6;
}

static int wps_parse(struct rt_wlan_info * info)
{
    if(info->security & WPS_ENABLED)
    {
        return 1;
    }
    return 0;
}

static void print_mac(struct rt_wlan_info * info)
{
    at_server_printf("%02X:%02X:%02X:%02X:%02X:%02X",  info->bssid[0],
                                                        info->bssid[1],
                                                        info->bssid[2],
                                                        info->bssid[3],
                                                        info->bssid[4],
                                                        info->bssid[5]);
}

static at_result_t at_scan_exec(void)
{
    int num = 0,i;
    struct rt_wlan_info *info;
    struct rt_wlan_scan_result * result = rt_wlan_scan_sync();
    num = result->num;
    info = result->info;
    for(i = 0; i < num; i++)
    {
        at_server_printf("+CWLAP:");
        at_server_printf("%d,",enc_parse(&info[i]));
        info[i].ssid.val[info[i].ssid.len] = '\0';
        at_server_printf("\"%s\",",info[i].ssid.val);
        at_server_printf("%d,",info[i].rssi);
        print_mac(&info[i]);at_server_printf(",");
        at_server_printf("%d,",info[i].channel);
        at_server_printf("0,0,");//<freq offset>, <freq cali>

        at_server_printf("%d,",pairwise_cipher_parse(&info[i]));
        at_server_printf("%d,",pairwise_cipher_parse(&info[i]));
        at_server_printf("7,");//bgn
        at_server_printfln("%d",wps_parse(&info[i]));
    }

    return AT_RESULT_OK;
}

static at_result_t at_scan_setup(const char *args)
{
    //TODO:
    return at_scan_exec();
}

AT_CMD_EXPORT("AT+CWLAP", "=<ssid>[,<mac>,<channel>,<scan_type>,<scan_time_min>,<scan_time_max>]", NULL, NULL, at_scan_setup, at_scan_exec);