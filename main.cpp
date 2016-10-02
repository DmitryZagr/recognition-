//#include <opencv2\highgui\highgui.hpp>
//#include <opencv2\imgproc\imgproc.hpp>

#include "VideoFaceDetector.hpp"
extern "C" {
#include "sender.h"
}

#include <iostream>

#define MAX_FACE_RECTANGLE_WIDTH ( 75 )
#define MAX_FACE_RECTANGLE_HEIGHT ( 75 )
#define MAX_FACE_RECTANGLE_DELTA ( 90 )
#define DEAD_RADIUS ( 25 )
#define FRAME_W ( 1080 )
#define FRAME_H ( 720 )

using namespace cv;
using namespace std;;

const cv::String    WINDOW_NAME("Camera video");
const cv::String    CASCADE_FILE("haarcascade_frontalface_default.xml");

void commandToDevaice(Point frameCentre, Point facePosition, Rect faceRect, int *soc);
bool deadCircule(Point frameCentre, Point facePosition);
void centrateDistance(Rect faceRect, int *soc);
//void centrateDistance(Rect faceRect, int *soc) 

bool centrateX(Point frameCentre, Point facePosition, int *soc);
bool centrateY(Point frameCentre, Point facePosition, int *soc);


void commandToDevaice(Point frameCentre, Point facePosition, Rect faceRect, int *soc) {
    
    if(!deadCircule(frameCentre, facePosition)) {
        if(centrateX(frameCentre, facePosition, soc))
            centrateY(frameCentre, facePosition, soc);
    } else {
        centrateDistance(faceRect, soc);
    }
    
}

bool deadCircule(Point frameCentre, Point facePosition) {
    if(abs(frameCentre.x - facePosition.x) <= DEAD_RADIUS &&
       abs(frameCentre.y - facePosition.y) <= DEAD_RADIUS)
        return true;
    return false;
}

bool centrateX(Point frameCentre, Point facePosition, int *soc) {
    
    cout << "centrate X \n" << cout.flush();
    
    
    int32_t deltaX = frameCentre.x - facePosition.x;
    
    if(abs(deltaX) <= DEAD_RADIUS)
        return true;
    else {
        if(deltaX > 0) {
            straveStepLeft(soc);
            cout << "centrate X left\n" << cout.flush();
        }
        else {
            straveStepRight(soc);
            cout << "centrate X right\n" << cout.flush();
        }
    }
    return false;
}

bool centrateY(Point frameCentre, Point facePosition, int *soc) {
    
//    int* soc;
    
    cout << "centrate Y \n" << cout.flush();
    
    int32_t deltaY = frameCentre.y - facePosition.y;
    
    if(deltaY > 0) {
        cout << "centrate Y up\n" << cout.flush();
        moveStepUp(soc);
    }
    else {
        cout << "centrate Y down\n" << cout.flush();
        moveStepDown(soc);
    }
    
    return true;
    
}

void centrateDistance(Rect faceRect, int *soc) {
    
    int32_t rectDelta  = MAX_FACE_RECTANGLE_DELTA;
    int32_t lowDelta   = MAX_FACE_RECTANGLE_DELTA - 10;
    int32_t hightDelta = MAX_FACE_RECTANGLE_DELTA + 10;
    
    if(lowDelta > faceRect.width) {
        moveStepForward(soc);
    }
    else {
        if(hightDelta < faceRect.width)
            moveStepBackward(soc);
    }
    
}


int main(int argc, char** argv)
{
    
//    int sock;
//    struct sockaddr_un server;
//    initConnection(&sock, NAME, &server);
    
    
    // Try opening camera
    cv::VideoCapture camera(0);
    //cv::VideoCapture camera("D:\\video.mp4");
    if (!camera.isOpened()) {
        fprintf(stderr, "Error getting camera...\n");
        exit(1);
    }

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_KEEPRATIO | cv::WINDOW_AUTOSIZE);

    VideoFaceDetector detector(CASCADE_FILE, camera);
    cv::Mat frame;
    
    
    camera.set(CV_CAP_PROP_FRAME_WIDTH,  FRAME_W);
    camera.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_H);
    
//    double fps = 0, time_per_frame;
    
    Point frameCentre(FRAME_W, FRAME_H);
    
    
    
    while (true)
    {
        //usleep(300000);
//        auto start = cv::getCPUTickCount();
        detector >> frame;
//        auto end = cv::getCPUTickCount();

//        time_per_frame = (end - start) / cv::getTickFrequency();
//        fps = (15 * fps + (1 / time_per_frame)) / 16;

//        printf("Time per frame: %3.3f\tFPS: %3.3f\n", time_per_frame, fps);
        
//        commandToDevaice(frameCentre, detector.facePosition(), detector.face(), &sock);

        cv::rectangle(frame, detector.face(), cv::Scalar(255, 0, 0));
        cv::circle(frame, detector.facePosition(), 30, cv::Scalar(0, 255, 0));

        cv::imshow(WINDOW_NAME, frame);
        if (cv::waitKey(25) == 27) break;
    }

    return 0;
}





















