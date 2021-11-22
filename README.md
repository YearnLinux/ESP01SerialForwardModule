# ESP01SerialForwardModule

#### LICENSE
* 本项目使用 MIT 授权协议

#### 介绍
* 乐鑫 ESP01S 串口数据转发模块，可以将任何单片机串口引脚数据转发到 TCP 网络数据。
* 某宝搜索 ESP01S 可以搜索到很多这个板子, 买到板子后烧录这个代码到板子里面即可。
* 作者使用的IDE是 VSCODE 里面安装了 Arduino 插件，具体百度使用教程。
* ESP01S 有集成好的固件烧录器挺方便的,如果自己没有CH340G的下载器可以购买一个,具体看下面的图片,有的话就不需要购买。
* 乐鑫官方ESP01S文档 : https://docs.ai-thinker.com/_media/esp8266/docs/esp-01s_product_specification_zh_v1.0.pdf

#### 环境搭建参考文档
* 参考文档 1 : https://segmentfault.com/a/1190000017465290
* 参考文档 2 : https://blog.csdn.net/CCC_122/article/details/97611228
* ESP8266 环境配置 : https://www.jianshu.com/p/cb0274d612b5
* ESP01S 这个板子实际用的芯片是 ESP8266 所以搭建的环境是 ESP8266 的教程一样
* 烧录的时候请注意选择的板子类型,选择错误将导致 SPI FLASH 不对,导致 EEPROM 读写数据时出现问题,作者选择的是 Generic ESP8266 Module 进行烧录

#### 重置模块设置
* 第一次烧录代码到硬件里面先执行一次重置设置的操作,否则可能会出现问题.
* 将模块引脚 IO0 接一根线连接到模块上的 GND 引脚, 板载LED灯开始常亮,直到LED灯熄灭后把这根连接线拔出,重启模块 LED 灯常亮,进入 AP 配置模式

#### 使用方法
* 代码烧录到硬件里面后重启一下, 按照上面的重置模块设置先执行一次重置
* 重置成功后模块进入 AP 配置模式, 并且 LED 灯常亮
* 手机搜索 WIFI 找到名称为 ESP-TRAN-AP 的 WIFI 连接进去
* 连接成功后会自动跳转到转发数据的 TCP 服务器地址配置页面
* TCP 服务器地址和设备名称配置成功后, 跳转到 WIFI 配置页面, 输入你家的 WIFI 名称和密码进行连接
* 配置好 WIFI 名称和密码后, 模块会自动重启尝试连接到你家的 WIFI,连接过程中 LED 灯会一直闪烁, 直到 WIFI 连接成功后 LED 灯会完全熄灭,表示模块已成功连接到你家 WIFI

#### 开发 TCP 服务端注意
* 模块每次成功连接到 TCP 服务器后会自动上传一次设备数据
* DevName 为在 AP 模块配置的设备名称,用于多设备时区分是那台设备
* TcpServer 为在 AP 模块配置的TCP服务地址和端口号
* Hardware 为当前设备类型, 目前固定不可改
```
{"DevName":"Dev1","TcpServer":"192.168.1.183:60000","Hardware":"ESP01S"}
```

#### 接线说明
* ESP01S(RX)->单片机(TX)
* ESP01S(TX)->单片机(RX)
* ESP01S(GND)->单片机(GND)共地
* ESP01S(3V3)->单片机(3.3V)供电

#### ESP01S 引脚图
![image](https://gitee.com/yfyun/esp01-serial-forward-module/raw/master/ESP01S.jpg)

#### ESP01S CH340下载器
![image](https://gitee.com/yfyun/esp01-serial-forward-module/raw/master/DOWNLOAD.jpg)