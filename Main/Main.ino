/**
 * arduino主程序
 * <p>1.0：正式版</p>
 * <br/>
 * <p>1.1：增加WatchDog休眠模式,大大提高了整个应用的耗电量</p>
 * @author ZIY
 * @version 1.1
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
    while (!Serial);  // For Yun/Leo/Micro/Zero/...  
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
    //休眠时间:arg0 * arg1
    // 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
    // 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
    watchDog_init(4, 1);
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
    Serial.println("=========Program initialization completed==========");
    //锁的初始形态修正
    while (getState()) {
        lock();
    }
}

void loop() {
    if(checkDog()){
//        Serial.println("run...");
        defaultRun();
        setDogCount(0);
    }
    Sleep_avr();
}
