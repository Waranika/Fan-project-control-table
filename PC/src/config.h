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



#endif CONFIG_H