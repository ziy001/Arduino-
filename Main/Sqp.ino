/**
 * @author ZIY
 * @version 1.0
 * @date:2021/2/2
 * @Deprecated
 */

/**
 * 接近开关
 * 原则：不能让接近开关决定是否开锁
 */

void SQP_init() {
    pinMode(SQP_PIN, INPUT);
}

/**
 * 查询并返回SQP当前状态
 * 0(false):感应成功，门处于关闭状态
 * 1(true):未感应，门处于打开状态
 */
bool getSQPState() {
    return digitalRead(SQP_PIN);
}
