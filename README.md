# 物联网--智能遥控和语音控制系统

本项目基于 ESP8266 微控制器实现了一个智能控制系统。系统整合了多个外设，包括红外 (IR) 发射器、MPU6050 运动传感器、RGB LED、ASR-Pro 模块和 Wi-Fi 连接ESP01s。功能包括基于红外的设备控制、基于语音的舵机控制、语音交互和基于 HTTP 的远程PPT翻页。

---

## 功能特点

### 1. **Wi-Fi 连接**
   - 连接到 Wi-Fi 网络以启用 HTTP 服务器和客户端功能。
   - 支持配置 SSID 和密码。

### 2. **红外遥控**
   - 发送红外信号以控制设备，例如空调。
   - 包含预定义的格力空调电源开/关红外信号。

### 3. **PPT翻页**
   - 使用 MPU6050 运动传感器检测水平和垂直方向上的快速和慢速摆动或者按键输入。
   - 实现无线PPT翻页控制。

### 4. **RGB LED 控制**
   - 通过 PWM 实现全彩控制，用于状态指示或视觉反馈。

### 5. **ASR-Pro 语音交互**
   - 使用 ASR-Pro 模块实现语音识别功能。
   - 支持开关灯，开关空调，内容显示、温度和湿度查询、时间查询等功能。
   - 通过软件串口与 ASR-Pro 模块通信。

### 6. **HTTP 服务器和客户端**
   - 在 ESP8266 上托管 HTTP 服务器以接收命令。
   - 向外部服务发送 HTTP 请求以执行翻页等操作。

---

## 使用的组件

### 硬件
- **ESP8266 NodeMCU**
- **MPU6050 运动传感器**
- **红外 LED**
- **RGB LED**
- **按键**
- **Wi-Fi 路由器或者热点**
- **ASR-Pro 模块**
- **ESP01s单片机**
- **sg90舵机**

### 库
- `Arduino.h`
- `ESP8266WiFi.h`
- `ESP8266HTTPClient.h`
- `ESP8266WebServer.h`
- `WiFiUdp.h`
- `NTPClient.h`
- `SoftwareSerial.h`
- `ESP8266Ping.h`
- `IRremoteESP8266.h`
- `IRsend.h`
- `I2Cdev.h`
- `MPU6050.h`

---

## 引脚配置--仅给出esp8266部分的连接，详细见引脚图

| 组件               | 引脚        |
| ------------------ | ----------- |
| 红外 OUT           | GPIO16 (D0) |
| MPU6050 SDA        | GPIO4 (D2)  |
| MPU6050 SCL        | GPIO5 (D1)  |
| RGB LED (红)       | GPIO0 (D3)  |
| RGB LED (绿)       | GPIO2 (D4)  |
| RGB LED (蓝)       | GPIO12 (D6) |
| 按键输入           | GPIO1 (D9)  |
| ASRPRO软件串口PA2  | GPIO13 (D7) |
| ASRPRO软件串口 PA3 | GPIO15 (D8) |

---

## 关键功能

### `setColor(uint8_t r, uint8_t g, uint8_t b)`
- 将 RGB LED 设置为指定颜色，用于指令反馈。

---

## PPT翻页

| 手势类型      | 触发条件                   |
| ------------- | -------------------------- |
| PPT翻页下一页 | `|ΔAx| > 5000`或者按键按下 |
| PPT翻页上一页 | `|ΔAy| > -5000`            |

---

## 红外命令

| 命令     | 信号描述                               |
| -------- | -------------------------------------- |
| 开个空调 | 预定义的原始数据（来自安可信遥控助手） |
| 关个空调 | 预定义的原始数据（来自安可信遥控助手） |

---

## 开关灯--esp01部分不做详细解释，可以直接去看代码esp01_wifi

| 命令   | 描述                                                         |
| ------ | ------------------------------------------------------------ |
| 开个灯 | 语言接收后反馈给esp8266，esp8266与esp01建立tcp连接，告知sg90舵机进行90度拨动 |
| 关个灯 | 语言接收后反馈给esp8266，esp8266与esp01建立tcp连接，告知sg90舵机进行0度拨动 |

## ASR-Pro 语言功能

| 功能类型     | 说明                       |
| ------------ | -------------------------- |
| 内容显示     | 显示与LLM询问后的结果      |
| 温度湿度查询 | 查询并返回环境的温度和湿度 |
| 时间查询     | 返回当前时间信息           |
| 开灯或者关灯 | 让sg90舵机转动             |
| 开关空调     | 让红外模块发送编码         |
| 播放音乐     | 目前只收录了一首音乐       |

---

## 文件说明

3D_print文件夹是魔杖的3D模型，但是个人认为这个系统比较适合发展成片状的智能管家

Arduino_test文件夹是对每一个模块组件的单点测试

Arduion_main文件夹包含esp8266与esp01s单片机的烧录文件

tianwen_block文件夹包括语言模型和asrpro烧录文件

web_py文件夹包含问题提问服务器，问题回答服务器，控制ppt翻页服务器

各模块代码分解.md--单模块实现代码

各模块接线图.md--具体的接线图

前期准备和相关参考视频问题汇总.md--参考资源和前期问题汇总

注意：

整个网络环境要保证同处在一个局域网（pc端，esp8266端。esp01端）

ppt翻页和文字输入进行聊天问答前要把三个服务器打开

## 致谢

特别感谢贡献者和开源社区提供的必要库和资源，各模块参考可去看前期准备和相关参考视频问题汇总.md。
