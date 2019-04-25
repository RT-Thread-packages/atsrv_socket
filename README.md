# atsrv_socket

## 1、介绍

atsrv_socket 是包含了常用 socket 指令的 AT 服务端，能够响应客户端发送的命令，并执行相关操作。

支持以下几类 AT 指令：

**基础 AT 指令（base）**

|     指令     | 描述                 |
|--------------|----------------------|
| AT           | 测试 AT 启动         |
| ATZ          | 恢复出厂设置         |
| AT+RST       | 重启模块             |
| ATE          | 开关回显功能         |
| AT&L         | 获取指令列表         |
| AT+UART      | 串口配置             |
| AT+GMR       | 查看版本信息         |
| AT+SYSRAM    | 剩余内存查询         |

**OTA 指令**

|     指令     | 描述                 |
|--------------|----------------------|
| AT+OTA       | 执行 OTA 升级        |

**WLAN 指令**

|     指令     | 描述                 |
|--------------|----------------------|
| AT+CWMODE    | 模式选择             |
| AT+CWLAP     | 扫描 AP              |
| AT+CWJAP     | 连接 AP              |
| AT+CWQAP     | 断开 AP              |

**TCP/IP 指令**

|     指令     | 描述                 |
|--------------|----------------------|
| AT+CIPSTATUS | 状态查询             |
| AT+CIPMUX    | 连接模式设置         |
| AT+PING      | PING                 |
| AT+CIPDOMAIN | DNS 解析             |
| AT+CIPSTART  | 建立 TCP/UDP 连接    |
| AT+CIPCLOSE  | 断开连接             |
| AT+CIPSEND   | 发送数据             |

### 1.1 目录结构

| 名称  | 说明 |
| ---- | ---- |
| base | 基础指令源代码目录 |
| ip   | TCP/IP 指令源代码目录 |
| ota  | OTA 指令源代码目录 |
| wifi | Wlan 指令源代码目录 |

### 1.2 许可证

atsrv_socket 软件包遵循 Apache-2.0 许可，详见 LICENSE 文件。

### 1.3 依赖

- RT-Thread 3.0+
- AT commands 组件
- SAL 组件

## 2、获取软件包

使用 atsrv_socket 软件包需要在 RT-Thread 的包管理中选中它，具体路径如下：

```
RT-Thread online packages  --->
    IoT - internet of things  --->
         [*] atsrv_socket: AT server with socket commands.
         (uart3) Server device name
         [ ]   Enable base AT command
         [ ]   Enable OTA AT command
         [ ]   Enable WLAN AT command
         [ ]   Enable TCP/IP AT command
               Version (latest)  --->
```

**Server device name**：AT 串口设备名称，注意不能与 shell 串口相同。

**Enable base AT command**：使能基础类 AT 指令

**Enable OTA AT command**：使能 OTA 类 AT 指令

**Enable WLAN AT command**：使能 WLAN 类 AT 指令

**Enable TCP/IP AT command**：使能 TCP/IP 类 AT 指令

**Version**：配置软件包版本。

配置完成后让 RT-Thread 的包管理器自动更新，或者使用 `pkgs --update` 命令更新包到 BSP 中。

## 3、使用 atsrv_socket 软件包

在 menuconfig 打开 atsrv_socket 后，当进行 bsp 编译时，它会被加入到 bsp 工程中进行编译。

* 完整的 AT 命令资料可以访问 [AT 命令 - RT-Thread 文档中心](https://www.rt-thread.org/document/site/programming-manual/at/at/)
* 更多文档位于 [`/docs`](/docs) 下，使用前 **务必查看**

## 4、注意事项

* AT 串口与 Debug 串口设备不可以共用。
* 发送 AT 命令时，需在串口工具勾选**回车换行**

## 5、联系方式 & 感谢

* 维护：shaoguoji
* 主页：https://github.com/RT-Thread-packages/atsrv_socket
