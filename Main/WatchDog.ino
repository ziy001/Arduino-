/**
 * 休眠唤醒
 * @author ZIY
 * @version 1.0
 * @date:2021/2/18
 */

#include <avr/sleep.h>
#include <avr/wdt.h>

byte a;
volatile byte dogCount = 0;

/**
 * 看门狗初始化
 * // 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
// 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
 */
void watchDog_init(byte time, byte ab) {
    a = ab;
    setup_watchdog(time);
    ACSR |=_BV(ACD);//OFF ACD
    ADCSRA=0;//OFF ADC
    Sleep_avr();//Sleep_Mode
}

//Sleep mode is activated
void setup_watchdog(int ii) {
 
  byte bb;
 
  if (ii > 9 ) ii=9;
  bb=ii & 7;
  if (ii > 7) bb|= (1<<5);
  bb|= (1<<WDCE);
  MCUSR &= ~(1<<WDRF);
  // start timed sequence
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  // set new watchdog timeout value
  WDTCSR = bb;
  WDTCSR |= _BV(WDIE);
}
//WDT interrupt
ISR(WDT_vect) {
  ++dogCount;
// wdt_reset();
}
void Sleep_avr(){
  set_sleep_mode(SLEEP_MODE_PWR_DOWN  ); // sleep mode is set here
  sleep_enable();
  sleep_mode();                        // System sleeps here
}

/**
 * 检查看门狗是否超时（休眠结束）
 */
bool checkDog() {
    return dogCount >= a;
}
void setDogCount(byte b) {dogCount = b;}
byte getDogCount() {return dogCount;}
