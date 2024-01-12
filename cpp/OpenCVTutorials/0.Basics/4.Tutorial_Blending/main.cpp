#include <iostream>
#include <vector>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

int main(){
    cv::Mat A , B, C;
    A = cv::imread("LinuxLogo.jpg");
    B = cv::imread("WindowsLogo.jpg");

    double alpha,beta,gamma;
    alpha=0.7;
    beta=1-alpha;
    gamma=0.2;
    //C = alpha*A+beta*B+gamma
    cv::addWeighted(A,alpha,B,beta,gamma,C);
    cv::imshow("test",C);
    cv::waitKey(0);
    return 0;
}