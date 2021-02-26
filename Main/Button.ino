/**
 * @author ZIY
 * @version 1.0
 * @date:2021/2/2
 */
 
/**
 * button define
 */
#define BUT_PIN A3
//长按时间(ms)
#define PRESS 250

void but_init() {
    pinMode(BUT_PIN, INPUT);
}
void but_listen() {
    if (digitalRead(BUT_PIN) == 1) {
        delay(PRESS);
        if (digitalRead(BUT_PIN) == 1) {
            Serial.println("\n\rSuccessfully unlocked by button");
            //解锁
            success();
            //等待长按放开
            while (digitalRead(BUT_PIN) == 1);
        }
    }
}
