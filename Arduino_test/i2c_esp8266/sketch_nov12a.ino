#include <Wire.h>

void setup() {
  Serial.begin(115200);  // 初始化串口
  Wire.begin(4, 5);    // 设置I2C引脚（D2为SDA，D1为SCL）
  Serial.println("\nI2C扫描开始...");
}

void loop() {
  byte error, address;
  int nDevices = 0;

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C设备找到，地址为 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");

      nDevices++;
    } else if (error == 4) {
      Serial.print("未知错误 at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("没有找到I2C设备\n");
  else
    Serial.println("扫描完成\n");

  delay(5000); // 5秒后重新扫描
}
