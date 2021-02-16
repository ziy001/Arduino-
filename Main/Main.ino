/**
 * @author ZIY
 * @version 1.0
 * @date:2021/2/2
 */

/**
 * 全局匹配成功的操作
 */
void success() {
  remind_success();
  unlock();
}

/**
 * 全局匹配失败的操作
 */
void fail() {
  remind_fail();
}

void defaultInit() {
    //初始化Serial的波特率
    Serial.begin(9600);
    delay(100);
    //RC522射频卡初始化
    rc522_init();
    delay(100);
    //remind提醒模块初始化
    remind_init();
    //按钮模块初始化
    but_init();
    delay(10);
    //指纹模块初始化
    f_init();
    delay(50);
    //接近开关初始化
    SQP_init();
    //ESP模块的初始化
    iot_init();
    //舵机初始化，必须在最后初始化
    servo_init();
}
/**
 * 主处理程序
 */
void defaultRun() {
    //调用esp监听
    esp_listen();
    //调用按钮监听
    but_listen();
    //调用AS608指纹模块
    run_fingerver();
    //调用RC522模块
    run_rc522();
}

//主程序的设置
void setup() {
    //各组件初始化
    defaultInit();
    delay(10);
    //初始化成功提醒
    remind(1200);
    Serial.println("==========程序初始化完毕==========");
    //锁的初始形态修正
    while (getState()) {
        lock();
    }
}

void loop() {
    defaultRun();
    delay(250);
}
