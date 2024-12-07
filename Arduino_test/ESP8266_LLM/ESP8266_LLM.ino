#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char *WIFI_SSID = "玛卡巴卡"; // 填写你的WIFI名称
const char *WIFI_PWD = "123123123";  // 填写你的WIFI密码

WiFiClient client;  // 创建 获取问题 的 HTTP 服务器实例
ESP8266WebServer server(80); // 创建 ESP8266的 HTTP 服务器实例

void setup() {
  Serial.begin(115200);
  // 连接Wi-Fi网络
  wificonnect();
  Serial.println("Ready");
  // 设置路由，当访问 /question 时调用 handleQuestion 函数
  server.on("/question", HTTP_POST, handleQuestion);
  
  // 启动服务器
  server.begin();
  Serial.println("ESP8266_HTTP server started");
}

void loop() {
  server.handleClient(); // 处理客户端请求
}

// 连接Wi-Fi网络
void wificonnect() {
  WiFi.begin(WIFI_SSID, WIFI_PWD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
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
