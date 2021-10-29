#include <iostream>
#include <Eigen/Dense>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/core/eigen.hpp>

using namespace cv;
using namespace std;
using namespace Eigen;

const int width = 2208;
const int height = 1242;
bool in(int x, int l, int r)
{
    return ((x >= l) && (x <= r));
}
pair<int, int> location[width][height];
int main()
{
    //cout << "0" << endl;
    int maxX, minX;
    int maxY, minY;
    maxX = 0;
    maxY = 0;
    minX = 1000000;
    minY = 1000000;
    Mat depth = imread("data pack/1_dpt.tiff", IMREAD_ANYDEPTH);
    Mat img = imread("data pack/1.jpg");
    FileStorage reader("data pack/data.yml", FileStorage::READ);
    Mat c_mat, p_mat;
    Matrix<double, 3, 3> C;
    Matrix<double, 4, 4> P;
    //cout<<img.type()<<endl;//16
    //cout<<depth.type()<<endl;//0
    //cout << "1" << endl;

    reader["C"] >> c_mat;
    reader["D"] >> p_mat;
    cv2eigen(c_mat, C);
    cv2eigen(p_mat, P);
    cout << P << endl;
    int xa = 0;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            //cout<<"3"<<endl;
            Matrix<double, 3, 1> pictureL;
            pictureL << i, j, 1;
            Matrix<double, 3, 1> worldL;
            //cout<<depth<<endl;
            int z = depth.at<float>(j, i);
            worldL = z * (C.inverse()) * pictureL; //
            //cout<<worldL<<endl;
            Matrix<double, 4, 1> worldL2;
            worldL2(0, 0) = worldL(0, 0);
            worldL2(1, 0) = worldL(1, 0);
            worldL2(2, 0) = worldL(2, 0);
            worldL2(3, 0) = 1;
            Matrix<double, 4, 1> worldLc;
            worldLc = P * worldL2;
            Matrix<double, 3, 1> worldL3;
            worldL3(0, 0) = worldLc(0, 0) / worldLc(2, 0);
            worldL3(1, 0) = worldLc(1, 0) / worldLc(2, 0);
            worldL3(2, 0) = 1;
            Matrix<double, 3, 1> pictureLc;
            pictureLc = C * worldL3;
            //cout<<pictureLc<<endl;
            int x = pictureLc(0, 0);
            int y = pictureLc(1, 0);
            location[i][j] = make_pair(x, y);
            maxX = max(maxX, x);
            maxY = max(maxY, y);
            minX = min(minX, x);
            minY = min(minY, y);
        }
    }
    maxX = min(maxX, 4000);
    maxY = min(maxY, 4000);
    minX = max(minX, 0);
    minY = max(minY, 0);
    cout << maxX << " " << maxY << " " << minX << " " << minY << endl;
    Mat ans = Mat(maxX - minX, maxY - minY, img.type());
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            int x = location[i][j].first;
            int y = location[i][j].second;
            //cout<<x<<y<<endl;
            if (in(x, minX, maxX) && in(y, minY, maxY))
            {
                //cout<<"1"<<endl;
                ans.at<cv::Vec3b>(y - minY, x - minX) = img.at<cv::Vec3b>(j, i);
                xa++;
            }
        }
    }
    cout << xa << endl;
    //ans.resize(500);
    imshow("1", ans);
    waitKey(0);
    return 0;
}