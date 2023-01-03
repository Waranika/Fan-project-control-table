/**
 * @file image_processing.h
 * @author Bernard Szeliga
 * @brief Image processing module of fan table project
 * @version 0.1
 * @date 2023-01-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <opencv2/opencv.hpp>

/**
 * @brief Enumeration for camera selection
 * 
 */
typedef enum{
    BUILT_IN = 0,
    EXTERNAL = 1
}Camera;

/**
 * @brief Struct for selection of displayed subprocesses
 * 
 */
typedef struct {
    /// @brief Displaying preprocesses image
    bool bgr = false;
    /// @brief Displaying preprocessed image layer-by-layer
    bool bgr_split = false;
    /// @brief Displaying image converted to HSV
    bool hsv = false;
    /// @brief Displaying image converted to HSV layer-by-layer
    bool hsv_split = false;
    /// @brief Displaying blured image
    bool blure = false;
    /// @brief Displaying blured image layer-by-layer
    bool blure_split = false;
    /// @brief Displaying threcholded image
    bool threshold = false;
}Subprocesses;


/**
 * @brief Get the ball location object
 * Locates the ball of color defined by threshold range in config.h
 *
 * @param frame cv::Mat object contajning captured frame
 * @param display Option to decide if any of subprocesses should be
 * displayed in additional window
 * @return cv::Point location of a ball
 */
cv::Point get_ball_location(cv::Mat frame, Subprocesses display);

/**
 * @brief Initializes camera
 * 
 * @param camera Select camera for initialization
 * @return error code, 0 if sucassefull 
 */
int camera_init(Camera camera);

/**
 * @brief Releases initialized camera
 * 
 */
void camera_deinit(void);

/**
 * @brief Capture frame using initialized camera
 * 
 */
cv::Mat get_frame(void);

/**
 * @brief Draws additional elements on provided frame for visualization
 * of operation of the program
 * 
 * @param frame Oryginal frame captured by camera
 * @param location Current location of the ball
 * @param target Target location of the ball
 * @param work_area Points defining 2 opposite corners of the work area
 * @return cv::Mat New frame with added elements
 */
cv::Mat draw_visualization(cv::Mat frame, cv::Point location);

/**
 * @brief Callback function for the mouse
 * 
 * Function processes mouse events and checks if associated action is valuable.
 * 
 * Left mouse click moves top-left corner of work area
 * Right mouse click moves bottom-right corner of work area
 * Middle click moves set-point
 * 
 * If new location would move set-point outside work area event is ignored
 * This also prevents changing of place between corners of work area
 * 
 * New locations are stored in buffer and needs to be flushed to prevent
 * variable coruption in different parts of code
 */
void CallBackFunc(int event, int x, int y, int flags, void* userdata);

/**
 * @brief Function updating work area and set-point acording to mouse events
 * 
 * Function flushes buffers filled by CallBackFunc into use variables
 */
void update_work_area(void);

#endif 