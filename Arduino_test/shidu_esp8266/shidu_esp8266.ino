#include <DHT.h>

// 定义DHT传感器类型和数据引脚
#define DHTPIN 13     // DHT传感器连接的引脚（ESP8266的D7）
#define DHTTYPE DHT11 // 或者使用 DHT22，根据传感器类型选择

DHT dht(DHTPIN, DHTTYPE); // 初始化DHT传感器

void setup() {
    Serial.begin(115200); // 初始化串口通信
    dht.begin();          // 启动DHT传感器
    Serial.println("DHT传感器初始化完成");
}

void loop() {
    // 读取温度和湿度
    float humidity = dht.readHumidity();    // 读取湿度
    float temperature = dht.readTemperature(); // 读取温度（摄氏度）

    // 检查是否读取失败
    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("读取DHT传感器数据失败！");
        return;
    }

    // 输出温度和湿度
    Serial.print("湿度: ");
    Serial.print(humidity);
    Serial.print("%\t");
    Serial.print("温度: ");
    Serial.print(temperature);
    Serial.println("°C");

    delay(2000); // 每2秒读取一次
}
