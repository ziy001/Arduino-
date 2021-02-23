/**
 * @author ZIY
 * @version 1.0
 * @date:2021/2/2
 */

const MFRC522 rfid(SS_PIN, RST_PIN);

void rc522_init() {
  //MFRC522初始化
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  
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
        Serial.println("\nSuccessfully unlocked by card");
        success();
    } else {
        //匹配错误
        Serial.println("\nFailed to unlock by card");
        fail();
    }
    rc522_halt();
}

/**
 * RC522对卡信息进行验证
 * 返回是否验证成功
 */
bool rc522_verify() {
  //取出卡信息进行对比
  for (byte i = 0; i < CARD_COUNT; i++) {
    int count = DATA_COUNT - 1;
    for (; count >= 0; count--) {
      if (nuidPICC[i][count] != rfid.uid.uidByte[count]) {
        //不匹配
        break;
      }
    }
    if (count < 0) {
      return true;
      break;
    }
  }
  return false;
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
    // 休眠 halt
    rfid.PICC_HaltA();
    // Stop encryption on PCD
    rfid.PCD_StopCrypto1();
}
