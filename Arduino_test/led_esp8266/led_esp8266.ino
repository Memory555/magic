#include <Arduino.h>

// 定义 RGB LED 的引脚
#define RED_PIN 5   // R - GPIO5 (D1)
#define GREEN_PIN 4 // G - GPIO4 (D2)
#define BLUE_PIN 0  // B - GPIO0 (D3)

// 初始化 PWM 值（亮度：0-255）
int redValue = 0;
int greenValue = 0;
int blueValue = 0;

// 设置颜色函数
void setColor(int red, int green, int blue) {
    analogWrite(RED_PIN, red);    // 设置红色亮度
    analogWrite(GREEN_PIN, green); // 设置绿色亮度
    analogWrite(BLUE_PIN, blue);   // 设置蓝色亮度
}

void setup() {
    // 初始化引脚为输出模式
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);

    // 启动 PWM 功能
    analogWriteRange(255); // 设置 PWM 范围为 0-255
    analogWriteFreq(1000); // 设置 PWM 频率为 1kHz
}

void loop() {
    // 设置颜色为红色
    setColor(255, 0, 0);
    delay(1000);

    // 设置颜色为绿色
    setColor(0, 255, 0);
    delay(1000);

    // 设置颜色为蓝色
    setColor(0, 0, 255);
    delay(1000);

    // 混合颜色（紫色）
    setColor(128, 0, 128);
    delay(1000);

    // 混合颜色（白色）
    setColor(255, 255, 255);
    delay(1000);

    // 关闭 LED
    setColor(0, 0, 0);
    delay(1000);
}
