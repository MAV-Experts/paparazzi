
using namespace std;
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/features2d.hpp>
using namespace cv;
#include "modules/computer_vision/opencv_image_functions.h"

#include "marker_checkers.h"

Mat img_marker;
Ptr<ORB> detector;

void init_detect_checkers(void) {

    img_marker = imread("/data/ftp/internal_000/imav/marker_checkers.png", IMREAD_GRAYSCALE);

    detector = ORB::create();
    detector->setMaxFeatures(20);

    std::vector<KeyPoint> keypoints;

    detector->detect(img_marker, keypoints);
}


struct resultsc opencv_detect_checkers(char *img, int width, int height) {

    struct resultsc marker;

    Mat M(height, width, CV_8UC2, img);
    Mat image;

    cvtColor(M, image, CV_YUV2GRAY_Y422);

    std::vector<KeyPoint> keypoints;

    detector->detect(image, keypoints);

    drawKeypoints(image, keypoints, image, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

    cvtColor(image, image, CV_BGR2GRAY);

    grayscale_opencv_to_yuv422(image, img, width, height);

    return marker;
}