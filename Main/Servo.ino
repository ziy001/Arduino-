/**
 * @author ZIY
 * @version 1.0
 * @date:2021/2/2
 */
 
/**
 * 核心状态变量
 * 规范：每次改变必须和ESP做一次同步通信
 * false:上锁
 * true:解锁
 */
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
    Serial.print("pos: ");Serial.println(angle);
}

bool getState() {return oState;}
void setState(bool flag) {
    oState = flag;
    sync();
}
void servo_init() {
    pinMode(SERVO_PIN, OUTPUT);
    //调整门锁状态
    if (getSQPState()) {
        privateLock();
    } else {
        privateUnLock();
    }
}
/**
 * 开锁
 */
void unlock() {
    if (!oState) {
        privateUnLock();
        Serial.println("Unlocked...");
    }
    for (byte i = 0; i < AUTO_LOCK_TIME && getSQPState(); i++) {
        delay(1000);
    }
    //lock
    lock();
}
/**
 * 上锁
 * 等待接近开关感应后再上锁
 */
void lock() {
    Serial.println("\n\rWaiting for the lock...");
    while (true) {
        if (getSQPState()) {
            delay(500);
            if (getSQPState()) {
                privateLock();
                Serial.println("locked..");
                return;
            }
        }
    }
}
void privateLock() {
    do {
        write(MIN_POS);
    } while (angle > MIN_POS);
    setState(false);
}
void privateUnLock() {
    do {
        write(MAX_POS);
    } while (angle < MAX_POS);
    setState(true);
}
