/** Includes **/
/* External */
#include "../../external/simple-serial-port/SimpleSerial.h"
/* Internal */
#include "../common_config.h"

/* Global variables */
static SimpleSerial port(COM_PORT, BOUD_RATE);

bool check_port(void){
    return port.connected_;
}

bool send_fan_speed(uint8_t* speeds){
    char message[60];
    sprintf_s(message,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
    speeds[0], speeds[1], speeds[2], speeds[3], speeds[4], speeds[5], speeds[6], speeds[7],
    speeds[8], speeds[9], speeds[10], speeds[11], speeds[12], speeds[13], speeds[14], speeds[15]);
    bool sent = port.WriteSerialPort(message);
    return sent;
}

void reopen_port(void){
    port.CloseSerialPort();
    port = SimpleSerial(COM_PORT, BOUD_RATE);
}

void send_to_table(uint8_t* speeds){
    if (check_port()){
        if (send_fan_speed(speeds))
            printf_s("DATA SENT\n");
        else
            printf_s("ERROR OCCURRED WHILE SENDING\n");
    }else{
        printf_s("PORT NOT OPPENED\nATTEMPTING TO RECONNECT\n");
        reopen_port();
        if (check_port())
            printf_s("PORT OPENED\n");
        else
            printf_s("REOPENING FAILED\n");
    }
}