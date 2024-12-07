#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>
#include <ESP8266Ping.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <I2Cdev.h>
#include <MPU6050.h>

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

// IR signal data for Power On (GREE AC)
uint16_t rawDataPowerOn[279] = {9046, 4506,  650, 1640,  648, 534,  650, 560,  650, 1640,  648, 534,  650, 560,  650, 534,  648, 534,  650, 1638,  652, 558,  650, 534,  650, 1638,  650, 558,  650, 534,  650, 532,  650, 560,  650, 532,  674, 510,  648, 562,  650, 534,  650, 532,  650, 1664,  650, 532,  650, 534,  652, 558,  648, 534,  650, 534,  648, 562,  650, 1638,  648, 536,  648, 1640,  646, 564,  646, 536,  646, 1642,  646, 564,  648, 20010,  646, 564,  646, 536,  646, 536,  648, 562,  646, 536,  646, 538,  646, 562,  646, 538,  646, 538,  644, 566,  644, 538,  646, 538,  644, 566,  644, 538,  646, 1644,  644, 540,  644, 566,  644, 538,  644, 540,  644, 566,  644, 540,  642, 540,  642, 566,  644, 540,  642, 542,  642, 568,  642, 542,  640, 542,  642, 568,  642, 542,  642, 542,  642, 568,  640, 40042,  9008, 4538,  618, 1650,  638, 592,  618, 566,  616, 1672,  618, 592,  592, 592,  616, 566,  618, 592,  590, 1698,  616, 566,  618, 566,  616, 1698,  618, 566,  616, 568,  616, 594,  616, 566,  616, 566,  616, 594,  616, 568,  616, 568,  616, 594,  616, 1672,  616, 568,  616, 594,  616, 568,  614, 568,  616, 594,  616, 568,  614, 1674,  614, 1674,  616, 1698,  616, 568,  614, 568,  614, 1700,  590, 594,  616, 20042,  616, 594,  614, 568,  616, 568,  614, 594,  614, 568,  616, 568,  614, 596,  614, 570,  614, 570,  614, 596,  614, 568,  614, 568,  614, 596,  614, 572,  612, 568,  614, 596,  614, 570,  614, 570,  614, 596,  614, 570,  614, 570,  614, 596,  614, 570,  612, 570,  612, 598,  612, 570,  612, 572,  612, 598,  612, 572,  610, 572,  612, 1704,  610, 1680,  608};  // GREE Power On

// IR signal data for Power Off (GREE AC)
uint16_t rawDataPowerOff[279] = {9046, 4506,  650, 1640,  652, 532,  650, 558,  650, 534,  650, 534,  648, 562,  650, 534,  648, 536,  648, 1666,  648, 534,  650, 534,  650, 1638,  650, 560,  648, 536,  648, 534,  650, 560,  648, 534,  652, 532,  650, 560,  648, 534,  650, 534,  648, 1666,  650, 534,  650, 534,  648, 562,  652, 532,  650, 534,  650, 560,  652, 1638,  650, 534,  650, 1664,  652, 532,  650, 534,  650, 1640,  650, 560,  650, 20010,  648, 560,  648, 536,  648, 536,  648, 562,  648, 536,  648, 536,  648, 562,  644, 538,  646, 538,  646, 564,  646, 536,  646, 538,  646, 564,  646, 536,  646, 1642,  646, 564,  646, 538,  644, 538,  644, 566,  644, 538,  646, 538,  644, 566,  644, 538,  644, 540,  642, 568,  642, 540,  644, 540,  642, 566,  644, 540,  642, 540,  642, 566,  644, 1646,  642, 40042,  9012, 4516,  642, 1648,  640, 570,  640, 544,  640, 544,  640, 570,  640, 544,  640, 544,  640, 570,  640, 1650,  638, 566,  618, 572,  612, 1696,  618, 566,  618, 566,  618, 592,  616, 566,  618, 566,  618, 592,  620, 564,  618, 566,  616, 592,  618, 1672,  616, 566,  616, 592,  618, 566,  618, 566,  616, 592,  618, 566,  618, 1672,  616, 1672,  616, 1700,  616, 568,  616, 568,  616, 1700,  618, 592,  616, 20042,  616, 594,  614, 568,  616, 568,  614, 594,  614, 568,  616, 568,  614, 596,  614, 570,  614, 570,  614, 596,  614, 568,  614, 568,  614, 596,  614, 572,  612, 568,  614, 596,  614, 570,  614, 570,  614, 596,  614, 570,  614, 570,  614, 596,  614, 570,  612, 570,  612, 598,  612, 570,  612, 572,  612, 598,  612, 572,  610, 572,  612, 1704,  610, 1680,  608};  // GREE Power Off

// 声明全局变量--存储单次回答
String globalResponse;

// MPU6050对象（默认I2C地址为0x68）
MPU6050 accelgyro;

int16_t ax, ay, az; // 加速度计数据
int16_t gx, gy, gz; // 陀螺仪数据

// 阈值设置，需根据实际情况调整
const int16_t SWING_THRESHOLD_FAST = 30000; // Y轴加速度变化阈值
const int16_t SWING_THRESHOLD_SLOW = 15000; // X轴加速度变化阈值
const float DIAGONAL_RATIO_THRESHOLD = 0.5; // 判断斜向挥动的比例阈值

int16_t lastAy = 0;                    // 上一次的Y轴加速度
int16_t lastAx = 0;                    // 上一次的X轴加速度

String swing = ""; // 动作类型：横向快挥动、横向慢挥动、竖向快挥动、竖向慢挥动

WiFiClient tcpclient;
WiFiClient client;  // 创建 获取问题 的 HTTP 服务器实例
ESP8266WebServer server(80); // 创建 ESP8266的 HTTP 服务器实例
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

const char *WIFI_SSID = "玛卡巴卡"; // 填写你的WIFI名称及密码
const char *WIFI_PWD = "123123123";
#define PCIP "192.168.224.100"         // 地址一：电脑ip地址，这里存在用命令开关机电脑，注意在路由器上绑定ip，此处主要是用于ping一下，再次查询真实开关机状态
#define TCP_SERVER_IP "192.168.224.209" // 地址一：控制电脑开关的继电器esp01s模块ip地址，注意在路由器上绑定ip
#define TCP_SERVER_PORT 6000

#define MAX_COMMAND_LENGTH 10
char command[MAX_COMMAND_LENGTH] = ""; // 存储接收到的命令字符串
int commandLength = 0;                 // 当前接收到的命令长度

// 定义与ASRPRO引脚连接
SoftwareSerial MySerial(13, 15); // 定义软串口对象，RX=13, TX=15

// 定义红外发射引脚
const uint16_t kIrLedPin = 16; // D0引脚
IRsend irsend(kIrLedPin);

// 定义 RGB LED 的引脚
#define RED_PIN 0   // R - GPIO5 (D3)
#define GREEN_PIN 14 // G - GPIO4 (D5)
#define BLUE_PIN 12  // B - GPIO0 (D6)

// 定义MUP6050引脚
#define SW_SDA 4   // SDA - GPIO4 (D2)
#define SW_SCL 5 // SCL - GPIO5 (D1)

char chBuffer[64];
int p = 0;
const unsigned long TIMEOUT = 10000; // 设置超时时间为 10 秒

void setup()
{
    // 初始化引脚为输出模式
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);

    // 启动 PWM 功能
    analogWriteRange(255); // 设置 PWM 范围为 0-255
    analogWriteFreq(1000); // 设置 PWM 频率为 1kHz

    setColor(255, 255, 255);

    // 初始化串口和软串口
    Serial.begin(9600);
    MySerial.begin(115200);
    
    // 初始化红外发送信号
    irsend.begin();

    // 初始化I2C（D2为SDA，D1为SCL）
    Wire.begin(SW_SDA, SW_SCL);
    
    // 初始化MPU6050
    Serial.println("初始化I2C设备...");
    accelgyro.initialize();

    // 测试MPU6050连接
    Serial.println("检测设备连接...");
    Serial.println(accelgyro.testConnection() ? "MPU6050连接成功" : "MPU6050连接失败");

    // 设置路由，当访问 /question 时调用 handleQuestion 函数
    server.on("/question", HTTP_POST, handleQuestion);
  
    // 启动服务器
    server.begin();
    Serial.println("ESP8266_HTTP server started");

    //  连接Wi-Fi
    wificonnect();
    Serial.println("Ready");

    // 时间初始化
    timeClient.setTimeOffset(60*60*8);	//东八区时间设置
    timeClient.begin();
    Serial.println("时间初始化完成");
    timeClient.update();
    Serial.println("当前时间" + timeClient.getFormattedTime());
}

void loop()
{
    server.handleClient(); // 处理客户端请求
    if (MySerial.available())
    {
        int commandLength = 0; // 当前接收到的命令长度
        while (MySerial.available() > 0)
        {
            char c = MySerial.read();
            if (c == '%')
            {
                break;
            }
            if (commandLength >= MAX_COMMAND_LENGTH - 1)
            {
                break;
            }
            command[commandLength++] = c;
        }
        // command[commandLength] = '\0'; // 末尾添加"\0"作为字符串结束标志
        if (commandLength > 0)
        {
            String c = String(command);
            c.trim();
            Serial.println("Received command: " + c); // 输出接收到的命令
            commandLength = 0;                        // 清空命令长度变量
            memset(command, 0, sizeof(command));
            executeCommand(c); // 执行命令
        }
    }
}

// 检测挥动类型并返回结果
String detectSwing() {
  while (true) {
    // 从MPU6050读取加速度数据
    accelgyro.getAcceleration(&ax, &ay, &az);

    // 计算加速度变化
    int16_t deltaAx = abs(ax - lastAx);
    int16_t deltaAy = abs(ay - lastAy);

    // 更新上一次的加速度值
    lastAx = ax;
    lastAy = ay;

    // 判断挥动类型
    if (deltaAy > SWING_THRESHOLD_FAST && deltaAx < SWING_THRESHOLD_SLOW) {
        return "快速横向挥动";
    }
    // else if (deltaAy < SWING_THRESHOLD_FAST && deltaAy > SWING_THRESHOLD_SLOW && deltaAx < SWING_THRESHOLD_SLOW) {
    //     return "慢速横向挥动";
    // }
    else if (deltaAx > SWING_THRESHOLD_FAST && deltaAy < SWING_THRESHOLD_SLOW) {
        return "快速竖向挥动";
    }
    // else if (deltaAx < SWING_THRESHOLD_FAST && deltaAx > SWING_THRESHOLD_SLOW && deltaAy < SWING_THRESHOLD_SLOW) {
    //     return "慢速竖向挥动";
    // }
    // 延时一小段时间，防止高频读取导致资源浪费
    delay(10);
  }
}

/* 判断命令名称并执行对应的方法 */
void executeCommand(String command)
{
    if (command != "TIME" || command != "SHOW"){
      // 检测挥动并获取类型
      String swing = detectSwing();

      // 打印挥动类型
      Serial.println(swing);

      if (command == "LED_ON" && swing == "快速横向挥动")
      { // 开灯舵机命令
          LED_ON();
      }
      else if (command == "LED_OFF" && swing == "快速横向挥动")
      { // 关灯舵机命令
          LED_OFF();
      }
      else if (command == "AIR_ON" && swing == "快速竖向挥动")
      { // 开空调命令
          AIR_ON();
      }
      else if (command == "AIR_OFF" && swing == "快速竖向挥动")
      { // 关空调命令
          AIR_OFF();
      }
      else
      { // 未知命令
          Serial.println("指令或者手势不正确");
      }
    }
    else if(command == "TIME")
    { // 显示时间命令
        TIME_SHOW();
    }
    else if(command == "SHOW"){
      SHOW_OLED();
    } 
}

void AIR_ON()
{
  // 混合颜色（紫色）
  setColor(128, 0, 128);
  delay(2000); // 不放延时可能发送太快，还没有对准空调
  irsend.sendRaw(rawDataPowerOn, 279, 38);
  irsend.sendRaw(rawDataPowerOn, 279, 38);
  irsend.sendRaw(rawDataPowerOn, 279, 38); 
  Serial.println("Sending Power On Signal");
  MySerial.write("air_on_success%");
  delay(5000);
  setColor(255, 255, 255);
}

void AIR_OFF()
{
  // 混合颜色（红色）
  setColor(255, 0, 0);
  delay(2000);
  irsend.sendRaw(rawDataPowerOff, 279, 38);
  irsend.sendRaw(rawDataPowerOff, 279, 38);
  irsend.sendRaw(rawDataPowerOff, 279, 38); 
  Serial.println("Sending Power Off Signal");
  MySerial.write("air_off_success%");
  delay(5000);
  setColor(255, 255, 255);
}

void TIME_SHOW()
{
  timeClient.update();
  // Serial.println(timeClient.getFormattedTime());
  // MySerial.write(timeClient.getFormattedTime());
  // 获取时间
  String formattedTime = timeClient.getFormattedTime();
  Serial.println(formattedTime); // 打印时间到 Serial

  // 在响应的末尾添加 "%"
  formattedTime.concat("%");
  // 将时间通过 SoftwareSerial 发送
  MySerial.write(formattedTime.c_str()); // 转换为 const char* 再发送
  delay(1000); // 每秒发送一次
}

void LED_ON()
{
    if (Ping.ping(PCIP))
    {
        // 如果能ping通目标PC，则记录一条日志消息并返回
        Serial.println("pc is on...");
        Serial.println("LED_ON Start");
        // 设置颜色为绿色
        setColor(0, 255, 0);
        SendTcpData("LED_ON\r"); // 这里的POWER是与用于与esp01s通信，我在esp01s启用了一个tcp服务
        setColor(255, 255, 255);
    }
}

void LED_OFF()
{

    if (Ping.ping(PCIP))
    {
        // 如果能ping通目标PC，则记录一条日志消息并返回
        Serial.println("pc is on...");
        Serial.println("LED_OFF Start");
        // 设置颜色为蓝色
        setColor(0, 0, 255);
        SendTcpData("LED_OFF\r"); // 这里的POWER是与用于与esp01s通信，我在esp01s启用了一个tcp服务
        setColor(255, 255, 255);
    }
}


void SendTcpData(char *msg)
{
    Serial.print("connecting to tcp server");
    Serial.print(TCP_SERVER_IP);
    Serial.print(":");
    Serial.println(TCP_SERVER_PORT);

    if (!tcpclient.connect(TCP_SERVER_IP, TCP_SERVER_PORT))
    {
        Serial.println("connection failed"); // 如果连接失败，则打印连接失败信息，并返回
        MySerial.write("error%");
        return;
    }
    /* 如果连接成功，则发送一个字符串到TCP服务器 */
    Serial.println("Connected to TCPServer, sending data to server");
    if (!tcpclient.connected())
    {
        Serial.println("connection failed"); // 如果连接失败，则打印连接失败信息，并返回
        MySerial.write("error%");
        return;
    }
    tcpclient.println(msg);

    /* 等待TCP服务器返回消息 */
    Serial.println("waiting for receive from remote Tcpserver...");
    String response = "";
    unsigned long startTime = millis();
    while (tcpclient.connected())
    {
        while (tcpclient.available())
        {
            char ch = tcpclient.read();
            response += ch;
            if (response.endsWith("\r\n"))
            {
                // 完整地读取到了 HTTP 响应的头部，退出循环
                break;
            }
        }
        if (millis() - startTime > TIMEOUT)
        {
            // 如果超时，退出循环
            Serial.println("time out");
            break;
        }
    }

    // 处理接收到的数据（注意去除开始/结尾的空白字符）
    response.trim();
    Serial.print("response:");
    Serial.println(response);
    if (response == "led_on_success")
    {
        // 如果收到有效响应，则记录一条日志消息并返回
        Serial.println("led_on_success");
        MySerial.write("led_on_success%");
    }
    else if(response == "led_off_success")
    {
        // 如果收到有效响应，则记录一条日志消息并返回
        Serial.println("led_off_success");
        MySerial.write("led_off_success%");
    }
    else
    {
        // 否则，记录一条错误消息并返回
        Serial.println("error");
        MySerial.write("error%");
    }

    /* 接受到服务器返回的消息后关闭TCP连接 */
    Serial.println("closing connection");
}

// 设置颜色函数
void setColor(int red, int green, int blue) {
    analogWrite(RED_PIN, red);    // 设置红色亮度
    analogWrite(GREEN_PIN, green); // 设置绿色亮度
    analogWrite(BLUE_PIN, blue);   // 设置蓝色亮度
}

// 连接Wi-Fi网络
void wificonnect()
{
    WiFi.begin(WIFI_SSID, WIFI_PWD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("wificonnect!!!");
    delay(500);
}

// 处理来自 Flask 服务器的 POST 请求
void handleQuestion() {
  if (server.hasArg("question")) {
    String question = server.arg("question");
    Serial.println("Received question: " + question);
    
    // 调用 sendQuestion 函数处理问题
    sendQuestion(question);

    // 返回响应
    server.send(200, "text/plain", "Question received");
  } else {
    server.send(400, "text/plain", "No question provided");
  }
}

// 发送问题到指定的 API
void sendQuestion(String question) {
  HTTPClient http;
  http.begin(client, "http://192.168.224.100:5550/api/query"); // 初始化 HTTP 请求
  http.addHeader("Content-Type", "application/json");

  // 构建 POST 请求数据
  String httpRequestData = "{\"question\":\"" + question + "\"}";
  int httpResponseCode = http.POST(httpRequestData);

  if (httpResponseCode > 0) {
    String response = http.getString();
    String decodedResponse = decodeUnicode(response);
    
    // 解析响应中的内容
    int startIndex = decodedResponse.indexOf("\"response\": \"") + strlen("\"response\": \"");
    int endIndex = decodedResponse.indexOf("\"", startIndex);
    if (startIndex != -1 && endIndex != -1) {
      String extractedResponse = decodedResponse.substring(startIndex, endIndex);
      Serial.println(extractedResponse);
       // 将提取的响应赋值给全局变量
      globalResponse = extractedResponse;
    }
  } else {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

// Unicode 解码函数
String decodeUnicode(String input) {
  String result = "";
  int length = input.length();
  for (int i = 0; i < length; i++) {
    if (input[i] == '\\' && i + 1 < length && input[i + 1] == 'u') {
      if (i + 5 < length) {
        String unicodeHex = input.substring(i + 2, i + 6);
        unsigned int unicodeValue = strtol(unicodeHex.c_str(), NULL, 16);
        if (unicodeValue <= 0x7F) {
          result += (char)unicodeValue;
        } else if (unicodeValue <= 0x7FF) {
          result += (char)(0xC0 | (unicodeValue >> 6));
          result += (char)(0x80 | (unicodeValue & 0x3F));
        } else if (unicodeValue <= 0xFFFF) {
          result += (char)(0xE0 | (unicodeValue >> 12));
          result += (char)(0x80 | ((unicodeValue >> 6) & 0x3F));
          result += (char)(0x80 | (unicodeValue & 0x3F));
        } else {
          result += '?';
        }
        i += 5;
      }
    } else {
      result += input[i];
    }
  }
  return result;
}

void SHOW_OLED()
{
  // 在响应的末尾添加 "%"
  globalResponse.concat("%");
  MySerial.write(globalResponse.c_str()); // 转换为 const char* 再发送
  delay(1000); // 每秒发送一次
}