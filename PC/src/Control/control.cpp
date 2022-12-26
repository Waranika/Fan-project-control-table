#include <control.h>
#include <../common_config.h>

#include <opencv2/opencv.hpp>

/** Internal variables **/
static cv::Point zone = cv::Point(0,0);
static PIDController pid;

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
    cv::Point error(target.x - location.x, target.y - location.y);
    
    /* Proportional part */
    pid.proportional[0] = pid.kp * (float)error.x;
    pid.proportional[1] = pid.kp * (float)error.y;
    /* Integrating part */
    pid.integrator[0] = pid.integrator[0] + 0.5f * pid.ki * pid.t * (error.x + pid.prev_error[0]);
    pid.integrator[1] = pid.integrator[1] + 0.5f * pid.ki * pid.t * (error.x + pid.prev_error[1]);
    /**
     * Derivative (band-limited differentiator)
     * 
     * Note: derivative on measurement, therefore minus sign in front of equation
     * Derivative on measurement prevents kick when set-point is changed
     */
    pid.derivative[0] = -(2.0f * pid.kd * (location.x - pid.prev_measurement[0])
                        + (2.0f * pid.tau - pid.t) * pid.derivative[0])
                        / (2.0f * pid.tau + pid.t);
    pid.derivative[1] = -(2.0f * pid.kd * (location.y - pid.prev_measurement[1])
                        + (2.0f * pid.tau - pid.t) * pid.derivative[1])
                        / (2.0f * pid.tau + pid.t);
    /* Output */
    pid.out[0] = pid.proportional[0] + pid.integrator[0] + pid.derivative[0];
    pid.out[1] = pid.proportional[1] + pid.integrator[1] + pid.derivative[1];
    /* Saturation */
    if (pid.out[0] > FAN_MAX_SPEED)
        pid.out[0] = FAN_MAX_SPEED;
    else if (pid.out[0] < 0)
        pid.out[0] = 0;
    if (pid.out[1] > FAN_MAX_SPEED)
        pid.out[1] = FAN_MAX_SPEED;
    else if (pid.out[1] < 0)
        pid.out[1] = 0;
    /* Update buffers */
    pid.prev_measurement[0] = location.x;
    pid.prev_measurement[1] = location.y;
    pid.prev_error[0] = error.x;
    pid.prev_error[1] = error.y;
    /* Update active zone */
    calculate_active_zone(location);
    /* Reset fans speeds */
    for(uint8_t i=0; i < NO_FANS; i++){
        speeds[i] = 0;
    }
    /* Set active fans speeds */
    speeds[zone.x] = pid.out[1];
    speeds[NO_FANS - NO_FANS_Y - zone.x - 1] = FAN_MAX_SPEED - pid.out[1];
    speeds[NO_FANS_X + zone.y] = pid.out[0];
    speeds[NO_FANS - zone.y - 1] = FAN_MAX_SPEED - pid.out[0];
    
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
