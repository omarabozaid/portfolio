#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>

void test_container(){
    cv::Mat A = cv::imread("image.jpg", cv::IMREAD_COLOR);
    if (A.empty()) {
        std::cerr << "Error: Image not loaded." << std::endl;
    }

    int nPixels = A.total();
    std::cout<<" number of pixels is  "<<nPixels<<"\n";

    std::vector<double> i(nPixels,0);
    std::vector<double> j(nPixels,0);
    std::vector<double> k(nPixels,0);

    int row_counter=0;
    int col_counter=0;

    for(int row_counter=0;row_counter<A.rows;row_counter++){
        for(int col_counter=0;col_counter<A.cols;col_counter++){
            i.push_back(static_cast<double>(A.at<cv::Vec3b>(row_counter, col_counter)[0])/256);
            j.push_back(static_cast<double>(A.at<cv::Vec3b>(row_counter, col_counter)[1])/256);
            k.push_back(static_cast<double>(A.at<cv::Vec3b>(row_counter, col_counter)[2])/256);
        }
    }

    int resizing_pixels = 100; 
    cv::Mat input_image = A.clone();
    cv::Mat resized_image;
    cv::resize(input_image, resized_image, cv::Size(resizing_pixels,resizing_pixels));
    cv::imwrite("output_image.jpg", resized_image);
    cv::imshow("Original Image", input_image);
    cv::imshow("Resized Image", resized_image);

    cv::waitKey(0);
    cv::destroyAllWindows();

}

int main(int, char**) {
    test_container();
    return 0;
}
