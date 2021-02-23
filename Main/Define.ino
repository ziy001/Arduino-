/**
 * 对一些变量进行集中定义管理
 * <p>1.0：正式版</p>
 * @author ZIY
 * @version 1.0
 * @date:2021/2/2
 */
#include <avr/sleep.h>
#include <avr/wdt.h>
#include<SoftwareSerial.h>
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
#define CARD_COUNT 1
const byte nuidPICC[][DATA_COUNT] = {
  {57, 58, 10, 179}
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
