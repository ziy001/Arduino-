/**
 * 实现对指纹模组的操作
 * <p>1.0：正式版，提供指纹的识别功能</p>
 * <br/>
 * <p>1.1：优化代码架构;实现指纹模块的智能启动,大幅降低耗电量;</p>
 * @author ZIY
 * @version 1.1
 * @date:2021/2/2
 */
 
const SoftwareSerial fSerial(TX_PIN, RX_PIN);
const Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fSerial);

short fid = -3;

void f_init() {
    pinMode(HAVE, INPUT);
    pinMode(POWER, OUTPUT);
    //指纹模块供电，完成初始化
    digitalWrite(POWER, HIGH);
    // 设置传感器串行端口的数据速率
    finger.begin(57600);
    delay(5);
    if (!finger.verifyPassword()) {
        Serial.println("Fingerprint sensor not found");
    }
  //关闭电源
  digitalWrite(POWER, LOW);
}

/**
 * run fingerver
 */
void run_fingerver() {
    //检查当前是否有指纹
    if (digitalRead(HAVE) == 0) {
        return;
    }
    //供电
    digitalWrite(POWER, HIGH);

    //进行验证
    while(true) {
        fid = f_verify();
        switch (fid) {
            case -1:
                //一般由于设备初始化未完成导致，但是可能因为本来没有使用指纹解锁，模块上本来就没得指纹无法获取指纹，所以就导致无端进入这个死循环
                Serial.println("\n\rFingerprint error");
                if (digitalRead(HAVE) == 0) {
                    digitalWrite(POWER, LOW);
                    return;
                }
//                delay(100);
                break;
            case -2:
                Serial.print("\n\rFingerprint matching failed: ");Serial.println(fid);
                digitalWrite(POWER, LOW);
                fail();
                goto end;
            default:
                Serial.print("\n\rFingerprint matching succeeded: ");Serial.println(fid);
                digitalWrite(POWER, LOW);
                success();
                goto end;
        }
    }
    end: return;
//    Serial.println("\n\r======关闭电源======");
}

//指纹验证,成功即返回指纹ID，返回-1表示：当前指纹识别错误。返回-2表示：当前指纹匹配错误
int f_verify() {
    fSerial.listen();
    //验证传感器
    while(!finger.verifyPassword());
    uint8_t p = finger.getImage();
    if (p != FINGERPRINT_OK)  return -1;
  
    p = finger.image2Tz();
    if (p != FINGERPRINT_OK)  return -1;
    
  //  p = finger.fingerSearch();
    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK)  return -2;
    fSerial.end();
    return finger.fingerID;
}
