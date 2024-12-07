#include <ESP8266WiFi.h>
#include <Servo.h>  // 引入舵机库

// 设置Wi-Fi网络名称和密码
const char *WIFI_SSID = "玛卡巴卡"; 
const char *WIFI_PWD = "123123123"; 

WiFiServer server(6000);  // 创建TCP服务器，监听端口6000
Servo myServo;           // 创建舵机对象
int servoPin = 0;       // 设置舵机控制引脚

void setup() {
  // 初始化串口
  Serial.begin(115200);
  delay(10);

  // 连接Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PWD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.print("ESP01S IP Address: ");
  Serial.println(WiFi.localIP());  // 打印ESP01S的IP地址

  // 初始化舵机
  myServo.attach(servoPin);  // 将舵机引脚与舵机对象关联
  myServo.write(90);         // 将舵机设置到中间位置（90度）

  // 启动TCP服务器
  server.begin();
  Serial.println("TCP server started, waiting for connection...");
}

void loop() {
  WiFiClient client = server.available();  // 等待客户端连接
  
  if (client) {
    Serial.println("Client connected");
    String command = "";
    
    // 读取客户端发送的数据
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        command += c;
        
        // 如果接收到完整命令（以\r\n结束）
        if (command.endsWith("\r\n")) {
          Serial.println("Received command: " + command);
          command.trim();  // 去除首尾的空格和换行符
          // 执行命令
          if (command == "LED_ON") {
            // 控制舵机转到左边（0度）//开灯
            myServo.write(0);
            Serial.println("Servo moved to 0 degrees");
            // 发送响应给客户端
            client.println("led_on_success\r\n");
            command = "";  // 清空命令
          } 
          else if (command == "LED_OFF") {
            // 控制舵机转到右边（180度） // 关灯
            myServo.write(180);
            Serial.println("Servo moved to 180 degrees");
            // 发送响应给客户端
            client.println("led_off_success\r\n");
            command = "";  // 清空命令
          }
        }
      }
    }
    client.stop();  // 关闭客户端连接
    Serial.println("Client disconnected");
  }
}
