#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d.hpp>

using namespace cv;
using namespace std;
bool cmp(std::vector<cv::Point> x1, std::vector<cv::Point> x2)
{
    if (cv::contourArea(x1) < cv::contourArea(x2))
        return false;
    else
        return true;
}
void doPerspectiveTransform(Mat input, Mat &output, std::vector<cv::Point> ans)
{
    std::vector<Point2f> srcQuad(4), dstQuad(4);
    output = input.clone();
    srcQuad[0].x = ans[0].x, srcQuad[1].x = ans[2].x, srcQuad[2].x = ans[1].x, srcQuad[3].x = ans[3].x;
    srcQuad[0].y = ans[0].y, srcQuad[1].y = ans[2].y, srcQuad[2].y = ans[1].y, srcQuad[3].y = ans[3].y;
    dstQuad[0].x = 20, dstQuad[1].x = 20, dstQuad[2].x = 100, dstQuad[3].x = 100;
    dstQuad[0].y = 300, dstQuad[1].y = 20, dstQuad[2].y = 20, dstQuad[3].y = 300;

    Mat warp_matrix = getPerspectiveTransform(srcQuad, dstQuad);

    warpPerspective(input, output, warp_matrix, Size(120, 320));
}
float getDistance(const cv::Point &x, const cv::Point &y)
{
    return sqrt(abs(x.x - y.x) * abs(x.x - y.x) + abs(x.y - y.y) * abs(x.y - y.y));
}

std::vector<cv::Point> getPosition(Mat src)
{
    cv::Mat hsv;
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);
    cv::Mat hsv_part1, hsv_part2, hsv_part3;
    cv::inRange(hsv, cv::Scalar(100, 43, 46), cv::Scalar(124, 255, 255), hsv_part1);
    cv::Mat ones_mat = cv::Mat::ones(cv::Size(src.cols, src.rows), CV_8UC1);
    cv::Mat hsv_result = 255 * (ones_mat - (ones_mat - hsv_part1 / 255));
    cv::imshow("apple", hsv_result);
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));

    cv::morphologyEx(hsv_result, hsv_result, cv::MORPH_OPEN, element);
    cv::morphologyEx(hsv_result, hsv_result, cv::MORPH_CLOSE, element);
    cv::imshow("apple", hsv_result);
    std::vector<std::vector<cv::Point>> contour;
    std::vector<cv::Vec4i> hierachy;

    cv::findContours(hsv_result, contour, hierachy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    sort(contour.begin(), contour.end(), cmp);
    std::vector<cv::Point> aim = contour[0];
    std::vector<Point2f> srcQuad(4);
    cv::Moments moment;
    moment = moments(aim);
    cv::Point far;
    far.x = moment.m10 / moment.m00;
    far.y = moment.m01 / moment.m00;
    std::vector<cv::Point> ans;

    for (auto it = aim.begin(); it != aim.end(); it++)
    {
        ans.push_back(*it);
    }
    sort(ans.begin(), ans.end(), [&](const cv::Point &x1, const cv::Point &x2)
         { return getDistance(x1, far) > getDistance(x2, far); });
    std::vector<cv::Point> tempV;
    tempV.push_back(ans[0]);
    auto far1 = ans[0];
    sort(ans.begin(), ans.end(), [&](const cv::Point &x1, const cv::Point &x2)
         { return getDistance(x1, far1) > getDistance(x2, far1); });
    tempV.push_back(ans[0]);
    auto far2 = ans[0];
    sort(ans.begin(), ans.end(), [&](const cv::Point &x1, const cv::Point &x2)
         { return ((getDistance(x1, far2)) + getDistance(x1, far1)) > ((getDistance(x2, far2)) + getDistance(x2, far1)); });
    tempV.push_back(ans[0]);
    auto far3 = ans[0];
    for (auto it = ans.begin(); it != ans.end(); it++)
    {
        auto tempAns = *it;

        if (((tempAns.x - far.x) * (far3.x - far.x) < 0) && ((tempAns.y - far.y) * (far3.y - far.y) < 0))
        {
            tempV.push_back(tempAns);
            break;
        }
    }
    return tempV;
}
int main()
{
    Mat img = imread("data pack/car.jpg");
    Mat result;

    std::vector<cv::Point> ans = getPosition(img);
    doPerspectiveTransform(img, result, ans);
    transpose(result, result);
    flip(result, result, 1);

    namedWindow("input");
    namedWindow("output");

    imshow("input", img);
    imshow("output", result);

    waitKey(0);

    destroyWindow("input");
    destroyWindow("output");
    return 0;
}