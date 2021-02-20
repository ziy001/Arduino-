/**
 * 通过点灯科技平台对ESP8255芯片的物联网开发硬件封装进行开发，支持天猫精灵、小米小爱、百度小度平台。
 * <p>1.0：正式版发布，支持物联网平台操控</p>
 * </br>
 * <p>1.1：改进与arduino开发板的通信方式;增加smartConfig配网功能,实现手动配网;减少代码耦合度,改进功能的实现方式;</p>
 * @author ZIY
 * @version 1.1
 * @date:2021/2/2
 */

#define LED LED_BUILTIN
#define BLINKER_PRINT Serial
#define BLINKER_WIFI
//天猫精灵
#define BLINKER_ALIGENIE_SENSOR
//小米小爱
#define BLINKER_MIOT_SENSOR
//百度小度
#define BLINKER_DUEROS_SENSOR

#include <Blinker.h>
#define AUTO_CONN_TIME 30
//状态标识码（用于与arduino进行通信）
const byte ON = 8;
const byte OFF = 7;

const char open_icon[] = "fas fa-door-open";
const char closed_icon[] = "fas fa-door-closed";
const char open_text[] = "已打开";
const char closed_text[] = "已关闭";
const char open_color[] = "#00EE00";
const char closed_color[] = "#FF0000";

//填写在点灯科技申请的密钥
char auth[] = "77f65ef832c0";
//wifi连接状态
bool wifiStatus = false;
//设备状态
bool oState;

//定义按钮
BlinkerButton BUT_1("btn-1");

/**
 * 界面操作和语音操作的共同回调函数
 */
void handle_callback(const String & state) {
    BLINKER_LOG("need set power state: ", state);
    
    if (!oState) {
        BLINKER_LOG(ON);
        ONButUI();
    }
    //反馈按钮成功UI
    //反馈天猫精灵、小米小爱、百度小度的属性
    AIprint("on");
    //设置设备状态
    setOstate(true);
}
/**
 * 自定义设备查询回调函数(设置为只能查询当前设备电源状态)
 */
void aligenieQuery(int32_t queryCode)
{
    BLINKER_LOG("AliGenie Query codes: ", queryCode);
    AIprint(oState ? "on" : "off");
}
/**
 * 如果未绑定的组件被触发，则会执行其中内容
 */
void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);

    Blinker.vibrate();

    uint32_t BlinkerTime = millis();
    Blinker.print(BlinkerTime);
    Blinker.print("millis", BlinkerTime);
}
/**
 * 反馈数据
 * @param state 传入的电源状态
 */
void AIprint(const char* state) {
    //设置天猫精灵属性
    BlinkerAliGenie.powerState(state);
    BlinkerAliGenie.print();
    //小米小爱
    BlinkerMIOT.powerState(state);
    BlinkerMIOT.print();
    //百度小度
    BlinkerDuerOS.powerState(state);
    BlinkerDuerOS.print();
}

void ONButUI() {
    //设置界面的按钮属性
    BUT_1.icon(open_icon);
    BUT_1.color(open_color);
    BUT_1.text(open_text);
    //反馈按钮属性
    BUT_1.print("on");
}

void OFFButUI() {
    //设置界面的按钮属性
    BUT_1.icon(closed_icon);
    BUT_1.color(closed_color);
    BUT_1.text(closed_text);
    //反馈按钮属性
    BUT_1.print("off");
}
/**
 * 设置设备状态，并控制LED灯
 */
void setOstate(bool state) {
    oState = state;
    if (state && digitalRead(LED) == HIGH) {
        digitalWrite(LED, LOW);
    } else {
        digitalWrite(LED, HIGH);
    }
}
/**
 * 用于监听arduino发送的状态
 */
void listen() {
    if (Serial.available()) {
        char data = Serial.read();
        BLINKER_LOG("接收的数据: ");BLINKER_LOG(data);
        //syn信号
        if (data == 0 || data == 1) {
            if (data != oState) {
                Serial.write(oState ? ON : OFF);
            }
            return;
        }
        if (data != (oState ? ON : OFF)) {
            if (data == ON) {
                setOstate(true);
                //反馈按钮
                ONButUI();
            }
            if (data == OFF) {
                setOstate(false);
                OFFButUI();
            }
            return;
        }
    }
}
/**
 * smartConfig技术配网，KED灯闪烁进入smartConfig模式
 */
void smartConfig() {
    //使用WIFI STA模式
    WiFi.mode(WIFI_STA);
    BLINKER_LOG("Wait for SmartConfig...");
    //等待接收wifi信息
    WiFi.beginSmartConfig();
    while(true) {
        if (WiFi.smartConfigDone()) {
            BLINKER_LOG("SmartConfig Success");
            BLINKER_LOG("SSID: ", WiFi.SSID().c_str());
            BLINKER_LOG("PSWD: ", WiFi.psk().c_str());
            break;
        }
        BLINKER_LOG(".");
        delay(500);
        digitalWrite(LED, LOW);
        delay(500);
        digitalWrite(LED, HIGH);
    }
}

/**
 * 自动配网
 * 通过读取flash中存储的历史wifi信息进行连接
 */
bool autoConfig() {
    WiFi.begin();
    BLINKER_LOG("Auto Connection...");
    for (int time = 0; time < AUTO_CONN_TIME; time++) {
        if (WiFi.status() == WL_CONNECTED) {
            wifiStatus = true;
            break;
        }
        BLINKER_LOG("WIFI AutoConfig Waiting...");
        delay(1000);
    }
    BLINKER_LOG("WIFI AutoConfig Result: ", wifiStatus);
    return wifiStatus;
}
void setup()
{
    Serial.begin(38400);
    Serial.setRxBufferSize(0);
    //开启调试模式(如果定义了BLIKER_PRINT常量就打开调试模式)
    //注意:实际使用时，关闭调试功能可以有效的避免ESP01S与arduino之间的通信错误
//    #if defined(BLINKER_PRINT)
//        Serial.begin(9600);
//        BLINKER_DEBUG.stream(BLINKER_PRINT);
//    #endif
    //灯光初始化
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    //自动配网失败,采用smartConfig技术
    if (!autoConfig()) {
        smartConfig();
    }
    Blinker.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str());
    
    BUT_1.attach(handle_callback);
    Blinker.attachData(dataRead);
    
    //天猫精灵绑定
    BlinkerAliGenie.attachPowerState(handle_callback);
    BlinkerAliGenie.attachQuery(aligenieQuery);
    //小米小爱绑定
    BlinkerMIOT.attachPowerState(handle_callback);
    BlinkerMIOT.attachQuery(aligenieQuery);
    //百度小度绑定
    BlinkerDuerOS.attachPowerState(handle_callback);
    BlinkerDuerOS.attachQuery(aligenieQuery);
}

void loop() {
    //监听门锁的最新状态
    listen();
    Blinker.run();
}
