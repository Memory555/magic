#include "I2Cdev.h"
#include "MPU6050.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

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


void setup() {
    // 初始化串口，波特率为115200
    Serial.begin(115200);
    
    // 初始化I2C（D2为SDA，D1为SCL）
    Wire.begin(4, 5);

    // 初始化MPU6050
    Serial.println("初始化I2C设备...");
    accelgyro.initialize();

    // 测试MPU6050连接
    Serial.println("检测设备连接...");
    Serial.println(accelgyro.testConnection() ? "MPU6050连接成功" : "MPU6050连接失败");
}

void loop() {
    // 从MPU6050读取加速度和陀螺仪数据
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // 计算加速度变化
    int16_t deltaAy = ay - lastAy;
    int16_t deltaAx = ax - lastAx;

    // 判断挥动类型
    if (abs(deltaAy) > SWING_THRESHOLD_FAST && abs(deltaAx) < SWING_THRESHOLD_SLOW) {
        // 横向挥动
        swing = "快速横向挥动";
        // Serial.print("检测到快速横向挥动: 加速度/陀螺仪数据:\t");
        // Serial.print(ax); Serial.print("\t");
        // Serial.print(ay); Serial.print("\t");
        // Serial.print(az); Serial.print("\t");
        // Serial.print(gx); Serial.print("\t");
        // Serial.print(gy); Serial.println("\t");
    }
    else if (abs(deltaAy) < SWING_THRESHOLD_FAST && abs(deltaAy) > SWING_THRESHOLD_SLOW && abs(deltaAx) < SWING_THRESHOLD_SLOW) {
        // 横向挥动
        swing = "慢速横向挥动";
        // Serial.print("检测到慢速横向挥动: 加速度/陀螺仪数据:\t");
        // Serial.print(ax); Serial.print("\t");
        // Serial.print(ay); Serial.print("\t");
        // Serial.print(az); Serial.print("\t");
        // Serial.print(gx); Serial.print("\t");
        // Serial.print(gy); Serial.println("\t");
    }

    else if (abs(deltaAx) > SWING_THRESHOLD_FAST && abs(deltaAy) < SWING_THRESHOLD_SLOW) {
        // 竖向挥动
        swing = "快速竖向挥动";
        // Serial.print("检测到快速竖向挥动: 加速度/陀螺仪数据:\t");
        // Serial.print(ax); Serial.print("\t");
        // Serial.print(ay); Serial.print("\t");
        // Serial.print(az); Serial.print("\t");
        // Serial.print(gx); Serial.print("\t");
        // Serial.print(gy); Serial.println("\t");
    }
    else if (abs(deltaAx) < SWING_THRESHOLD_FAST  && abs(deltaAx) > SWING_THRESHOLD_SLOW && abs(deltaAy) < SWING_THRESHOLD_SLOW) {
        // 竖向挥动
        swing = "慢速竖向挥动";
        // Serial.print("检测到慢速竖向挥动: 加速度/陀螺仪数据:\t");
        // Serial.print(ax); Serial.print("\t");
        // Serial.print(ay); Serial.print("\t");
        // Serial.print(az); Serial.print("\t");
        // Serial.print(gx); Serial.print("\t");
        // Serial.print(gy); Serial.println("\t");
    }

    // // 更新上一次的加速度值
    // lastAy = ay;
    // lastAx = ax;

    // 延时100ms
    delay(100);
}
