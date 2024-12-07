const int buttonPin = 5; // 按键连接到 D1 引脚
int buttonState = 0;     // 按键状态变量

void setup() {
  pinMode(buttonPin, INPUT); // 将按键引脚设置为输入模式
  Serial.begin(115200);      // 初始化串口通信
}

void loop() {
  buttonState = digitalRead(buttonPin); // 读取按键状态
  if (buttonState == HIGH) {
    Serial.println("Button Pressed"); // 如果按下按钮
  } else {
    Serial.println("Button Released"); // 如果松开按钮
  }
  delay(100); // 延时，避免抖动
}
