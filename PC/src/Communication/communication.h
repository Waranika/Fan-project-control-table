/**
 * @file communication.h
 * @author Bernard Szeliga
 * @brief Communication module for fan table project
 * @version 0.1
 * @date 2023-01-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include "stdint.h"

/**
 * @brief Function checking if port to arduino is open
 * 
 * @return true Port is open
 * @return false Port is not open
 */
bool check_port(void);

/**
 * @brief Function converts array of fan speeds into string
 * and sends it to arduino
 * 
 * @param speeds Array of fan speeds normalized to range [0,255]
 * @return true String successfully sent
 * @return false transfer failed
 */
bool send_fan_speed(uint8_t* speeds);

/**
 * @brief Function attempting to reope the UART port
 * 
 */
void reopen_port(void);

/**
 * @brief Function sending provided speed and displying state of communication
 * 
 * @param speeds Array of fan speeds normalized to range [0,255]
 */
void send_to_table(uint8_t* speeds);
#endif