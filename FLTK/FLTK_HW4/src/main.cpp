#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#define alpha_max 100
using namespace cv;
using namespace std;
string filename1("archie.jpg");
string filename2("betty.jpg");
void on_trackbar(int, void*);
void onMouse_1(int Event, int x, int y, int flags, void* param);
void onMouse_2(int Event, int x, int y, int flags, void* param);
void Button_tri(int event, int x, int y, int flags, void* userdata);
void txt1_point();
void txt2_point();
void morphTriangle(Mat &img1, Mat &img2, Mat &img, vector<Point2f> &t1, vector<Point2f> &t2, vector<Point2f> &t, double alpha);
ofstream in_txt_point("in_txt_point.txt");
ofstream out_txt_point("out_txt_point.txt");
ofstream tri_index("tri_index.txt");
vector<Point2f> points1;
vector<Point2f> points2;
vector<Point2f> points;
Mat img1;
Mat img2;
Mat imgMorph;
Mat image1,image2;
int alpha_slider;
bool flag = false;
double alpha;
// Read points stored in the text files
vector<Point2f> readPoints(string pointsFileName)
{
    vector<Point2f> points;
    ifstream ifs(pointsFileName.c_str());
    float x, y;
    while (ifs >> x >> y)
    {
        points.push_back(Point2f(x, y));
    }

    return points;
}
static void draw_delaunay(Mat& img, Subdiv2D& subdiv)
{

    vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    vector<Point> pt(3);
    Size size = img.size();
    Rect rect(0, 0, size.width, size.height);
    for (size_t i = 0; i < triangleList.size(); i++)
    {
        Vec6f t = triangleList[i];
        pt[0] = Point(cvRound(t[0]), cvRound(t[1]));
        pt[1] = Point(cvRound(t[2]), cvRound(t[3]));
        pt[2] = Point(cvRound(t[4]), cvRound(t[5]));
        // Draw rectangles completely inside the image.
        if (rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2]))
        {
            ifstream ifs("in_txt_point.txt");
            int x, y, it;
            int vex_1, vex_2, vex_3;
            it = vex_1 = vex_2 = vex_3 = 0;
            while (ifs >> x >> y) {
                if (pt[0].x == x && pt[0].y == y)
                    vex_1 = it;
                if (pt[1].x == x && pt[1].y == y)
                    vex_2 = it;
                if (pt[2].x == x && pt[2].y == y)
                    vex_3 = it;
                it++;
            }
            tri_index << vex_1 << " " << vex_2 << " " << vex_3 << endl;
        }
    }
    tri_index.close();
}

// Apply affine transform calculated using srcTri and dstTri to src
void applyAffineTransform(Mat &warpImage, Mat &src, vector<Point2f> &srcTri, vector<Point2f> &dstTri)
{

    // Given a pair of triangles, find the affine transform.
    Mat warpMat = getAffineTransform(srcTri, dstTri);

    // Apply the Affine Transform just found to the src image
    warpAffine(src, warpImage, warpMat, warpImage.size(), INTER_LINEAR, BORDER_REFLECT_101);
}

// Warps and alpha blends triangular regions from img1 and img2 to img
void morphTriangle(Mat &img1, Mat &img2, Mat &img, vector<Point2f> &t1, vector<Point2f> &t2, vector<Point2f> &t, double alpha)
{

    // Find bounding rectangle for each triangle
    Rect r = boundingRect(t);
    Rect r1 = boundingRect(t1);
    Rect r2 = boundingRect(t2);

    // Offset points by left top corner of the respective rectangles
    vector<Point2f> t1Rect, t2Rect, tRect;
    vector<Point> tRectInt;
    for (int i = 0; i < 3; i++)
    {
        tRect.push_back(Point2f(t[i].x - r.x, t[i].y - r.y));
        tRectInt.push_back(Point(t[i].x - r.x, t[i].y - r.y)); // for fillConvexPoly

        t1Rect.push_back(Point2f(t1[i].x - r1.x, t1[i].y - r1.y));
        t2Rect.push_back(Point2f(t2[i].x - r2.x, t2[i].y - r2.y));
    }

    // Get mask by filling triangle
    Mat mask = Mat::zeros(r.height, r.width, CV_32FC3);
    fillConvexPoly(mask, tRectInt, Scalar(1.0, 1.0, 1.0), 16, 0);

    // Apply warpImage to small rectangular patches
    Mat img1Rect, img2Rect;
    img1(r1).copyTo(img1Rect);
    img2(r2).copyTo(img2Rect);

    Mat warpImage1 = Mat::zeros(r.height, r.width, img1Rect.type());
    Mat warpImage2 = Mat::zeros(r.height, r.width, img2Rect.type());

    applyAffineTransform(warpImage1, img1Rect, t1Rect, tRect);
    applyAffineTransform(warpImage2, img2Rect, t2Rect, tRect);

    // Alpha blend rectangular patches
    Mat imgRect = (1.0 - alpha) * warpImage1 + alpha * warpImage2;

    // Copy triangular region of the rectangular patch to the output image
    multiply(imgRect, mask, imgRect);
    multiply(img(r), Scalar(1.0, 1.0, 1.0) - mask, img(r));
    img(r) = img(r) + imgRect;


}
void on_trackbar(int, void*) {
    if (flag == true) {
    points1 = readPoints("in_txt_point.txt");
    points2 = readPoints("out_txt_point.txt");
        for (int i = 0; i < points1.size(); i++)
        {
            float x, y;
            alpha = (double)alpha_slider / alpha_max;
            x = (1 - alpha) * points1[i].x + alpha * points2[i].x;
            y = (1 - alpha) * points1[i].y + alpha * points2[i].y;
            points.push_back(Point2f(x, y));

        }
        ifstream ifs("tri_index.txt");
        int x, y, z;

        while (ifs >> x >> y >> z)
        {
            // Triangles
            vector<Point2f> t1, t2, t;
            // Triangle corners for image 1.
            t1.push_back(points1[x]);
            t1.push_back(points1[y]);
            t1.push_back(points1[z]);

            // Triangle corners for image 2.
            t2.push_back(points2[x]);
            t2.push_back(points2[y]);
            t2.push_back(points2[z]);

            // Triangle corners for morphed image.
            t.push_back(points[x]);
            t.push_back(points[y]);
            t.push_back(points[z]);

            morphTriangle(image1, image2, imgMorph, t1, t2, t, alpha);

        }
    }
    points.clear();
    imshow("Morphed Face", imgMorph / 255.0);
}
void onMouse_1(int Event, int x, int y, int flags, void* param) {
    if (Event == CV_EVENT_LBUTTONDOWN && flag == false) {
        cout << x << "," << y << endl;
        circle(img1, Point(x, y), 2, Scalar(0, 255, 0), 3, 10);
        in_txt_point << to_string(x) << " " << to_string(y) << endl;
        imshow("Initial Face", img1 / 255.0);
    }
    if(Event == CV_EVENT_MBUTTONDOWN && flag == false){
        in_txt_point.close();
        in_txt_point.open("in_txt_point.txt", std::ios::out | std::ios::trunc);
        txt1_point();
        image1.copyTo(img1);
        imshow("Initial Face", img1 / 255.0);
    }
}
void onMouse_2(int Event, int x, int y, int flags, void* param) {
    if (Event == CV_EVENT_LBUTTONDOWN && flag == false) {
        cout << x << "," << y << endl;
        circle(img2, Point(x, y), 2, Scalar(0, 0, 255), 3, 10);
        out_txt_point << to_string(x) << " " << to_string(y) << endl;
        imshow("Result Face", img2 / 255.0);
    }
    if(Event == CV_EVENT_MBUTTONDOWN && flag == false){
        out_txt_point.close();
        out_txt_point.open("out_txt_point.txt", std::ios::out | std::ios::trunc);
        txt2_point();
        image2.copyTo(img2);
        imshow("Result Face", img2 / 255.0);
    }
}

void txt1_point() {
    in_txt_point << 1 << " " << 1 << endl;
    for (int i = 99; i < img1.rows; i += 100)
        in_txt_point << 1 << " " << i << endl;
    for (int i = 99; i < img1.cols; i += 100)
        in_txt_point << i << " " << img1.rows - 1 << endl;
    for (int i = img1.rows - 1; i > 0; i -= 100)
        in_txt_point << img1.cols - 1 << " " << i << endl;
    for (int i = img1.cols - 1; i > 0; i -= 100)
        in_txt_point << i << " " << 1 << endl;
}
void txt2_point(){
    out_txt_point << 1 << " " << 1 << endl;
    for (int i = 99; i < img1.rows; i += 100)
        out_txt_point << 1 << " " << i << endl;
    for (int i = 99; i < img1.cols; i += 100)
        out_txt_point << i << " " << img1.rows - 1 << endl;
    for (int i = img1.rows - 1; i > 0; i -= 100)
        out_txt_point << img1.cols - 1 << " " << i << endl;
    for (int i = img1.cols - 1; i > 0; i -= 100)
        out_txt_point << i << " " << 1 << endl;
}

void Button_tri(int event, int x, int y, int flags, void* userdata) {
    if (event == EVENT_LBUTTONDOWN){
        flag = true;
        points1 = readPoints("in_txt_point.txt");
        Size size = img1.size();
        Rect rect(0, 0, size.width, size.height);
        Subdiv2D subdiv(rect);
        // Create an instance of Subdiv2D
        for (vector<Point2f>::iterator it = points1.begin(); it != points1.end(); it++)
            subdiv.insert(*it);
        draw_delaunay(img1, subdiv);
    }
    if(event == EVENT_MBUTTONDOWN && flag == true){
        Size size = img1.size();
        VideoWriter writer("Morphing.avi",CV_FOURCC('M', 'J', 'P', 'G'),10,size);
        int ti = 0;
        while(ti < 150){
            points1 = readPoints("in_txt_point.txt");
            points2 = readPoints("out_txt_point.txt");
                for (int i = 0; i < points1.size(); i++)
                {
                    float x, y;
                    alpha = (double)ti / 150;
                    x = (1 - alpha) * points1[i].x + alpha * points2[i].x;
                    y = (1 - alpha) * points1[i].y + alpha * points2[i].y;
                    points.push_back(Point2f(x, y));

                }
                ifstream ifs("tri_index.txt");
                int x, y, z;
                while (ifs >> x >> y >> z)
                {
                    // Triangles
                    vector<Point2f> t1, t2, t;
                    // Triangle corners for image 1.
                    t1.push_back(points1[x]);
                    t1.push_back(points1[y]);
                    t1.push_back(points1[z]);

                    // Triangle corners for image 2.
                    t2.push_back(points2[x]);
                    t2.push_back(points2[y]);
                    t2.push_back(points2[z]);

                    // Triangle corners for morphed image.
                    t.push_back(points[x]);
                    t.push_back(points[y]);
                    t.push_back(points[z]);

                    morphTriangle(image1, image2, imgMorph, t1, t2, t, alpha);
                }
            ti++;
            points.clear();
           string name = "./JPG/" + to_string(ti) + ".jpg";
           imwrite(name,imgMorph);
        }
        ti = 0;
        Mat image;
        while (true){
            VideoCapture in_capture("./JPG/" + to_string(ti) + ".jpg") ;
            in_capture >> image;
            if(image.empty())
                break;
            writer.write(image);
            ti++;
          }
        writer.release();
    }
}
int main(int argc, char** argv)
{
    alpha_slider = 0;
    //Read input images
    img1 = imread(argv[1]);
    img2 = imread(argv[2]);
    points1 = readPoints("in_txt_point.txt");
    points2 = readPoints("out_txt_point.txt");
    Mat Button(50, 250, CV_8UC1);
    for(MatIterator_<uchar> ite= Button.begin<uchar>(); ite != Button.end<uchar>(); ite ++)
        (*ite) = 0;
    putText(Button, String("Save point"), Point(30, 30), 0, 1, Scalar(255, 255, 255), 3);
    txt1_point();
    txt2_point();
    //convert Mat to float data type
    img1.convertTo(img1, CV_32F);
    img2.convertTo(img2, CV_32F);
    img1.copyTo(image1);img2.copyTo(image2);
    imgMorph = Mat::zeros(img1.size(), CV_32FC3);
    //Read points
    // Display Result
    namedWindow("Morphed Face", 1);
    namedWindow("Result Face", 1);
    namedWindow("Button_trigle");
    moveWindow("Result Face", img1.rows*2, 0);
    moveWindow("Morphed Face", img1.rows-125, 0);
    imshow("Result Face", img2 / 255.0);
    imshow("Initial Face", img1 / 255.0);
    imshow("Button_trigle", Button);
    moveWindow("Button_trigle", img1.rows,img1.cols+300);

    setMouseCallback("Initial Face", onMouse_1, NULL);
    setMouseCallback("Result Face", onMouse_2, NULL);
    setMouseCallback("Button_trigle", Button_tri, NULL);

    createTrackbar("Trackbar", "Morphed Face", &alpha_slider, alpha_max, on_trackbar);
    on_trackbar(alpha_slider, 0);

    waitKey(0);
    in_txt_point.close();
    out_txt_point.close();
    destroyAllWindows();
    return 0;
}

