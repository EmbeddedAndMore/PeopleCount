#include <iostream>
#include <stdio.h>
#include <opencv4/opencv2/opencv.hpp>
#include <vector>
#include <thread>
#include <chrono>
#include <string>

#include "ConcurrentQueue.h"

using namespace cv;

void init()
{

}
int frame_produce(ConcurrentQueue<cv::Mat>& queue, int deviceId ,std::string vidPath="") {
    Mat frame;
    VideoCapture cap;

    int apiID = cv::CAP_ANY;      // 0 = autodetect default API

    if(vidPath.empty())
        cap.open(deviceId, apiID);
    else
        cap.open(vidPath, apiID);

    // check if we succeeded
    if (!cap.isOpened()) {
        std::cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    //--- GRAB AND WRITE LOOP
    std::cout << "Start grabbing" << std::endl
         << "Press any key to terminate" << std::endl;
    for(;;)
    {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        // check if we succeeded
        if (frame.empty()) {
            std::cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        queue.push(frame);
        std::cout << cap.get(cv::CAP_PROP_POS_FRAMES) << std::endl;

    }
    return 1;
}

void consume(ConcurrentQueue<int>& q, unsigned int id) {

}
int main(int argc, char** argv ) {

    ConcurrentQueue<cv::Mat> q(10);

    // frame producer thread
    std::thread prod_thr([&q] { return frame_produce(q,0, "/home/mohamad/projects/kurzvortrag/%03d.bmp"); });

    for(;;)
    {
        Mat frame = q.pop();
        imshow("frame",frame );
        waitKey(5);

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }


//    auto cam = VideoCapture(0);
//
//
//    Mat im = imread( "blob.jpg", IMREAD_GRAYSCALE );
//    waitKey(0);
//
//    SimpleBlobDetector::Params params;
//
//    params.minThreshold = 10;
//    params.maxThreshold = 200;
//    params.thresholdStep = 100;
//
//    // Filter by Area.
//    params.filterByArea = true;
//    params.minArea = 1500;
//
//    // Filter by Circularity
//    params.filterByCircularity = false;
//    params.minCircularity = 0.1;
//
//    // Filter by Convexity
//    params.filterByConvexity = false;
//    params.minConvexity = 0.87;
//
//    // Filter by Inertia
//    params.filterByInertia = false;
//    params.minInertiaRatio = 0.01;
//
//
//    // Storage for blobs
//    std::vector<KeyPoint> keypoints;
//
//    // Set up detector with params
//    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
//
//    double start = getTickCount();
//    // Detect blobs
//    detector->detect( im, keypoints);
//    double duration = (getTickCount() - start) / getTickFrequency();
//    std::cout << "Duration: " << duration << "Secs";
//    // Draw detected blobs as red circles.
//    // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
//    // the size of the circle corresponds to the size of blob
//
//    Mat im_with_keypoints;
//    drawKeypoints( im, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
//
//    // Show blobs
//    imshow("keypoints", im_with_keypoints );
//    waitKey(0);


    prod_thr.join();
    return 0;
}
