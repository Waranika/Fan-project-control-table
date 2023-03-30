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
#include "Communication/communication.h"
#include "DebugUtils/utils.h"

int main()
{
    // Create camera capture 
    if (camera_init(EXTERNAL))
        // Exit code if no camera is detected  
        return -1;

    /* Local variables */
    uint8_t speeds[16];
    std::chrono::high_resolution_clock::time_point now, calc_start_time;
    cv::Mat frame; 

    /* Selecting displayed subprocesses */
    Subprocesses sub;
    sub.threshold = true;

    /* Creating window and asigning callback function for mouse */
    cv::namedWindow("Ball control", 1);
    cv::setMouseCallback("Ball control", CallBackFunc, NULL);
    /* Main event loop */
    while (true)
    {
        calc_start_time = std::chrono::high_resolution_clock::now();
        system("cls");
        /* Frame capture */
        frame = get_frame();
        /* Image processing and ball detection */
        cv::Point loc = get_ball_location(frame, sub);
        /* Loading work area from buffer and displying visual elements */
        update_work_area();
        frame = draw_visualization(frame, loc);
        imshow("Ball control", frame);
        
        /* Calculating fan speeds */
        #if CONTROL_METHOD == SIMPLE
            /* PID */
            PID_controller(loc, speeds);
        #else
            /* Simple */
            simple_centering(loc, speeds);
        #endif

        /* Sending the fans speeds to Arduino */
        send_to_table(speeds);

        /* Exits code if Esc is presed */ 
        char c = (char)cv::waitKey(1);
        if (c == 27) 
            break;
        /* Loop time synchronization */
        do{
            now = std::chrono::high_resolution_clock::now();
        }while(std::chrono::duration_cast<std::chrono::microseconds>(now - calc_start_time).count() < SAMPLING_TIME_US);

    }
    /* Releasing camera */
    camera_deinit();
    return 0;
}
