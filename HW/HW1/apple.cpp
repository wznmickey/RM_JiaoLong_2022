#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <assert.h>
#include <getopt.h>
#include <vector>
//int get_num(int argc, char *argv[]);
bool cmp (std::vector<cv::Point> x1,std::vector<cv::Point> x2)
{
    if (cv::contourArea(x1)<cv::contourArea(x2)) return false;
    else return true;
}
struct Apple
{
    int x1, y1, x2, y2;
    //double xc,yc;
};
Apple get_contour(const std::vector<std::vector<cv::Point>> &contours)
{
    Apple apple;
    //assert(contours.size() == 1);
    auto c = contours[0];
    apple.x1=10000;
    apple.y1=10000;
    apple.x2=-1;
    apple.y2=-1;
    //int tempy1,tempy2,tempx1,tempx2;
    for (auto it = c.begin(); it != c.end(); it++)
    {
        apple.x1 = std::min(apple.x1, (*it).x);
        apple.y1 = std::min(apple.y1, (*it).y);
        apple.x2 = std::max(apple.x2, (*it).x);
        apple.y2 = std::max(apple.y2, (*it).y);
        //if (apple.x1==(*it).x) tempy1=(*it).y;
        //if (apple.x2==(*it).x) tempy2=(*it).y;
        //if (apple.y1==(*it).y) tempx1=(*it).x;
        //if (apple.y2==(*it).y) tempx2=(*it).x;
    }
    std::cout<<apple.x1<<std::endl;
    std::cout<<apple.x2<<std::endl;
    std::cout<<apple.y1<<std::endl;
    std::cout<<apple.y2<<std::endl;
    //std::cout<<tempx1<<std::endl;

    //std::cout<<tempx2<<std::endl;
    //apple.yc=tempy1;
    //apple.xc=((double)(tempx1+tempx2) )/ 2;
    //apple.yc=((double)(tempy1+tempy2) )/ 2;
    //double al=apple.xc-apple.x1;
    //double ar=(double)apple.x2-apple.xc;
    //double bt=apple.yc-apple.y1;
    //double bb=(double)apple.y2-apple.yc;
    //std::cout<<al<<std::endl;
    //    std::cout<<ar<<std::endl;
    //std::cout<<bt<<std::endl;
    //   std::cout<<bb<<std::endl;

    //apple.x1=apple.xc-(std::max(al,ar)); 
    //apple.x2=apple.xc+(std::max(al,ar)); 
    //apple.y1=apple.yc-(std::max(bt,bb)); 
    //apple.y2=apple.yc+(std::max(bt,bb)); 
    return apple;
}
void drawOrigin(const std::vector<std::vector<cv::Point>> &contours,cv::Mat &src)
{
    auto c = contours[0];
    for (auto it=c.begin();it!=c.end();it++)
    {
        src.at<cv::Vec3b>((*it).y,(*it).x)[0] =(uchar)0;
        src.at<cv::Vec3b>((*it).y,(*it).x)[1] =(uchar)0;
        src.at<cv::Vec3b>((*it).y,(*it).x)[2] =(uchar)255;
    }
}
void draw(Apple &apple,cv::Mat &src)
{
    for(int i=apple.x1;i<=apple.x2;i++)
    {
        src.at<cv::Vec3b>(apple.y1,i)[0] =(uchar)0;
        src.at<cv::Vec3b>(apple.y1,i)[1] =(uchar)0;
        src.at<cv::Vec3b>(apple.y1,i)[2] =(uchar)255;
        src.at<cv::Vec3b>(apple.y2,i)[0] =(uchar)0;
        src.at<cv::Vec3b>(apple.y2,i)[1] =(uchar)0;
        src.at<cv::Vec3b>(apple.y2,i)[2] =(uchar)255;
   }
   for(int i=apple.y1;i<=apple.y2;i++)
    {
        src.at<cv::Vec3b>(i,apple.x1)[0] =(uchar)0;
        src.at<cv::Vec3b>(i,apple.x1)[1] =(uchar)0;
        src.at<cv::Vec3b>(i,apple.x1)[2] =(uchar)255;
        src.at<cv::Vec3b>(i,apple.x2)[0] =(uchar)0;
        src.at<cv::Vec3b>(i,apple.x2)[1] =(uchar)0;
        src.at<cv::Vec3b>(i,apple.x2)[2] =(uchar)255;
   }
   return ;
}
int main(int argc, char **argv)
{

    cv::Mat src;
    src = cv::imread("apple.jpg");
    std::cout<<src.depth()<<std::endl;
    cv::Mat hsv;
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);
    cv::Mat hsv_part1, hsv_part2,hsv_part3;
    cv::inRange(hsv, cv::Scalar(0, 43, 46), cv::Scalar(10, 255, 255), hsv_part1);
    cv::inRange(hsv, cv::Scalar(156, 43, 46), cv::Scalar(180, 255, 255), hsv_part2);
    cv::inRange(hsv,cv::Scalar(10,200,50),cv::Scalar(30,255,255),hsv_part3);
    cv::Mat ones_mat = cv::Mat::ones(cv::Size(src.cols, src.rows), CV_8UC1);
    cv::Mat hsv_result = 255 * (ones_mat - (ones_mat - hsv_part1 / 255).mul(ones_mat - hsv_part2 / 255).mul(ones_mat - hsv_part3 / 255));
    //cv:: Mat blur;
    //cv::GaussianBlur(hsv_result, blur, cv::Size(3, 3), 0, 0);
    cv::imshow("apple", hsv_result);   cv::waitKey(0);
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));

    cv::morphologyEx(hsv_result, hsv_result, cv::MORPH_OPEN, element);
    cv::morphologyEx(hsv_result, hsv_result, cv::MORPH_CLOSE, element);
    cv::Mat canny_result;

    cv::Canny(hsv_result, canny_result, 100, 100, 3);
    std::vector<std::vector<cv::Point>> contour;
    std::vector<cv::Vec4i> hierachy;

    cv::findContours(canny_result, contour, hierachy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    sort(contour.begin(),contour.end(),cmp);
    Apple apple = get_contour(contour);
    draw(apple,src);
    drawOrigin(contour,src);
    cv::imshow("apple", src);
    cv::waitKey(0);
    return 0;
}
/*
int get_num(int argc, char *argv[])
{
    char get_index;
    int option_index = 0;
    struct option long_options[1] = {

        {"num", 1, NULL, 'n'},

    };
    while ((get_index = getopt_long(argc, argv, "n:", long_options, &option_index)) != -1)
    {
        switch (get_index)
        {
        case 'n':
            return (int)atol(optarg);
        default:
            return 0;
        }
    }
}
*/