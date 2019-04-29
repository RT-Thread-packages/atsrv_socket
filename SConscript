from building import *

cwd  = GetCurrentDir()
src  = Glob('*.c')
path = [cwd + '/inc']

if GetDepend(['ATSRV_SOCKET_ENABLE_CMD_SYSRAM']):
    src += Glob('src/base/at_mem.c')
	
if GetDepend(['ATSRV_SOCKET_ENABLE_CMD_GMR']):
    src += Glob('src/base/at_version.c')	

if GetDepend(['ATSRV_SOCKET_ENABLE_CMD_CIPDOMAIN']):
    src += Glob('src/ip/at_ipdns.c')

if GetDepend(['ATSRV_SOCKET_ENABLE_CMD_CIPMUX']):
    src += Glob('src/ip/at_ipmode.c')

if GetDepend(['ATSRV_SOCKET_ENABLE_CMD_PING']):
    src += Glob('src/ip/at_ipping.c')

if GetDepend(['ATSRV_SOCKET_ENABLE_CMD_CIPSEND']):
    src += Glob('src/ip/at_ipsend.c')

if GetDepend(['ATSRV_SOCKET_ENABLE_CMD_CIPSTART']):
    src += Glob('src/ip/at_ipstart.c')

if GetDepend(['ATSRV_SOCKET_ENABLE_CMD_CIPSTATUS']):
    src += Glob('src/ip/at_ipstatus.c')

if GetDepend(['ATSRV_SOCKET_ENABLE_CMD_CIPCLOSE']):
    src += Glob('src/ip/at_ipstop.c')

if GetDepend(['ATSRV_SOCKET_ENABLE_CMD_OTA']):
    src += Glob('src/ota/at_ota.c')

if GetDepend(['ATSRV_SOCKET_ENABLE_CMD_CWJAP']):
    src += Glob('src/wifi/at_connect.c')

if GetDepend(['ATSRV_SOCKET_ENABLE_CMD_CWQAP']):
    src += Glob('src/wifi/at_disconnect.c')

if GetDepend(['ATSRV_SOCKET_ENABLE_CMD_CWMODE']):
    src += Glob('src/wifi/at_mode.c')

if GetDepend(['ATSRV_SOCKET_ENABLE_CMD_CWLAP']):
    src += Glob('src/wifi/at_scan.c')
	
group = DefineGroup('atsrv_socket', src, depend = ['PKG_USING_ATSRV_SOCKET'], CPPPATH = path)

Return('group')