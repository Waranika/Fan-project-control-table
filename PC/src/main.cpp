#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "config.h"

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
    // Calculate the centroid
    cv::Moments m = moments(threshFrame, false);
    cv::Point com(m.m10 / m.m00, m.m01 / m.m00);
    return com;
}

int main()
{
    // Create camera capture
    cv::Mat frame;           
    cv::VideoCapture cap(1); 
    // Exit code if no camera is detected
    if (!cap.isOpened())
    {
        std::cout << "No video stream detected" << std::endl;
        system("pause");
        return -1;
    }
    while (true)
    {
        // Transfer and resize captured frame for procesing
        cap >> frame;

        // Get the location of ball
        cv::Point loc = get_ball_location(frame);
        // Draw marker on received location
        cv::Scalar color = cv::Scalar(0, 0, 255);
        drawMarker(frame, loc, color, cv::MARKER_CROSS, 50, 5);

        // Display frame with marker on ball
        imshow("Ball", frame);

        // Exits code if Esc is presed
        char c = (char)cv::waitKey(25);
        if (c == 27) 
            break;
    }
    cap.release();
    return 0;
}