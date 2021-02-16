/**
 * @author ZIY
 * @version 1.0
 * @date:2021/2/2
 */

/**
 * 初始化函数
 */
void remind_init() {
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(HG_PIN, OUTPUT);
    pinMode(HR_PIN, OUTPUT);
}

/**
 * 自定义规则的提醒(保留)
 * @param
 * @param
 * @param
 */
void customRemind(int greenLEDTime, int redLEDTime, int buzzerTime) {
    
}
 
/**
 * 成功提醒
 * 蜂鸣器 和 灯  1秒
 * 
 */
void remind_success() {
    digitalWrite(HG_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(500);
    digitalWrite(HG_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
}

/**
 * 失败提醒
 * 蜂鸣器 和 灯 2秒
 */
void remind_fail() {
    digitalWrite(HR_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000);
    digitalWrite(HR_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
}
/**
 * 程序初始化完毕提醒
 */
void remind(int time) {
    digitalWrite(HG_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(HR_PIN, HIGH);
    delay(time);
    digitalWrite(HG_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(HR_PIN, LOW);
}
