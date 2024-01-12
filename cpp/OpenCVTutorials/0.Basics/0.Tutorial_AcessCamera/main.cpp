#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cstdlib>

void test_camera(int camera_index){
    cv::VideoCapture camera(camera_index, cv::CAP_V4L2);
    if (!camera.isOpened()) {
        std::cerr << "ERROR: Could not open camera \n";
        return;
    }
    cv::Mat frame;
    //Loop till break
    while (1) {
        camera >> frame;
        cv::imshow("Webcam", frame);
        auto key = cv::waitKey(10);
        // Check if the 'X' key is pressed
        if (key == 'x' || key == 'X') {
            std::cout<<"would you like to save the photo press y or n \n";
            auto accept_key = cv::waitKey(0);
            if(accept_key== 'y' || accept_key=='Y'){
                cv::imwrite("captured_image.jpg", frame);
                std::cout << "Image captured and saved as captured_image.jpg \n";
            }
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    int camera_index = 0 ;
    if(argc>1){
        camera_index = atoi(argv[1]);
    }
    test_camera(camera_index);
    return 0;
}
