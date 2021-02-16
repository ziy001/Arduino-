/**
 * 对一些变量进行集中定义管理
 * <p>1.0：正式版</p>
 * @author ZIY
 * @version 1.0
 * @date:2021/2/2
 */

#include <Adafruit_Fingerprint.h>
#include <SPI.h>
#include <MFRC522.h>


/**
 * rc522 define
 */
//定义针脚号
#define SS_PIN 10
#define RST_PIN 9
#define DATA_COUNT 4
//白名单卡数量,必须需nuidPICC二维数组的行数保持一致
#define CARD_COUNT 2
//存储卡信息,数量与CARD_COUNT相等
const byte nuidPICC[CARD_COUNT][DATA_COUNT] = {
  {57, 58, 10, 179}, 
  {10, 149, 226, 63}
};

/**
 * servo define
 */
 //定义舵机的最大旋转角度
//打开
#define MAX_POS 180
//关闭
#define MIN_POS 0
#define SERVO_PIN 6
#define AUTO_LOCK_TIME 5

/**
 * remind define
 */
//绿灯
#define HG_PIN A0
//红灯
#define HR_PIN A1
//蜂鸣器针脚号
#define BUZZER_PIN A2

/**
 * sqp define
 */
#define SQP_PIN A4
 
/**
 * fingerver define
 */
#define TX_PIN 2
#define RX_PIN 3
#define HAVE 8
#define POWER 7
