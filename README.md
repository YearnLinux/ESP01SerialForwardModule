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
* 烧录的时候请注意选择的板子类型,选择错误将导致 SPI FLASH 不对, 导致 EEPROM 读写数据时出现问题, 作者选择的是 Generic ESP8266 Module 进行烧录

#### 接线说明
* ESP01S(RX)->单片机(TX)
* ESP01S(TX)->单片机(RX)
* ESP01S(GND)->单片机(GND)共地
* ESP01S(3V3)->单片机(3.3V)供电

#### ESP01S 引脚图
![image](https://gitee.com/yfyun/esp01-serial-forward-module/blob/master/ESP01S.jpg)

#### ESP01S CH340下载器
![image](https://gitee.com/yfyun/esp01-serial-forward-module/blob/master/DOWNLOAD.jpg)

