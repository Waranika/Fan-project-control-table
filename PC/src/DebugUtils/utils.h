/**
 * @file utils.h
 * @author Bernard Szeliga (b.szeliga@ecam.fr)
 * @brief Testing utilities for the fan table project. Does not provide any functionality to the project
 * @version 0.1
 * @date 2023-01-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef UTILS_H
#define UTILS_H

#include <chrono>
/**
 * @brief Function calcuilating tiome from its last call or update of prev timestamp
 * 
 * @param prev Timestamp form witch elapsed time should be calculated
 * @param proces_name Name of process, used for diplying
 */
void exec_time(std::chrono::high_resolution_clock::time_point* prev, char* proces_name);

/**
 * @brief Function displying fan speeds in console in such way that the placement
 * of the number corresponds to physical location of the fan on table
 * 
 * @param speeds Aray of the fan speeds normalized to range [0,255]
 */
void print_speeds(uint8_t* speeds);

#endif