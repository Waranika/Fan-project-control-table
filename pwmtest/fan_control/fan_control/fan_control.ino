#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void set_fan_speed(int* speeds) {
  // Drive each PWM in a 'wave'
  
  //speeds = parser("056,255,008,158,102,130,123,003,132,255,97,123,042,069,166,058")
  for(int m=0; m<16; m++) 
  {
    uint32_t speed = speeds[m] ;
  
    uint32_t off = 4096 * speed / 255;
    pwm.setPWM( m, 4096, off);
    // Serial.print("i= ");
    // Serial.println(i);  
    // Serial.print("off=");
    // Serial.println(off);
    // Serial.print("m=");
    // Serial.println(m)

    
#ifdef ESP8266
    yield();  // take a breather, required for ESP8266
#endif
  }
}
