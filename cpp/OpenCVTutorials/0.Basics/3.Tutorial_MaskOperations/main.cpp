#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>




int main(){
    cv::Mat A = cv::Mat_<char>(cv::Size(3, 3), CV_8S);
    A.at<char>(0,0)=static_cast<char>(0);
    A.at<char>(0,1)=static_cast<char>(-1);
    A.at<char>(0,2)=static_cast<char>(0);
    A.at<char>(1,0)=static_cast<char>(-1);
    A.at<char>(1,1)=static_cast<char>(5);
    A.at<char>(1,2)=static_cast<char>(-1);
    A.at<char>(2,0)=static_cast<char>(0);
    A.at<char>(2,1)=static_cast<char>(-1);
    A.at<char>(2,2)=static_cast<char>(0);


    cv::Mat B = cv::imread("image.jpg",cv::IMREAD_COLOR);

    cv::Mat C;

    cv::filter2D( B, C, B.depth(), A );

    cv::imshow("test",C);

    cv::waitKey(0);

    return 0;
}