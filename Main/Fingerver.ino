/**
 * 实现对指纹模组的操作
 * <p>1.0：正式版，提供指纹的识别功能</p>
 * <br/>
 * <p>1.1：优化代码架构;实现指纹模块的智能启动,大幅降低耗电量;</p>
 * @author ZIY
 * @version 1.1
 * @date:2021/2/2
 */
 
#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)

SoftwareSerial fSerial(TX_PIN, RX_PIN);

#else

#define fSerial Serial1

#endif


const Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fSerial);

int fid = -3;

void f_init() {
    pinMode(HAVE, INPUT);
    pinMode(POWER, OUTPUT);
    //指纹模块供电，完成初始化
    digitalWrite(POWER, HIGH);
    while (!Serial);  // For Yun/Leo/Micro/Zero/...  
    // 设置传感器串行端口的数据速率
    finger.begin(57600);
    delay(5);
    if (finger.verifyPassword()) {
      Serial.println("找到指纹传感器了！");
    } else {
      Serial.println("找不到指纹传感器 :(");
      while (1) { delay(1); }
    }
  Serial.println(F("指纹模块参数"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  //波特率
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);
  //获取存储的指纹个数
  finger.getTemplateCount();

  Serial.print("指纹库中有 "); Serial.print(finger.templateCount); Serial.println(" 个指纹数据");
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
                Serial.println("\n指纹无法识别!!!\n1、请检查是否有手指按压\n2、请检查设备是否正常运行");
                if (digitalRead(HAVE) == 0) {
                    digitalWrite(POWER, LOW);
                    return;
                }
                delay(100);
                break;
            case -2:
                Serial.print("\n指纹匹配失败: ");Serial.println(fid);
                digitalWrite(POWER, LOW);
                fail();
                goto end;
            default:
                Serial.print("\n指纹匹配成功: ");Serial.println(fid);
                digitalWrite(POWER, LOW);
                success();
                goto end;
        }
    }
    end: return;
//    Serial.println("\n======关闭电源======");
}

//指纹验证,成功即返回指纹ID，返回-1表示：当前指纹识别错误。返回-2表示：当前指纹匹配错误
int f_verify() {
    fSerial.listen();
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
