#include <string.h>
#include <stdio.h>
#include <stdlib.h>  
#include <limits.h>

#include <sys/types.h> 
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int* speeds;    
char* values;
void setup() {
    Serial.begin(115200);
    pwm.begin();
    pwm.setOscillatorFrequency(27000000);
    pwm.setPWMFreq(1600);
    Wire.setClock(400000);
}

void loop() {
    if (Serial.available() > 0) {
        // read the incoming byte:
        values = Serial.read();
    }
    
    speeds = parser(values);
    set_fan_speed(speeds);
}


void set_fan_speed(int* speeds) {
    for(int m=0; m<16; m++) 
    {
        uint32_t speed = speeds[m] ;
        uint32_t off = 4096 * speed / 255;
        pwm.setPWM( m, 4096, off);
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