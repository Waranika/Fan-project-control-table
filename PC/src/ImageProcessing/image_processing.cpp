#include "image_processing.h"
#include <../common_config.h>

#include <opencv2/opencv.hpp>

static cv::VideoCapture cap;
extern cv::Point work_area[2] = {cv::Point(TABLE_WIDTH_MIN, TABLE_HEIGHT_MIN), 
                                 cv::Point(TABLE_WIDTH_MAX, TABLE_HEIGHT_MAX)};
extern cv::Point target = cv::Point(DEFAULT_TARGET_X, DEFAULT_TARGET_Y);

cv::Point get_ball_location(cv::Mat frame, Subprocesses display)
{
    // Convert colorspace to HSV for easier detection
    cv::Mat hsvFrame;
    if (display.bgr)
        imshow("BGR", frame);
    if (display.bgr_split){
        cv::Mat bgr_split[3];
        cv::split(frame,bgr_split);
        imshow("Blue",bgr_split[0]);
        imshow("Green",bgr_split[1]);
        imshow("Red",bgr_split[2]);
    }
    cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);
    if (display.hsv)
        imshow("HSV",hsvFrame);
    if (display.hsv_split){
        cv::Mat hsv_split[3];
        cv::split(hsvFrame,hsv_split);
        imshow("Hue",hsv_split[0]);
        imshow("Saturation",hsv_split[1]);
        imshow("Value",hsv_split[2]);
    }
    // Apply gausian blure for reducing impuls noise
    blur(hsvFrame, hsvFrame, cv::Size(1, 1));
    if (display.blure)
        imshow("Blure", hsvFrame);
    if (display.blure_split){
        cv::Mat hsv_split[3];
        cv::split(hsvFrame,hsv_split);
        imshow("Hue blured",hsv_split[0]);
        imshow("Saturation blured",hsv_split[1]);
        imshow("Value blured",hsv_split[2]);
    }
    // Threshold the image
    cv::Scalar lowerBound = cv::Scalar(H_MIN, S_MIN, V_MIN);
    cv::Scalar upperBound = cv::Scalar(H_MAX, S_MAX, V_MAX);
    cv::Mat threshFrame;
    inRange(hsvFrame, lowerBound, upperBound, threshFrame);
    if (display.threshold)
        imshow("threshold",threshFrame);    
    // Calculate the centroid
    cv::Moments m = moments(threshFrame, false);
    cv::Point com(m.m10 / m.m00, m.m01 / m.m00);
    return com;
}

int camera_init(Camera camera){
    cv::VideoCapture capture(camera);
    cap = capture;
    if (!cap.isOpened())
    {
        std::cout << "NO VIDEO STREAM DETECTED, ABORTING" << std::endl;
        system("pause");
        return -1;
    }
    return 0;
}

void camera_deinit(void){
    cap.release();
}

cv::Mat get_frame(void){
    cv::Mat frame;
    cap >> frame;
    return frame;
}

cv::Mat draw_visualization(cv::Mat frame, cv::Point location){
    cv::Mat new_frame = frame;
    rectangle(frame, work_area[0], work_area[1], WORK_AREA_BORDER_COLOR, WORK_AREA_BORDER_THICKNESS,
    WORK_AREA_BORDER_LINE_TYPE, WORK_AREA_BORDER_SHIFT);
    drawMarker(new_frame, location, LOCATION_MARKER_COLOR, LOCATION_MARKER_TYPE, LOCATION_MARKER_SIZE,
    LOCATION_MARKER_THICKNESS);
    drawMarker(new_frame, target, TARGET_MARKER_COLOR, TARGET_MARKER_TYPE, TARGET_MARKER_SIZE,
    TARGET_MARKER_THICKNESS);
    return new_frame;
}

void visualization_init(void){
        work_area[0].x = TABLE_WIDTH_MIN;
        work_area[0].y = TABLE_HEIGHT_MIN;
        work_area[1].x = TABLE_WIDTH_MAX;
        work_area[1].y = TABLE_HEIGHT_MAX;
        target.x = (TABLE_WIDTH_MAX - TABLE_WIDTH_MIN) / 2 + TABLE_WIDTH_MIN;
        target.y = (TABLE_HEIGHT_MAX - TABLE_HEIGHT_MIN) / 2 + TABLE_HEIGHT_MIN;
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
     if ( event == cv::EVENT_LBUTTONDOWN )
     {
          if (x < target.x && y < target.y){
            work_area[0].x = x;
            work_area[0].y = y;
          }
     }
     else if ( event == cv::EVENT_RBUTTONDOWN )
     {
          if (x > target.x && y > target.y){
            work_area[1].x = x;
            work_area[1].y = y;
          }
     }
     else if ( event == cv::EVENT_MBUTTONDOWN )
     {
          if (x > work_area[0].x && x < work_area[1].x &&
              y > work_area[0].y && y < work_area[1].y){
            target.x = x;
            target.y = y;
          }
     }
}
