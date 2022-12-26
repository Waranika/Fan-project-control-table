/**
 * @file main.cpp
 * @author Bernard Szeliga (b.szeliga@ecam.fr)
 * @brief Application analyzing the position of the ping-pong ball
 * on the frame captured by the camera and converts it to fan speed 
 * commands for "fan table" controlled by Arduino microcontroller
 * @version 0.1
 * @date 2022-12-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */
/** Includes **/
/* Standard libraries */
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <chrono>
/* External */
#include <opencv2/opencv.hpp>
#include "../external/simple-serial-port/SimpleSerial.h"
/* Internal */
#include "common_config.h"
#include "ImageProcessing/image_processing.h"
#include "Control/control.h"


int main()
{
    // Create camera capture
    cv::Mat frame;           
    if (camera_init(BUILT_IN))
        return -1;
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
    int x;
    int y;
    uint8_t speeds[16];
    std::chrono::high_resolution_clock::time_point now;
    std::chrono::high_resolution_clock::time_point calc_start_time;
    while (true)
    {
        calc_start_time = std::chrono::high_resolution_clock::now();
        // Transfer and resize captured frame for procesing
        frame = get_frame();

        // Get the location of ball
        Subprocesses sub;
        // sub.blure_split = true; sub.hsv_split = true;
        cv::Point loc = get_ball_location(frame, sub);
     
        frame = draw_visualization(frame, loc);
        // Display frame with marker on ball
        cv::namedWindow("Ball control", 1);
        cv::setMouseCallback("Ball control", CallBackFunc, NULL);
        imshow("Ball control", frame);
        x = loc.x;
        y = loc.y;
        simple_centering(loc, speeds);

        /* Sending the fans speeds to Arduino */
        sprintf_s(read_in,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
        speeds[0], speeds[1], speeds[2], speeds[3], speeds[4], speeds[5], speeds[6], speeds[7],
        speeds[8], speeds[9], speeds[10], speeds[11], speeds[12], speeds[13], speeds[14], speeds[15]);
        // to_send = &read_in;
        // Serial.WriteSerialPort(to_send);

        /* Visualization of fan sppeds */
        system("cls");
        printf_s("Ball location \tx:\t%d\ty:\t%d\n", x, y);
        printf_s("\t%d\t%d\t%d\t%d\t%d\n%d\t\t\t\t\t\t%d\n%d\t\t\t\t\t\t%d\n%d\t\t\t\t\t\t%d\n\t%d\t%d\t%d\t%d\t%d\n",
        speeds[0], speeds[1], speeds[2], speeds[3], speeds[4], speeds[15], speeds[5], speeds[14], speeds[6],
        speeds[13], speeds[7], speeds[12], speeds[11], speeds[10], speeds[9], speeds[8]);
        printf(read_in); // For debug

        /* Exits code if Esc is presed */
        char c = (char)cv::waitKey(25);
        if (c == 27) 
            break;
        
        
        /* Checking calculation time, debug purpouse only */
        printf("Calculation time: %dms\n",(int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - calc_start_time).count());

        /* Waiting for new frame */
        do{
            now = std::chrono::high_resolution_clock::now();
        }while(std::chrono::duration_cast<std::chrono::microseconds>(now - calc_start_time).count() < SAMPLING_TIME_US);
    }
    camera_deinit();
    return 0;
}
