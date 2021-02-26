/*
 * @author ZIY
 * @version 1.0
 * @date:2021/2/2
 */
#define NUID_SIZE 4
const MFRC522 rfid(SS_PIN, RST_PIN);
const byte newUid[NUID_SIZE] = {0xDE, 0xAD, 0xBE, 0xEF};
const byte nuid[NUID_SIZE] = {57, 58, 10, 179};
MFRC522::MIFARE_Key key;

void rc522_init() {
    //MFRC522初始化
    SPI.begin(); // Init SPI bus
    rfid.PCD_Init(); // Init MFRC522
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
}

 
/**
 * run rc522
 */
void run_rc522() {
    if (!rc522_check()) {
        return;
    }
    if (rc522_verify()) {
        //匹配成功
        Serial.println("\n\rSuccessfully unlocked by card");
        success();
    } else {
        //匹配错误
        Serial.println("\n\rFailed to unlock by card");
        fail();
    }
    rc522_halt();
}

/**
 * RC522对卡信息进行验证
 * 返回是否验证成功
 */
bool rc522_verify() {
    //尝试对0扇区进行更改
    if (rfid.MIFARE_UnbrickUidSector(false)) {
        return false;
    }
    //检查uid
    for (byte i = 0; i < NUID_SIZE; i++) {
        if (rfid.uid.uidByte[i] != nuid[i]) {
            return false;
        }
    }
    return true;
}

/**
 * 对卡进行监听、检查、读取
 * 
 */
bool rc522_check() {
  //当没有卡时，返回false
  if ( ! rfid.PICC_IsNewCardPresent()) {
    return false;
  }
  // 验证是否已读取NUID
  // 如果没有成功读取NUID，返回false
  if ( ! rfid.PICC_ReadCardSerial()) {
    return false;
  }
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    return false;
  }
  return true;
}
/**
 * halt PICC
 */
void rc522_halt() {
    // 指示处于激活状态的PICC进入停止状态。
    rfid.PICC_HaltA();
    // 用于将PCD从其已验证状态退出。
    //记住在与经过身份验证的PICC通信后调用此函数-否则无法启动新的通信。
    rfid.PCD_StopCrypto1();
}
