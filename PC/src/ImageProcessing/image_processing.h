#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <opencv2/opencv.hpp>

typedef enum{
    BUILT_IN = 0,
    EXTERNAL = 1
}Camera;

typedef struct {
    bool bgr = false;
    bool bgr_split = false;
    bool hsv = false;
    bool hsv_split = false;
    bool blure = false;
    bool blure_split = false;
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

void CallBackFunc(int event, int x, int y, int flags, void* userdata);

#endif 