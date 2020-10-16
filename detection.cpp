//
//  detection.cpp
//  
//
//  Created by Aditya Nitin Bhagwat on 7/22/20.
//
#include <stdio.h>
#include<iostream>
#include<cmath>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <math.h>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

int g[1000][1000] = {0};
int w[1000][1000] = {0};

int dx[4] = {+1, 0, -1, 0};
int dy[4] =  {0, +1, 0, -1};

int component = 0;

int label[1000][1000];

int m[1000][1000];

int row_count;
int col_count;


void clear(Mat image)
{
    for(int i=0;i<image.rows;i++)
        for(int j=0;j<image.cols;j++)
            image.at<uchar>(i,j) = 0;
}


void clear_3(Mat image)
{
    for(int i=0;i<image.rows;i++)
    {
        for(int j=0;j<image.cols;j++)
        {
            image.at<Vec3b>(i,j)[0] = 0;
            image.at<Vec3b>(i,j)[1] = 0;
            image.at<Vec3b>(i,j)[2] = 0;
        }
    }
}

Mat erosionMatrix(int rows, int columns)
{
    Mat erosion = Mat::zeros(rows, columns, CV_8U);
    //fill the entire thing with 255 (this can be changed later for a different effect)
    for (int x = 0; x < erosion.cols; x++)
    {
        for (int y = 0; y < erosion.rows; y++)
        {
            erosion.at<uchar>(y, x) = 255;
        }
    }
    return erosion;
}


//255 for yes, 0 for no (assume center is origin. this implies odd # rows / cols)
Mat dilationMatrix(int rows, int columns)
{
    Mat dilation = Mat::zeros(rows, columns, CV_8U);
    //fill the entire thing with 255 (this can be changed later for a different effect)
    for (int x = 0; x < dilation.cols; x++)
    {
        for (int y = 0; y < dilation.rows; y++)
        {
            dilation.at<uchar>(y, x) = 255;
        }
    }
    return dilation;
}

Mat erosionOrDilation(Mat bin, Mat erosion, bool erode)
{
    Mat erodedMat = bin.clone();
    //iterate through each pixel in the binary image
    for (int x = 0; x < bin.cols; x++)
    {
        for (int y = 0; y < bin.rows; y++)
        {
            if (erode == true)
            {
                int dilationcount = 0;
                for (int i = 0; i < erosion.cols; i++)
                {
                    for (int j = 0; j < erosion.rows; j++)
                    {
                        if (bin.at<uchar>(y + j, x + i) != 255 )
                        {
                            erodedMat.at<uchar>(y, x) = 0;
                        }
                    }
                }
            }
            //if operation is dilation
            else
            {
                if (bin.at<uchar>(y, x) == 255)
                {
                    for (int i = 0; i < erosion.cols; i++)
                    {
                        for (int j = 0; j < erosion.rows; j++)
                        {
                            erodedMat.at<uchar>(y + j, x + i) = 255;
                        }
                    }
                }
            }
        }
    }
    return erodedMat;
}


Mat performErosion(Mat bin, Mat erosion)
{
    return erosionOrDilation(bin, erosion, true);
}

Mat performDilation(Mat bin, Mat dilation)
{
    return erosionOrDilation(bin, dilation, false);
}


void dfs(int x, int y, int current_label)
{
    if (x < 0 || x == row_count)
    {
        return; // out of bounds
    }
    if (y < 0 || y == col_count)
    {
        return; // out of bounds
    }
    if (label[x][y] || !m[x][y])
    {
        return; // already labeled or not marked with 1 in m
    }

    label[x][y] = current_label; // mark the current cell
    // recursively mark the neighbors
    for (int direction = 0; direction < 4; ++direction)
    {
        dfs(x + dx[direction], y + dy[direction], current_label);
    }
}


void find_components()
{
    for (int i = 0; i < row_count; ++i)
    {
        for (int j = 0; j < col_count; ++j)
        {
            if (!label[i][j] && m[i][j])
            {
                dfs(i, j, ++component);
            }
        }
    }
}


int xGradient(Mat image, int x, int y)
{
    return (image.at<uchar>(y-1, x-1) +
            2*image.at<uchar>(y, x-1) +
            image.at<uchar>(y+1, x-1) -
            image.at<uchar>(y-1, x+1) -
            2*image.at<uchar>(y, x+1) -
            image.at<uchar>(y+1, x+1));
}
 

 
int yGradient(Mat image, int x, int y)
{
    return (image.at<uchar>(y-1, x-1) +
            2*image.at<uchar>(y-1, x) +
            image.at<uchar>(y-1, x+1) -
            image.at<uchar>(y+1, x-1) -
            2*image.at<uchar>(y+1, x) -
            image.at<uchar>(y+1, x+1));
}
 
Mat Sobel(Mat src)
{
    Mat dst;
    int gx, gy, sum;
    dst = src.clone();
    
   for(int y = 0; y < src.rows; y++)
   {
       for(int x = 0; x < src.cols; x++)
       {
           dst.at<uchar>(y,x) = 0.0;
       }
   }

   for(int y = 1; y < src.rows - 1; y++)
   {
       for(int x = 1; x < src.cols - 1; x++)
       {
           gx = xGradient(src, x, y);
           gy = yGradient(src, x, y);
           
           sum = abs(gx) + abs(gy);
           sum = sum > 255 ? 255:sum;
           sum = sum < 0 ? 0 : sum;
           
           dst.at<uchar>(y,x) = sum;
       }
   }
    return dst;
}

int main(int argc, char** argv)
{
    Mat src = imread("Picture3-small.bmp");
    namedWindow("Source Image");
    imshow("Source Image",src);
    waitKey(0);
    
    Mat new_src = imread("Picture3-small.bmp",IMREAD_GRAYSCALE);
    
    Mat temp5 = new_src.clone();
    clear(temp5);
    
    for(int i=0;i<src.rows;i++)
    {
        for(int j=0;j<src.cols;j++)
        {
            if(  int(src.at<Vec3b>(i,j)[0]) > 220 && int(src.at<Vec3b>(i,j)[0]) < 250  &&  int(src.at<Vec3b>(i,j)[1]) > 200 && int(src.at<Vec3b>(i,j)[1]) < 230  &&  int(src.at<Vec3b>(i,j)[2]) > 200 && int(src.at<Vec3b>(i,j)[2]) < 240  )
            {
                temp5.at<uchar>(i,j) = 255;
            }
            else
                temp5.at<uchar>(i,j) = 0;
        }
    }
    
    imshow("GRY Channel",temp5);
    waitKey(0);
    
    Mat eroMat = erosionMatrix(6, 6);
    Mat eroded = performErosion(temp5, eroMat);
    
    imshow("After Erosion",eroded);
    waitKey(0);
    
    Mat dilMat = dilationMatrix(8, 8);
    Mat dilated = performDilation(eroded, dilMat);
    
    imshow("After Dilation",dilated);
    waitKey(0);
    imwrite("Dilated.bmp",dilated);
    
    row_count = dilated.rows;
    col_count = dilated.cols;
    
    for (int i=0;i<dilated.rows;i++)
    {
        for(int j=0;j<dilated.cols;j++)
        {
            m[i][j] = (int)dilated.at<uchar>(i,j);
        }
    }
    
    find_components();
    
    cout<<"Total number of abnormal cells detected are :"<<component<<"\n";
    
    Mat final_image = dilated.clone();
    clear(final_image);
    
    final_image = Sobel(dilated);
    
    for(int i=0;i<src.rows;i++)
    {
        for(int j=0;j<src.cols;j++)
        {
            if(int(final_image.at<uchar>(i,j)) == 255)
            {
                src.at<Vec3b>(i,j)[0] = 0;
                src.at<Vec3b>(i,j)[1] = 0;
                src.at<Vec3b>(i,j)[2] = 0;
                
            }
        }
    }
    
    imshow("Output :",src);
    waitKey(0);
    
    return 0;
}
//g++ -std=c++11 detection.cpp -o main -I/usr/local/include/opencv4 -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs
