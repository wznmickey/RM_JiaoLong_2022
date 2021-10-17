#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
using namespace cv;
int main()
{
    const int board_w = 9,
              board_h = 6;
    const int board_n = board_w * board_h;
    Size board_size(9,6);
    Mat gray_img, drawn_img;
    std::vector<Point2f> point_pix_pos_buf;
    std::vector<std::vector<Point2f>> point_pix_pos;
    int found, successes = 0;
    Size img_size;
    int cnt = 0;
    int k = 0, n = 0;
    int testAns;
    double minAns=1000;
    //for (int testAns=4989;testAns>=3;testAns-=2)
    //{
    for (int i = 0; i < 24; i++)
    {
        //std::cout<<3<<std::endl;
        std::string path="calib2/";
        cv::Mat src0;
        cv::Mat src = cv::imread(path.append((std::__cxx11::to_string(i).append("_orig.jpg"))));
        
        //std::cout<<src0;
                //std::cout<<4<<std::endl;
        cv::Mat src1;
        cv::cvtColor(src, src1, cv::COLOR_BGR2GRAY);
                //std::cout<<5<<std::endl;
        cv::Mat src2;

        cv::adaptiveThreshold(src1, src0, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 87, 0);
               // std::cout<<6<<std::endl;

        //cv::imshow("result", src0);
        //cv::waitKey(0);
        if (!cnt)
        {
            img_size.width = src0.cols;
            img_size.height = src0.rows;
        }
        found = findChessboardCorners(src0, board_size, point_pix_pos_buf);
        //std::cout<<found<<std::endl;
        if (found && point_pix_pos_buf.size() == board_n)
        {
            successes++;
            //cvtColor(src0, gray_img, COLOR_BGR2GRAY);
            find4QuadCornerSubpix(src0, point_pix_pos_buf, Size(5, 5));
            point_pix_pos.push_back(point_pix_pos_buf);
            drawn_img = src0.clone();
            drawChessboardCorners(drawn_img, board_size, point_pix_pos_buf, found);
            cv::Mat show ;
            cv::resize(drawn_img, show,Size(960, 540));
            imshow("corners", show);
            waitKey(50);
        }
        else std::cout << "failed in"<< i << std::endl;
        //std::cout << "1" << std::endl;
        point_pix_pos_buf.clear();
                //std::cout << "2" << std::endl;
        cnt++;
        
    };
    std::cout << successes << " useful chess boards" << std::endl;
    Size square_size(10, 10);
    std::vector<std::vector<Point3f>> point_grid_pos;
    std::vector<Point3f> point_grid_pos_buf;
    std::vector<int> point_count;
    Mat camera_matrix(3, 3, CV_32FC1, Scalar::all(0));
    Mat dist_coeffs(1, 5, CV_32FC1, Scalar::all(0));
    std::vector<Mat> rvecs;
    std::vector<Mat> tvecs;
    for (int i = 0; i < successes; i++)
    {
        for (int j = 0; j < board_h; j++)
        {
            for (int k = 0; k < board_w; k++)
            {
                Point3f pt;
                pt.x = k * square_size.width;
                pt.y = j * square_size.height;
                pt.z = 0;
                point_grid_pos_buf.push_back(pt);
            }
        }
        point_grid_pos.push_back(point_grid_pos_buf);
        point_grid_pos_buf.clear();
        point_count.push_back(board_h * board_w);
    }
    double temp=calibrateCamera(point_grid_pos, point_pix_pos, img_size, camera_matrix, dist_coeffs, rvecs, tvecs);
    std::cout << temp << std::endl;
    std::cout << camera_matrix << std::endl
              << dist_coeffs << std::endl;
               std::string path="calib2/";
            cv::Mat src = cv::imread(path.append((std::__cxx11::to_string(1).append("_orig.jpg"))));
            cv::Mat showx ;
            int x;

            cv::undistort( src, showx, camera_matrix, dist_coeffs);
            cv:imwrite("showx.jpg",showx);
           
    //if (temp<=minAns) {minAns=temp;
    //std::cout<<"get best in"<<testAns<<" with "<<minAns<<std::endl;}
   // }
    return 0;
}