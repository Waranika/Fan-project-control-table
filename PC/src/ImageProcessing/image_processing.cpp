/** Includes **/
/* External */
#include <opencv2/opencv.hpp>
/* Internal */
#include "image_processing.h"
#include <../common_config.h>

/** Global variables **/
/* Video capture */
static cv::VideoCapture cap;
/* Work area, shared with controle modlue */
extern cv::Point work_area[2] = {cv::Point(TABLE_WIDTH_MIN, TABLE_HEIGHT_MIN), 
                                 cv::Point(TABLE_WIDTH_MAX, TABLE_HEIGHT_MAX)};
extern cv::Point target = cv::Point(DEFAULT_TARGET_X, DEFAULT_TARGET_Y);
/* Worka area buffers */
static cv::Point work_area_buffer[2] = {cv::Point(TABLE_WIDTH_MIN, TABLE_HEIGHT_MIN), 
                                 cv::Point(TABLE_WIDTH_MAX, TABLE_HEIGHT_MAX)};
static cv::Point target_buffer = cv::Point(DEFAULT_TARGET_X, DEFAULT_TARGET_Y);

cv::Point get_ball_location(cv::Mat frame, Subprocesses display)
{
    cv::Mat hsvFrame;
    /* Display image before processing */
    if (display.bgr)
        imshow("BGR", frame);
    /* Display image before processing layer-by-layer*/ 
    if (display.bgr_split){
        cv::Mat bgr_split[3];
        cv::split(frame,bgr_split);
        imshow("Blue",bgr_split[0]);
        imshow("Green",bgr_split[1]);
        imshow("Red",bgr_split[2]);
    }
    /* Convert colorspace to HSV for easier detection */
    cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);
    /* Display image after conversion to HSV */
    if (display.hsv)
        imshow("HSV",hsvFrame); // imshow uses BGR or gray-scaled so image in HSV looks wierd
    /* Display image after conversion to HSV layer-by-layer */
    if (display.hsv_split){
        cv::Mat hsv_split[3];
        cv::split(hsvFrame,hsv_split);
        imshow("Hue",hsv_split[0]);
        imshow("Saturation",hsv_split[1]);
        imshow("Value",hsv_split[2]);
    }
    /* Apply  blure for reducing impuls noise */
    // blur(hsvFrame, hsvFrame, cv::Size(1, 1));
    GaussianBlur(hsvFrame, hsvFrame, cv::Size(1, 1),1,1); // Gaussian supricingly is fastest 
    // medianBlur(hsvFrame, hsvFrame, 1);
    /* Show blured image */
    if (display.blure)
        imshow("Blure", hsvFrame); // Still HSV so sitll looks wierd
    /* Show blured image layer-by-layer */
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
    /* Display thresholded image */
    if (display.threshold)
        imshow("threshold",threshFrame);    
    // Calculate the centroid
    cv::Moments m = moments(threshFrame, false);
    cv::Point com(m.m10 / m.m00, m.m01 / m.m00);
    return com;
}

int camera_init(Camera camera){
    /* Open camera stream */
    cv::VideoCapture capture(camera);
    /* Coppy to global variable */
    cap = capture;
    /* Check if stream is open */
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
    /* Create new frame based on provided one */
    cv::Mat new_frame = frame;
    /* Draw visual elements */
    rectangle(frame, work_area[0], work_area[1], WORK_AREA_BORDER_COLOR, WORK_AREA_BORDER_THICKNESS,
    WORK_AREA_BORDER_LINE_TYPE, WORK_AREA_BORDER_SHIFT);
    drawMarker(new_frame, location, LOCATION_MARKER_COLOR, LOCATION_MARKER_TYPE, LOCATION_MARKER_SIZE,
    LOCATION_MARKER_THICKNESS);
    drawMarker(new_frame, target, TARGET_MARKER_COLOR, TARGET_MARKER_TYPE, TARGET_MARKER_SIZE,
    TARGET_MARKER_THICKNESS);
    return new_frame;
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if (event == cv::EVENT_LBUTTONDOWN){
        if (x < target.x && y < target.y){
            work_area_buffer[0].x = x;
            work_area_buffer[0].y = y;
        }
    }
    else if (event == cv::EVENT_RBUTTONDOWN){
        if (x > target.x && y > target.y){
            work_area_buffer[1].x = x;
            work_area_buffer[1].y = y;
        }
    }
    else if (event == cv::EVENT_MBUTTONDOWN){
        if (x > work_area[0].x && x < work_area[1].x &&
            y > work_area[0].y && y < work_area[1].y){
            target_buffer.x = x;
            target_buffer.y = y;
        }
    }
}

void update_work_area(void){
    work_area[0] = work_area_buffer[0];
    work_area[1] = work_area_buffer[1];
    target = target_buffer;
}
