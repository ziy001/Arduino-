/**
 * @author ZIY
 * @version 1.0
 * @date:2021/2/2
 */
 
//核心实时状态，改变一次必须调用sync()和ESP做数据同步
bool oState = false;
//角度值
int angle = 0;
//脉宽值
int pulseWidth;
/**
 * 传入一个角度值，控制舵机旋转到指定角度
 */
void write(int pos) {
    //计算脉宽值(单位:微秒)
    pulseWidth = 500 + (11 * pos);
    //一直向舵机发送这种PWM信号，即可完成舵机的转动
    //一次循环是发送一次PWM信号，发送的信号个数自调(发送少：会出现不能旋转的情况)
    for (int i = 0; i < 50; i++) {
        //输出高电位,保持指定脉宽
        digitalWrite(SERVO_PIN, HIGH); 
        delayMicroseconds(pulseWidth);
        //输出低电位
        digitalWrite(SERVO_PIN, LOW);
        delayMicroseconds(20000 - pulseWidth);
    }
    angle = pos;
    Serial.print("角度值: ");Serial.println(angle);
}

bool getState() {return oState;}
void setState(bool flag) {
    oState = flag;
    sync();
}
void servo_init() {
    pinMode(SERVO_PIN, OUTPUT);
    //辅助接近开关一起判断
    //门处于打开状态
    bool f1 = getSQPState();
    delay(500);
    if (getSQPState() && f1) {
        do {
            write(MAX_POS);
        } while (angle < MAX_POS);
        setState(true);
    } else {
        //关闭状态
        do {
            write(MIN_POS);
        } while (angle > MIN_POS);
        setState(false);
    }
}
/**
 * 开锁
 */
void unlock() {
    if (!oState) {
        do {
            write(MAX_POS);
        } while (angle < MAX_POS);
        setState(true);
        Serial.println("已解锁...");
    }
    for (int i = 0; i < AUTO_LOCK_TIME; i++) {
      if (getSQPState()) {
          lock();
          return;
      }
      delay(1000);
    }
    //超时上锁
    lock();
}
/**
 * 上锁
 * 等待接近开关感应后再上锁
 */
void lock() {
    Serial.println("\n等待上锁...");
    while (true) {
        if (!getSQPState()) {
            delay(500);
            if (!getSQPState()) {
                do {
                    write(MIN_POS);
                } while (angle > MIN_POS);
                setState(false);
                Serial.println("已上锁...");
                return;
            }
        }
    }
}
