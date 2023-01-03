#ifndef CONTROL_H
#define CONTROL_H

#include <opencv2/opencv.hpp>
#include "../common_config.h"

/**
 * @brief Structure storing PID controller parameters as well as 
 * 
 */
typedef struct {
    /* Parameters */
    float kp = PID_KP;
    float ki = PID_KI;
    float kd = PID_KD;
    float tau = PID_TAU;
    float t = (float)SAMPLING_TIME_US / 1000000;
    /* Output buffers */
    float proportional = 0;
    float integrator = 0;
    float derivative = 0;
    float out = 0;
    /*Input buffers*/
    float prev_error = 0;
    float prev_measurement = 0;
}PIDController;

/**
 * @brief Calculates the speeds of the fans by proportion of ball current 
 * location to a coresponding dimension of the table
 * 
 * @param location cv::Point, Current location of the ball
 * @param speeds uint8_t 1D array of speeds of 16 fans
 */
void simple_centering(cv::Point location, uint8_t* speeds);

/**
 * @brief Calculates zone in witch ball is placed
 * 
 * @param location location of the ball in camera point of refarence
 */
void calculate_active_zone(cv::Point location);

void PID_controller(cv::Point location, uint8_t* speeds);

#endif