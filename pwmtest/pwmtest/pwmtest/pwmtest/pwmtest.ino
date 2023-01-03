#include <string.h>
#include <stdio.h>
#include <stdlib.h>  
#include <limits.h>

#include <sys/types.h> 
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);
char* values = 0;
void setup() {
  Serial.begin(9600);
  Serial.println("16 channel PWM test!");

  pwm.begin();
  /*
   * In theory the internal oscillator (clock) is 25MHz but it really isn't
   * that precise. You can 'calibrate' this by tweaking this number until
   * you get the PWM update frequency you're expecting!
   * The int.osc. for the PCA9685 chip is a range between about 23-27MHz and
   * is used for calculating things like writeMicroseconds()
   * Analog servos run at ~50 Hz updates, It is importaint to use an
   * oscilloscope in setting the int.osc frequency for the I2C PCA9685 chip.
   * 1) Attach the oscilloscope to one of the PWM signal pins and ground on
   *    the I2C PCA9685 chip you are setting the value for.
   * 2) Adjust setOscillatorFrequency() until the PWM update frequency is the
   *    expected value (50Hz for most ESCs)
   * Setting the value here is specific to each individual I2C PCA9685 chip and
   * affects the calculations for the PWM update frequency. 
   * Failure to correctly set the int.osc value will cause unexpected PWM results
   */
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(1600);  // This is the maximum PWM frequency

  // if you want to really speed stuff up, you can go into 'fast 400khz I2C' mode
  // some i2c devices dont like this so much so if you're sharing the bus, watch
  // out for this!
  Wire.setClock(400000);
}

void loop() {
  int* speeds;
  if (Serial.available() > 0) {
    // read the incoming byte:
    values = Serial.read();
  }
  
  speeds = parser(values);
  set_fan_speed(speeds);
}

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

int* parser(char* x){
  
  char line[100];
  strcpy(line,x);
    
  char *separator = ",";
  char str[16][3];
	int i = 0;
	
	
	char *token = strtok(line, separator);
	do 
	{
		strcpy(str[i++], token);
	}while ((i < 16) && ((token = strtok(NULL, separator)) != NULL));
	
    int y = 0;
    int speed[16];
    char val[3];
    for(y=0; y<=15; y++)
    {
        memcpy(val, str+y, 3);
        speed[y]=atoi(val);
    }
}
