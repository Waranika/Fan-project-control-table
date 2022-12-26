/**
 * @file common_config.h
 * @author Bernard Szeliga (b.szeliga@ecam.fr)
 * @brief File containing configuration shared between different internal modules
 * @version 0.1
 * @date 2022-12-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef CONFIG_H
#define CONFIG_H

/**
 * HSV range for color thresholding
 * H - Hue <0-179>
 * S - Saturation <0-255>
 * V - Value <0-255>
 */
#define H_MIN 7 
#define H_MAX 22
#define S_MIN 125
#define S_MAX 255
#define V_MIN 125
#define V_MAX 255

/**
 * Camera ranges
 * 
 * Maximum values for which it can be said that the ball was successfully detected
 */
#define CAMERA_WIDTH_MAX 640
#define CAMERA_HEIGHT_MAX 470


/**
 * Table ranges
 *  
 * Ranges of values in picture concidered to be inside the work area
 */
#define TABLE_WIDTH_MIN 20
#define TABLE_WIDTH_MAX 620
#define TABLE_HEIGHT_MIN 90
#define TABLE_HEIGHT_MAX 450

#define DEFAULT_TARGET_X (TABLE_WIDTH_MAX - TABLE_WIDTH_MIN) / 2 + TABLE_WIDTH_MIN
#define DEFAULT_TARGET_Y (TABLE_HEIGHT_MAX - TABLE_HEIGHT_MIN) / 2 + TABLE_HEIGHT_MIN

/**
 * Information about fans
 * 
 */
#define NO_FANS 16
#define NO_FANS_X 5
#define NO_FANS_Y 3
#define FAN_MAX_SPEED 255
#define FAN_WIDTH (int)((TABLE_WIDTH_MAX - TABLE_WIDTH_MIN) / NO_FANS_X)

/**
 * Communication configuration
 * 
 */
#define COM_PORT "\\\\.\\COM8"
#define BOUD_RATE (DWORD)CBR_9600

/**
 * PID configuration
 * 
 */
#define SAMPLING_TIME_US 33333 // 1/30Hz
#define PID_KP 0.5
#define PID_KI 0.02
#define PID_KD 0.002
#define PID_TAU 0.001

/**
 * OpenCV color definitions
 * 
 * cv::Scalar stores BGR values of colors
 */
#define OPENCV_GREEN cv::Scalar(0, 255, 0)
#define OPENCV_RED cv::Scalar(0, 0, 255)
#define OPENCV_BLUE cv::Scalar(255, 0, 0)
#define OPENCV_BLACK cv::Scalar(0, 0, 0)
#define OPENCV_WHITE cv::Scalar(255, 255, 255)

/**
 * Visual components
 * 
 */
#define WORK_AREA_BORDER_COLOR OPENCV_GREEN
#define WORK_AREA_BORDER_THICKNESS 2
#define WORK_AREA_BORDER_LINE_TYPE 8
#define WORK_AREA_BORDER_SHIFT 0

#define LOCATION_MARKER_COLOR OPENCV_RED
#define LOCATION_MARKER_TYPE cv::MARKER_CROSS
#define LOCATION_MARKER_SIZE 30
#define LOCATION_MARKER_THICKNESS 3

#define TARGET_MARKER_COLOR OPENCV_BLUE
#define TARGET_MARKER_TYPE cv::MARKER_TILTED_CROSS 
#define TARGET_MARKER_SIZE 30
#define TARGET_MARKER_THICKNESS 3


#endif CONFIG_H