/**
 * 实现与ESP物联网模组的通信
 * <p>1.0：正式版，实现ESP与本机(arduino uno)通信</p>
 * </br>
 * <p>1.1：优化与ESP通信机制，大幅降低资源的损耗</p>
 * 
 * @author ZIY
 * @version 1.1
 * @date:2021/2/2
 */
 
// rx tx
SoftwareSerial softSerial(4,5);
const byte ON = 8;
const byte OFF = 7;

byte data;

void iot_init() {
    softSerial.begin(38400);
}
/**
 * 该函数用于与ESP01S做数据同步
 * 发送最新数据到ESP01S
 */
void sync() {
    Serial.print("sync: ");Serial.println(getState() ? ON : OFF);
    softSerial.write(getState() ? ON : OFF);
}

/**
 * 监听ESP的状态修改，读取数据
 * ArduinoUNO板，因为同一时刻只能监听一个软串口，导致可能ESP发送数据，但是因为当前软串口没有监听数据导致丢失。
 * 解决方案:由ArduinoUNO主动询问更新：UNO发送SYN信号，ESP收到SYN信号将保存的最新状态发送到UNO板中
 */
void esp_listen() {
    //开始监听软串口RX端的数据（同一时刻软串口只能监听一个）
    softSerial.listen();
    //发送当前状态
    softSerial.write(getState());
    delay(5);
    if (softSerial.available()) {
        data = softSerial.read();
//        Serial.print("\n接收的数据: ");Serial.println(data);
        switch (data) {
            case ON:
                if (getState() == false) {
                    Serial.println("\nSuccessfully unlocked by ESP");
                    success();
                }
                break;
            case OFF:
                break;
        }
    }
    softSerial.end();
    
    while (softSerial.isListening()) {
        softSerial.end();
    }
}
