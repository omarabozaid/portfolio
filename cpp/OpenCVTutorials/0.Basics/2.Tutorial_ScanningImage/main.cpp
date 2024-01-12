#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

class LookUpTable;

class LookUpTableInstance {
public:
    
    friend class LookUpTable;
    const std::vector<uchar>& Table() const { return Table_; }

private:
    LookUpTableInstance() : Table_(std::vector<uchar>(256, 0)) , DivideWidth_(10){
        for(int i = 0; i<256; i++){
            Table_[i] = (uchar)(DivideWidth_*(i/DivideWidth_)) ;
        }
    };
    int DivideWidth_;
    std::vector<uchar> Table_;
};

class LookUpTable {
public:
    static const LookUpTableInstance& LUT() {
        return LUT_;
    }

private:
    static LookUpTableInstance LUT_;
};

LookUpTableInstance LookUpTable::LUT_;


int main(){
    const auto& LUT = LookUpTable::LUT().Table();
    cv::Mat A = cv::imread("image.jpg", cv::IMREAD_COLOR);
    if (A.empty()) {
        std::cerr << "Error: Image not loaded." << std::endl;
    }
    double t = (double)cv::getTickCount();
    for(int row_counter=0;row_counter<A.rows;row_counter++){
        for(int col_counter=0;col_counter<A.cols;col_counter++){
            A.at<cv::Vec3b>(row_counter, col_counter)[0] = LUT[A.at<cv::Vec3b>(row_counter, col_counter)[0]];
            A.at<cv::Vec3b>(row_counter, col_counter)[1] = LUT[A.at<cv::Vec3b>(row_counter, col_counter)[1]];
            A.at<cv::Vec3b>(row_counter, col_counter)[2] = LUT[A.at<cv::Vec3b>(row_counter, col_counter)[2]];
        }
    }
    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
    std::cout << "Times passed in seconds: " << t << "\n";
    cv::imwrite("output_image.jpg", A);
    return 0;
}