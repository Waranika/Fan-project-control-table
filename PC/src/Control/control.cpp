/**
 * @file control.cpp
 * @author Bernard Szeliga (b.szeliga@ecam.fr)
 * @brief Control module for the fan table project. Includes simple centering
 * as well as digital PID controller
 * @version 0.1
 * @date 2022-12-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <control.h>
#include <../common_config.h>

#include <opencv2/opencv.hpp>

/** Internal variables **/
static cv::Point zone = cv::Point(0,0);
static PIDController xPID,yPID;

extern cv::Point work_area[2];
extern cv::Point target;

void simple_centering(cv::Point location, uint8_t* speeds){
    cv::Point table_location;
    uint8_t fan_x_speed, fan_y_speed;
    /* Calculate position in table referance frame */
    table_location.x = location.x - work_area[0].x;
    table_location.y = location.y - work_area[0].y;
    /* Calculate active zone */
    calculate_active_zone(location);
    /* Calculate speeds */
    fan_x_speed = (uint8_t)(table_location.x * FAN_MAX_SPEED / (work_area[1].x - work_area[0].x));
    fan_y_speed = (uint8_t)(table_location.y * FAN_MAX_SPEED / (work_area[1].y - work_area[0].y));
    /* Reset fans speeds */
    for(uint8_t i=0; i < NO_FANS; i++){
        speeds[i] = 0;
    }
    /* Set active fans speeds */
    speeds[zone.x] = fan_y_speed;
    speeds[NO_FANS - NO_FANS_Y - zone.x - 1] = FAN_MAX_SPEED - fan_y_speed;
    speeds[NO_FANS_X + zone.y] = fan_x_speed;
    speeds[NO_FANS - zone.y - 1] = FAN_MAX_SPEED - fan_x_speed;
}

void PID_controller(cv::Point location, uint8_t* speeds){
    cv::Point table_loc(location.x - work_area[0].x, location.y - work_area[0].y);
    cv::Point error(target.x - location.x,
                    target.y - location.y);
    
    /* Proportional part */
    xPID.proportional = xPID.kp * (float)error.x;
    yPID.proportional = yPID.kp * (float)error.y;
    /* Integrating part */
    xPID.integrator = xPID.integrator + 0.5f * xPID.ki * xPID.t * (error.x + xPID.prev_error);
    yPID.integrator = yPID.integrator + 0.5f * yPID.ki * yPID.t * (error.x + yPID.prev_error);
    /**
     * Derivative (band-limited differentiator)
     * 
     * Note: derivative on measurement, therefore minus sign in front of equation
     * Derivative on measurement prevents kick when set-point is changed
     */
    xPID.derivative = -(2.0f * xPID.kd * (table_loc.x - xPID.prev_measurement));
                        // + (2.0f * pid.tau - pid.t) * pid.derivative[0])
                        // / (2.0f * pid.tau + pid.t);
    yPID.derivative = -(2.0f * yPID.kd * (table_loc.y - yPID.prev_measurement));
                        // + (2.0f * pid.tau - pid.t) * pid.derivative[1])
                        // / (2.0f * pid.tau + pid.t);
    /* Output */
    xPID.out = xPID.proportional + xPID.integrator + xPID.derivative;
    yPID.out = yPID.proportional + yPID.integrator + yPID.derivative;
    /* Saturation */
    if (xPID.out > FAN_MAX_SPEED / 2)
        xPID.out = FAN_MAX_SPEED / 2;
    else if (xPID.out < -FAN_MAX_SPEED / 2)
        xPID.out = - FAN_MAX_SPEED / 2;
    if (yPID.out > FAN_MAX_SPEED / 2)
        yPID.out = FAN_MAX_SPEED / 2;
    else if (yPID.out < - FAN_MAX_SPEED / 2)
        yPID.out = - FAN_MAX_SPEED / 2;
    /* Mapping to unsigned */
    xPID.out += FAN_MAX_SPEED / 2;
    yPID.out += FAN_MAX_SPEED / 2;    
    /* Update buffers */
    xPID.prev_measurement = (float)table_loc.x;
    yPID.prev_measurement = (float)table_loc.y;
    xPID.prev_error = (float)error.x;
    yPID.prev_error = (float)error.y;
    /* Update active zone */
    calculate_active_zone(location);
    /* Reset fans speeds */
    for(uint8_t i=0; i < NO_FANS; i++){
        speeds[i] = 0;
    }
    /* Set active fans speeds */
    speeds[zone.x] = (uint8_t)yPID.out;
    speeds[NO_FANS - NO_FANS_Y - zone.x - 1] = FAN_MAX_SPEED - (uint8_t)yPID.out;
    speeds[NO_FANS_X + zone.y] = (uint8_t)xPID.out;
    speeds[NO_FANS - zone.y - 1] = FAN_MAX_SPEED - (uint8_t)xPID.out;
    
}

void calculate_active_zone(cv::Point location){
    cv::Point table_location;
    int fan_x_width, fan_y_width;
    /* Calculate position in table referance frame */
    table_location.x = location.x - work_area[0].x;
    table_location.y = location.y - work_area[0].y;
    /* Calculate fan widths */
    fan_x_width = (work_area[1].x - work_area[0].x) / NO_FANS_X;
    fan_y_width = (work_area[1].y - work_area[0].y) / NO_FANS_Y;
    /* Calculate active zone */
    zone.x = (uint8_t)(table_location.x / fan_x_width);
    zone.y = (uint8_t)(table_location.y / fan_y_width);
}
