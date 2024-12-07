#include <IRrecv.h>
#include <IRutils.h>

#define RECV_PIN 14 // 红外接收器连接的 GPIO 引脚 (D5 = GPIO4)

// 创建红外接收对象
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(115200); // 初始化串口通信
  irrecv.enableIRIn();  // 启动红外接收
  Serial.println("IR Receiver is enabled.");
}

void loop() {
  if (irrecv.decode(&results)) {
    // 打印接收到的 RAW 数据
    Serial.println("Received IR Signal:");
    serialPrintUint64(results.value, HEX); // 打印协议码值
    Serial.println("");

    // 如果是 RAW 数据类型，打印详细的 RAW 数组
    if (results.decode_type == UNKNOWN) {
      Serial.println("Unknown protocol. Printing raw data:");
      Serial.println(resultToSourceCode(&results)); // 输出 RAW 数组
    }

    irrecv.resume(); // 准备接收下一个信号
  }
}
