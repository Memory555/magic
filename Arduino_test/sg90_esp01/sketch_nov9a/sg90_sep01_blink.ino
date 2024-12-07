#define BLINKER_WIFI
#define BLINKER_MIOT_OUTLET
#include <Servo.h>
#include <Blinker.h>


char auth[] = "e0fb6e87267e";   // 点灯密匙key
char ssid[] = "makabaka";          //wifi名称
char pswd[] = "123123123";      //wifi密码



// 创建开关
BlinkerButton Button1("btn_k");    //舵机的开
BlinkerButton Button2("btn_g");    //舵机的关


//创建舵机
Servo myservo;//定义（创建）一个舵机名称为myservo





//创建3个滑块
BlinkerSlider Slider1("ran_max1");   //滑块1 控制第一个舵机最大角度
BlinkerSlider Slider2("ran_min1");   //滑块2 控制第一个舵机最小角度
BlinkerSlider Slider3("ran_mid1");   //滑块3 控制第一个舵机中间角度




//定义3个变量
int ran_max1, ran_min1, ran_mid1;
//小爱同学专属两个变量
int xa_mid1=50;
int xa_max1=180;

//创建一个Bool类型的变量
bool on_or_off = false;
bool oState = false;

// app 端按下按键即会执行该函数 回调函数
void button1_callback(const String & state) {
  BLINKER_LOG("get button state: ", state);//读取按键状态

  myservo.write(ran_max1);  //写入滑块1 的角度   这个角度大小可以通过滑块1 来设置
  Blinker.vibrate();       //使手机震动
  Blinker.delay(1000);    //这个delay 一定要有 不然舵机转不过来，1000毫秒延迟
  myservo.write(ran_mid1);  //1000毫秒=1s后恢复到滑块3设置的角度，
  Blinker.vibrate();  //使手机震动

}
void button2_callback(const String & state) {
  BLINKER_LOG("get button state: ", state);//这个和上面的一样
  myservo.write(ran_min1);
  Blinker.vibrate();
  Blinker.delay(1000);
  myservo.write(ran_mid1);
  Blinker.vibrate();
}


//小爱电源回调函数
//里面同样设置舵机的动作
void miotPowerState(const String & state)
{
  BLINKER_LOG("need set power state: ", state);
  if (state == BLINKER_CMD_ON && ran_mid1 == 0 && ran_max1 == 0){  //先判断是否满足最大角度和中间角度为0，满足0度则是esp重启初始化了，默认使用所给的固定变量
    myservo.write(xa_max1);
    Blinker.delay(1000);
    myservo.write(xa_mid1);
    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();
  }
  else if (state == BLINKER_CMD_ON) {  //上面条件不满足就是我在点灯设置了角度，重新判断是否为开，并按照点灯的数据来设置
    // digitalWrite(LED_BUILTIN, HIGH);
    myservo.write(ran_max1);  //写入滑块1 的角度   这个角度大小可以通过滑块1 来设置
    Blinker.delay(1000);    //这个delay 一定要有 不然舵机转不过来
    myservo.write(ran_mid1);  //1s后恢复到滑块3设置的角度，
    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();
  }
  else if (state == BLINKER_CMD_OFF && ran_mid1 == 0 && ran_max1 == 0) {  //判断是否为关并且两个角度是否为0
    myservo.write(ran_min1);
    Blinker.delay(1000);
    myservo.write(xa_mid1);//为0就使用小爱的默认固定中间角度变量
    BlinkerMIOT.powerState("off");
    BlinkerMIOT.print();
  }
  else if (state == BLINKER_CMD_OFF) {
    //  digitalWrite(LED_BUILTIN, LOW);
    myservo.write(ran_min1);
    Blinker.delay(1000);
    myservo.write(ran_mid1);
    BlinkerMIOT.powerState("off");
    BlinkerMIOT.print();
  }
}



void miotQuery(int32_t queryCode)
{
  BLINKER_LOG("MIOT Query codes: ", queryCode);

  switch (queryCode)
  {
    case BLINKER_CMD_QUERY_ALL_NUMBER :
      BLINKER_LOG("MIOT Query All");
      BlinkerMIOT.powerState(oState ? "on" : "off");
      BlinkerMIOT.print();
      break;
    case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
      BLINKER_LOG("MIOT Query Power State");
      BlinkerMIOT.powerState(oState ? "on" : "off");
      BlinkerMIOT.print();
      break;
    default :
      BlinkerMIOT.powerState(oState ? "on" : "off");
      BlinkerMIOT.print();
      break;
  }
}

void dataRead(const String & data)
{
  BLINKER_LOG("Blinker readString: ", data);

  Blinker.vibrate();

  uint32_t BlinkerTime = millis();
  Blinker.print(BlinkerTime);
  Blinker.print("millis", BlinkerTime);
}



//滑块设置
void slider1_callback(int32_t value)   //最大角度滑动条
{
  ran_max1 = value;
  myservo.write(ran_max1);
  Blinker.vibrate();  //使手机震动
  Blinker.delay(1000);
  BLINKER_LOG("get slider value: ", value);
}


void slider2_callback(int32_t value)  //最小角度滑动条
{
  ran_min1 = value;
  myservo.write(ran_min1);
  Blinker.vibrate();  //使手机震动
  Blinker.delay(1000);
  BLINKER_LOG("get slider value: ", value);
}


void slider3_callback(int32_t value)   //中间角度滑动条
{
  ran_mid1 = value;
  myservo.write(ran_mid1);//打印输出滑条角度设置数据
  Blinker.vibrate();  //使手机震动
  Blinker.delay(1000);
  BLINKER_LOG("get slider value: ", value);
}



// 初始化串口，并开启调试信息
void setup() {


  Serial.begin(115200);
  Blinker.begin(auth, ssid, pswd);

  Button1.attach(button1_callback); //绑定按键执行回调函数
  Button2.attach(button2_callback); //绑定按键执行回调函数


  Slider1.attach(slider1_callback);
  Slider2.attach(slider2_callback);
  Slider3.attach(slider3_callback);

  myservo.attach(0); //定义舵机连接的引脚
  pinMode(LED_BUILTIN, OUTPUT);//定义灯引脚


  BlinkerMIOT.attachQuery(miotQuery);   //小爱同学回调函数
  Blinker.attachData(dataRead);
  BlinkerMIOT.attachPowerState(miotPowerState);

}





void loop() {

  Blinker.run();  //*每次运行都会将设备收到的数据进行一次解析。
//  digitalWrite(LED_BUILTIN, HIGH);//不要亮灯把这个删掉
//  digitalWrite(LED_BUILTIN, LOW); //不要亮灯把这个删掉
}



//blink app 界面配置代码复制到app里面即可
//{¨version¨¨2.0.0¨¨config¨{¨headerColor¨¨transparent¨¨headerStyle¨¨dark¨¨background¨{¨img¨¨assets/img/headerbg.jpg¨¨isFull¨«}}¨dashboard¨|{¨type¨¨tex¨¨t0¨¨blinker入门示例¨¨t1¨¨文本2¨¨bg¨Ë¨ico¨´´¨cols¨Í¨rows¨Ê¨key¨¨tex-272¨´x´É´y´É¨speech¨|÷¨lstyle¨Ê¨clr¨¨#FFF¨}{ßC¨btn¨ßJ¨fal fa-power-off¨¨mode¨ÉßE¨关灯¨ßGßHßIÉßKËßLËßM¨btn_g¨´x´Î´y´ÏßPÉ}{ßCßSßJßTßUÉßE¨开灯¨ßGßHßIÉßKËßLËßM¨btn_k¨´x´Ê´y´ÏßPÉ}{ßC¨ran¨ßE¨最大角度¨ßQ¨#389BEE¨¨max¨¢2u¨min¨ÉßIÉßKÑßLËßM¨ran_max1¨´x´É´y´¤DßPÉ}{ßCßZßE¨最小角度¨ßQßbßcº0ßdÉßIÉßKÑßLËßM¨ran_min1¨´x´É´y´ÒßPÉ}{ßCßZßE¨中间角度¨ßQßbßcº0ßdÉßIÉßKÑßLËßM¨ran_mid1¨´x´É´y´¤BßPÉ}{ßC¨deb¨ßUÉßIÉßKÑßLÌßM¨debug¨´x´É´y´Ì}÷¨actions¨|¦¨cmd¨¦¨switch¨‡¨text¨‡¨on¨¨打开?name¨¨off¨¨关闭?name¨—÷¨triggers¨|{¨source¨ßn¨source_zh¨¨开关状态¨¨state¨|ßpßr÷¨state_zh¨|¨打开¨´关闭´÷}÷´rt´|÷}
