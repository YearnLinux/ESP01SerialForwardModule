//  MIT License

//  Copyright (c) 2021 悦妃云

//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:

//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.

//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

//  website   : https://www.yfyun.xin
//  bolg      : https://blog.csdn.net/u012168125
//  project   : https://gitee.com/yfyun/esp01-serial-forward-module


#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <uri/UriBraces.h>
#include <uri/UriRegex.h>
#include <ESP8266HTTPClient.h>
#include "WEB_HTML_PAGE.h"

#define GPIO0 0
#define GPIO2 2
#define APSSID "ESP_TRAN_AP"

byte _WIFIWorkState = 0; // 0/未连接 1/连接WIFI 2/AP模式
unsigned long _LEDBlinkTime = 0; // 板载 LED 灯闪烁时间

WiFiClient _Tcp;
unsigned long _HeartbeatTime = 0;   //TCP心跳时间       

DNSServer _DnsServer;
const byte _DNSPORT = 53;
ESP8266WebServer _WebServer(80);

void HtmlHome()
{
  String mHtml = _WIFIWorkState == 1 ? "404" : String(TARND_SET_PAGE);
  mHtml.replace("{name}",eepromReadStr(0,50));
  mHtml.replace("{ip}",eepromReadStr(51,15));
  mHtml.replace("{port}",eepromReadStr(67,5));
  _WebServer.send(200, "text/html", mHtml);
}

void HtmlWifi()
{
  _WebServer.send(200, "text/html", _WIFIWorkState == 1 ? "404" : String(HOME_WIFI_SET_PAGE));
}

void ApiSaveWifi()
{
  _WebServer.send(200, "text/plane", _WebServer.arg("name") != "" ? "OK" : "FAIL");
  if (_WebServer.arg("name") != "")
  {
    delay(800);
    WiFi.softAPdisconnect();
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    if (_WebServer.arg("pass") == "") {
      WiFi.begin(_WebServer.arg("name"));
    }else{
      WiFi.begin(_WebServer.arg("name"), _WebServer.arg("pass"));
    }
    delay(1000);
    ESP.restart();
  }
}

void ApiSaveServer()
{
  int state = 0;
  eepromWriteStr(0,50,_WebServer.arg("name"));
  eepromWriteStr(51,15,_WebServer.arg("ip"));
  eepromWriteStr(67,5,_WebServer.arg("port"));
  if(_WebServer.arg("name") == eepromReadStr(0,50)) {state++;}
  if(_WebServer.arg("ip") == eepromReadStr(51,15)) {state++;}
  if(_WebServer.arg("port") == eepromReadStr(67,5)) {state++;}
  _WebServer.send(200, "text/plane", state == 3 ? "OK" : "FAIL");
}

String eepromReadStr(int startAddress, int len)
{
  String readValue;
  for (int i = startAddress; i < startAddress + len; i++)
  {
    byte r_t = EEPROM.read(i);
    if (r_t != 255){readValue += String((char)r_t);}
  }
  readValue.trim();
  return readValue;
}

void eepromWriteStr(int startAddress, int len, char value[])
{
  for (int i = startAddress; i < startAddress + len; i++)
  {
    if (i - startAddress < strlen(value))
    {
      EEPROM.write(i, (byte)value[i - startAddress]);
    }
    else
    {
      EEPROM.write(i, 0);
    }
  }
  EEPROM.commit();
}

void eepromWriteStr(int startAddress, int len, String value)
{
  char charTemp[len + 1];
  value.toCharArray(charTemp, len + 1);
  eepromWriteStr(startAddress, len, charTemp);
}

bool isStringNullOrEmpty(String value){
  value.trim();
  return value == "null" || value == "NULL" || value == "";
}

bool IsResetWIFI()
{
  return WiFi.SSID() == "" || WiFi.SSID() == "RESETDEV";
}

void ConnectWIFI(bool start)
{
  if (WiFi.getMode() == WIFI_STA && WiFi.status() == WL_CONNECTED && WiFi.SSID() != "RESETDEV"){
    BlinkLED(0);
    _WIFIWorkState = 1;
    if(start){
      // WiFi.config(IPAddress(192,168,3,252), IPAddress(192,168,3,1),IPAddress(255,255,255,0),IPAddress(114,114,114,114),IPAddress(211,136,192,6)); //设置静态IP
      delay(1000);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
    };
    return;
  }
  
  if (!IsResetWIFI())
  {
    _WIFIWorkState = 0;
    String ssid = WiFi.SSID();
    String psk = WiFi.psk();
    Serial.println("Connecting to Network.");
    Serial.print("WIFI Name : ");
    Serial.println(ssid);
    Serial.print("WIFI Password : ");
    Serial.println(psk);
    // WiFi.config(IPAddress(192,168,3,252), IPAddress(192,168,3,1),IPAddress(255,255,255,0),IPAddress(114,114,114,114),IPAddress(211,136,192,6));//设置静态IP
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.setAutoReconnect(true);
    WiFi.hostname("ESP-TRAN");
    WiFi.begin(ssid, psk);
    volatile unsigned long connTime = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
      BlinkLED(250);delay(250);Serial.print(".");
      if (millis() - connTime > 20000){ break; }
    }
    Serial.println("");
    if (WiFi.status() == WL_CONNECTED)
    {
      IPAddress myIP = WiFi.localIP();
      Serial.println("WiFi connected.");
      Serial.print("IP address: ");
      Serial.println(myIP);
      _WIFIWorkState = 1;
      BlinkLED(0);
    }else{
      Serial.println("WiFi connection failed");
    }
  }
  else
  {
    if (_WIFIWorkState == 2) { return; }
    BlinkLED(1);
    _WIFIWorkState = 2;
    Serial.println("Configuring access point...");
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(IPAddress(192, 168, 8, 1), IPAddress(192, 168, 8, 1), IPAddress(255, 255, 255, 0));
    WiFi.softAP(APSSID);
    _DnsServer.start(_DNSPORT, "*", IPAddress(192, 168, 8, 1));
    IPAddress myIP = WiFi.softAPIP();
    Serial.println("Open url address : http://" + myIP.toString());
  }
}

bool IsReset = false;
void ICACHE_RAM_ATTR ResetButton(); //中断检测 Rest WIFI 事件

void ResetButton(){
  if (IsReset) { return; }
  BlinkLED(1);
  IsReset = true;
  bool resetOk = false;
  unsigned long PressTime = millis();
  Serial.println("Reseting.");
  while (digitalRead(GPIO0) == 0)
  {
    if(millis() - PressTime >= 5000) { // 按下 5 秒重置 WIFI
      resetOk = true;
      WiFi.disconnect();
      WiFi.mode(WIFI_STA);
      WiFi.begin("RESETDEV","RESETDEV");
      Serial.println("Reset OK.");
      Serial.println("Rebooting.");
      BlinkLED(0);
      delay(2000);
      ESP.restart();
      break;
    }
  }
  BlinkLED(0);
  IsReset = false;
  if (!resetOk) {Serial.println("Reset fail.");}
}

void setup()
{
  delay(2000);
  Serial.begin(115200);
  Serial.println();
  EEPROM.begin(80);
  
  pinMode(GPIO0,INPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  //设置中断引脚  RISING ：在引脚为低变高电平时触发中断。CHANGE ：在引脚更改值时触发中断。FALLING ：当引脚从高电平变为低电平时触发中断。  
  attachInterrupt(digitalPinToInterrupt(GPIO0), ResetButton, FALLING);
  ConnectWIFI(true);

  if (IsResetWIFI()) {
    _WebServer.on(F("/"), HtmlHome);
    _WebServer.on(F("/WIFI"), HtmlWifi);
    _WebServer.on(F("/SAVEWIFI"), ApiSaveWifi);
    _WebServer.on(F("/SAVESERVER"), ApiSaveServer);
    _WebServer.onNotFound(HtmlHome);
    _WebServer.begin();
    Serial.println("HTTP server started, port 80");
  }
}

void loop()
{
  ConnectWIFI(false);
  if (IsResetWIFI())
  {
    _DnsServer.processNextRequest();
    _WebServer.handleClient();
  }else{
    if(_WIFIWorkState == 1) 
    {
      if(_Tcp.status() == 0 && millis() - _HeartbeatTime > 5000) 
      {
        _Tcp.stop();
        _Tcp.flush();
        Serial.flush();
        String mDevName = eepromReadStr(0,50);
        String mTcpServer = eepromReadStr(51,15) + ":" + eepromReadStr(67,5);
        Serial.println("Connecting to tcp server.");
        Serial.print("DevName : ");
        Serial.println(mDevName);
        Serial.print("TcpServer : ");
        Serial.println(mTcpServer);
        _Tcp.connect(eepromReadStr(51,15),eepromReadStr(67,5).toInt()); 
        if(_Tcp.status() == 4) 
        {
          String mJson = "{\"DevName\":\"{name}\",\"TcpServer\":\"{server}\",\"Hardware\":\"ESP01S\"}";
          mJson.replace("{name}",mDevName);
          mJson.replace("{server}",mTcpServer);
          _Tcp.println(mJson);
        }
        _HeartbeatTime = millis();
      }

      if(_Tcp.status() == 4) 
      {
        bool mUpdateTime = false;
        if(millis() - _HeartbeatTime >= 30000) { _Tcp.write(0x00); mUpdateTime = true; }
        while (_Tcp.available() > 0) { Serial.write(_Tcp.read()); mUpdateTime = true; }
        while (Serial.available() > 0) { _Tcp.write(Serial.read()); mUpdateTime = true; }
        if (mUpdateTime) { _HeartbeatTime = millis(); }
      }
    }
  }
  BlinkLED(500);
  CheckTime();
}

void BlinkLED(uint Interval){
  if(Interval == 0 || Interval == 1)
  {
    digitalWrite(LED_BUILTIN,Interval == 0 ? HIGH:LOW);
  }else if (_WIFIWorkState == 0 && millis() - _LEDBlinkTime >= Interval)
  {
    digitalWrite(LED_BUILTIN,digitalRead(LED_BUILTIN) == HIGH ? LOW:HIGH);
    _LEDBlinkTime = millis();
  }
}

void CheckTime()
{
  unsigned long mTime = millis();
  _LEDBlinkTime = mTime < _LEDBlinkTime ? mTime : _LEDBlinkTime;
  _HeartbeatTime = mTime < _HeartbeatTime ? mTime : _HeartbeatTime;
}