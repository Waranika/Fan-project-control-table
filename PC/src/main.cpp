#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "config.h"
#include "../external/simple-serial-port/SimpleSerial.h"
#include <string.h>

/**
 * @brief Get the ball location object
 * Locates the ball of color defined by threshold range in config.h
 *
 * @param frame cv::Mat object contajning captured frame
 * @return cv::Point location of a ball
 */
cv::Point get_ball_location(cv::Mat frame)
{
    // Convert colorspace to HSV for easier detection
    cv::Mat hsvFrame;
    cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);
    // Apply gausian blure for reducing impuls noise
    blur(hsvFrame, hsvFrame, cv::Size(1, 1));
    // Threshold the image
    cv::Scalar lowerBound = cv::Scalar(H_MIN, S_MIN, V_MIN);
    cv::Scalar upperBound = cv::Scalar(H_MAX, S_MAX, V_MAX);
    cv::Mat threshFrame;
    inRange(hsvFrame, lowerBound, upperBound, threshFrame);
    imshow("threshold",threshFrame);
    // Calculate the centroid
    cv::Moments m = moments(threshFrame, false);
    cv::Point com(m.m10 / m.m00, m.m01 / m.m00);
    return com;
}

/**
 * @brief Calculates speed of fans based on position of the ball with target location equal to the middle of the table
 * 
 * @param location cv::Point, Current location of the ball
 * @param speeds uint8_t 1D array of speeds of 16 fans
 */
void calculate_fan_speeds(cv::Point location, uint8_t* speeds){
    cv::Point table_location;
    uint8_t fan_x, fan_y, fan_x_speed, fan_y_speed;
    /* Calculate position in table referance frame */
    table_location.x = location.x - TABLE_WIDTH_MIN;
    table_location.y = location.y - TABLE_HEIGHT_MIN;
    /* Calculate active zone */
    fan_x = (uint8_t)(table_location.x / FAN_WIDTH);
    fan_y = (uint8_t)(table_location.y / FAN_WIDTH);
    /* Calculate speeds */
    fan_x_speed = (uint8_t)(table_location.x * FAN_MAX_SPEED / (TABLE_WIDTH_MAX - TABLE_WIDTH_MIN));
    fan_y_speed = (uint8_t)(table_location.y * FAN_MAX_SPEED / (TABLE_HEIGHT_MAX - TABLE_HEIGHT_MIN));
    /* Reset fans speeds */
    for(uint8_t i=0; i<NO_FANS; i++){
        speeds[i] = 0;
    }
    /* Set active fans speeds */
    speeds[fan_x] = fan_y_speed;
    speeds[NO_FANS - NO_FANS_Y - fan_x - 1] = FAN_MAX_SPEED - fan_y_speed;
    speeds[NO_FANS_X + fan_y] = fan_x_speed;
    speeds[NO_FANS - fan_y - 1] = FAN_MAX_SPEED - fan_x_speed;
}


int main()
{
    // Create camera capture
    cv::Mat frame;           
    cv::VideoCapture cap(1); // 0 for built in camera, 1 for webca-,i

    /* Configuration of serial port */
    // SimpleSerial Serial(COM_PORT, BOUD_RATE);
    char read_in[75];
    // char *to_send;
    // if(!connected_)
    // {
    //     std::cout << "NO ARDUINO DETECTED, ABORTING" << std::endl;
    //     system("pause");
    //     return -2;        
    // }

    // Exit code if no camera is detected
    if (!cap.isOpened())
    {
        std::cout << "NO VIDEO STREAM DETECTED, ABORTING" << std::endl;
        system("pause");
        return -1;
    }
    int x;
    int y;
    uint8_t speeds[16];
    while (true)
    {
        // Transfer and resize captured frame for procesing
        cap >> frame;

        // Get the location of ball
        cv::Point loc = get_ball_location(frame);
        // Draw marker on received location
        cv::Scalar color = cv::Scalar(0, 0, 255);
        drawMarker(frame, loc, color, cv::MARKER_CROSS, 50, 5);

        cv::Point pt1, pt2;
        pt1.x = TABLE_WIDTH_MIN;
        pt1.y = TABLE_HEIGHT_MIN;
        pt2.x = TABLE_WIDTH_MAX;
        pt2.y = TABLE_HEIGHT_MAX;

        rectangle(frame, pt2, pt1, cv::Scalar(0, 255, 0), 2, 8, 0);
        // Display frame with marker on ball
        imshow("Ball", frame);
        x = loc.x;
        y = loc.y;
        calculate_fan_speeds(loc,speeds);
        /* Visualization of fan sppeds */
        printf_s("Ball location \tx:\t%d\ty:\t%d\n", x, y);
        printf_s("\t%d\t%d\t%d\t%d\t%d\n%d\t\t\t\t\t\t%d\n%d\t\t\t\t\t\t%d\n%d\t\t\t\t\t\t%d\n\t%d\t%d\t%d\t%d\t%d\n",
        speeds[0], speeds[1], speeds[2], speeds[3], speeds[4], speeds[15], speeds[5], speeds[14], speeds[6],
        speeds[13], speeds[7], speeds[12], speeds[11], speeds[10], speeds[9], speeds[8]);

        /* Sending the fans speeds to Arduino */
        sprintf_s(read_in,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
        speeds[0], speeds[1], speeds[2], speeds[3], speeds[4], speeds[5], speeds[6], speeds[7],
        speeds[8], speeds[9], speeds[10], speeds[11], speeds[12], speeds[13], speeds[14], speeds[15]);
        printf(read_in); // For debug
        // to_send = &read_in;
        // Serial.WriteSerialPort(to_send);

        /* Exits code if Esc is presed */
        char c = (char)cv::waitKey(25);
        if (c == 27) 
            break;

        // Sleep(100);
    }
    cap.release();
    return 0;
}
