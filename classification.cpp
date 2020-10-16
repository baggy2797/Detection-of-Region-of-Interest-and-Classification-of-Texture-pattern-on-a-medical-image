//
//  classification.cpp
//  
//
//  Created by Aditya Nitin Bhagwat on 7/25/20.
//

#include<iostream>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <cstdlib>

using namespace std;
using namespace cv;
 
void clear(Mat image)
{
    for(int i=0;i<image.rows;i++)
        for(int j=0;j<image.cols;j++)
            image.at<uchar>(i,j) = 0;
}

void clear_hist(int arr[256])
{
    for(int i=0;i<256;i++)
        arr[i] = 0;
}


void HistDisplay(Mat image)
{
    // allcoate memory for no of pixels for each intensity value
       int histogram[256];
    
       // initialize all intensity values to 0
       for(int i = 0; i < 255; i++)
       {
           histogram[i] = 0;
       }
    
       // calculate the no of pixels for each intensity values
       for(int y = 0; y < image.rows; y++)
           for(int x = 0; x < image.cols; x++)
               histogram[(int)image.at<uchar>(y,x)]++;
    
    
       //for(int i = 0; i < 256; i++)
       //    cout<<histogram[i]<<" ";
    
    
       // draw the histograms
       int hist_w = 512; int hist_h = 400;
       int bin_w = cvRound((double) hist_w/256);
    
       Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));
    
       // find the maximum intensity element from histogram
       int max = histogram[0];
       for(int i = 1; i < 256; i++)
       {
           if(max < histogram[i])
           {
               max = histogram[i];
           }
       }
    
       // normalize the histogram between 0 and histImage.rows
    
       for(int i = 0; i < 255; i++)
       {
           histogram[i] = ((double)histogram[i]/max)*histImage.rows;
       }
    
    
       // draw the intensity line for histogram
       for(int i = 0; i < 255; i++)
       {
           line(histImage, Point(bin_w*(i), hist_h),
                                 Point(bin_w*(i), hist_h - histogram[i]),
                Scalar(0,0,0), 1, 8, 0);
       }
        namedWindow("Intensity Histogram");
       imshow("Intensity Histogram", histImage);
    
       namedWindow("Image");
       imshow("Image", image);
       waitKey(0);
}

/*
void clear_array(int arr[8][8])
{
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            arr[i][j] = 0;
        }
    }
}
*/

int maximum (int a,int b,int c)
{
    if(a>b)
   {
        if(a>c)
      {
          //cout<<a;
          return a;
      }
   }
   else if(b>a)
   {
       if(b>c)
      {
          //cout<<b;
          return b;
      }
   }
   else if(c>b)
   {
       if(c>a)
      {
          //cout<<c;
          return c;
      }
   }
}


int main()
{
    Mat image = imread("Picture4-small_parial.bmp", IMREAD_GRAYSCALE);
    Mat backup = imread("Picture4-small_parial.bmp");
    
    HistDisplay(image);

    Mat c1 = imread("Class1.bmp",IMREAD_GRAYSCALE);
    Mat c2 = imread("Class2.bmp",IMREAD_GRAYSCALE);
    Mat c3 = imread("Class3.bmp",IMREAD_GRAYSCALE);
    
    //Normalizing the Class-1 histogram
    
    int hist_c1[256];
    clear_hist(hist_c1);
    for(int i=0;i<c1.rows;i++)
        for(int j=0;j<c1.cols;j++)
            hist_c1[ int( c1.at<uchar>(i,j) ) ]++;
    
    int max = hist_c1[0];
       for(int i = 1; i < 256; i++)
       {
           if(max < hist_c1[i])
           {
               max = hist_c1[i];
           }
       }
    
       // normalize the histogram between 0 and histImage.rows
    
       for(int i = 0; i < 255; i++)
       {
           hist_c1[i] = ((double)hist_c1[i] / max) * 40;
       }
    
    
    //Normalizing the Class-2 histogram
    
    int hist_c2[256];
    clear_hist(hist_c2);
    for(int i=0;i<c2.rows;i++)
        for(int j=0;j<c2.cols;j++)
            hist_c2[ int( c2.at<uchar>(i,j) ) ]++;
    
    max = hist_c2[0];
       for(int i = 1; i < 256; i++)
       {
           if(max < hist_c2[i])
           {
               max = hist_c2[i];
           }
       }
    
       // normalize the histogram between 0 and histImage.rows
    
       for(int i = 0; i < 255; i++)
       {
           hist_c2[i] = ((double)hist_c2[i] / max) * 40;
       }
    
    
    //Normalizing the Class-3 histogram
    
    int hist_c3[256];
    clear_hist(hist_c3);
    for(int i=0;i<c3.rows;i++)
        for(int j=0;j<c3.cols;j++)
            hist_c3[ int( c3.at<uchar>(i,j) ) ]++;
    
    max = hist_c3[0];
    
       for(int i = 1; i < 256; i++)
       {
           if(max < hist_c3[i])
           {
               max = hist_c3[i];
           }
       }
    
       // normalize the histogram between 0 and histImage.rows
    
       for(int i = 0; i < 255; i++)
       {
           hist_c3[i] = ((double)hist_c3[i]/max) * 40;
       }
    
    
    //HistDisplay(c1);
    //HistDisplay(c2);
    //HistDisplay(c3);
    

    
    int height = image.rows - ( image.rows % 40 );
    int width = image.cols - ( image.cols % 40);
    
    Mat sample;
    
    for(int r = 0 ; r < height ; r = r + 40)
    {
        for(int c = 0; c < width ; c = c + 40)
        {
            int hist_sample[256];
            clear_hist(hist_sample);
            
            
            sample = Mat::zeros(Size(40,40),CV_8U);
            //imshow("Before - Sample :",sample);
            //waitKey(0);
            
            for(int i = r; i < r + 40 ; i++)
            {
                for(int j = c; j < c + 40 ; j++)
                {
                    sample.at<uchar>(abs(r-i),abs(c-j)) = int( image.at<uchar>(i,j) );
                }
            }
        
            //imwrite("Sample.bmp",sample);
            //imshow("Sample :",sample);
            //HistDisplay(sample);
            //waitKey(0);
            
            
            //generated the sample hist of the block
            
            for(int i=0;i<sample.rows;i++)
                for(int j=0;j<sample.cols;j++)
                    hist_sample[ int( sample.at<uchar>(i,j) ) ]++;
            
            //
            
            int maxi_ = 0;
            
            
            for(int i = 1; i < 256; i++)
               {
                   if(maxi_ < hist_sample[i])
                   {
                       maxi_ = hist_sample[i];
                   }
               }
            
               // normalize the histogram between 0 and histImage.rows
            
               for(int i = 0; i < 255; i++)
               {
                   hist_sample[i] = ((double)hist_sample[i]/maxi_) * 40;
               }
            
            
            // class - 1
            int count1 = 0;
            
            for(int i=0;i<256;i++)
            {
                if(hist_sample[i] == hist_c1[i])
                {
                    count1++;
                }
            }
            
            //class - 2
            int count2 = 0;
            
            for(int i=0;i<256;i++)
            {
                if(hist_sample[i] == hist_c2[i])
                {
                    count2++;
                }
            }
            
            //class-3
            int count3 = 0;
            
            for(int i=0;i<256;i++)
            {
                if(hist_sample[i] == hist_c3[i])
                {
                    count3++;
                }
            }
            
            int trial = maximum(count1,count2,count3);
            
            if( trial == count1 && trial > 60 )
            {
                cout<<"\n Match with Class - 1.\n";
                rectangle( backup, Point (r,c), Point (r + height, c + width), Scalar(255,0,0), 1,8,0 );
            }
            else if (trial == count2 && trial > 60 )
            {
                cout<<"\n Match with Class - 2.\n";
                rectangle( backup, Point (r,c), Point (r + height, c+ width), Scalar(0,0,255), 1,8,0 );
            }
            else if ( trial == count3 && trial > 60 )
            {
                cout<<"\n Match with Class - 3.\n";
                rectangle( backup, Point (r,c), Point (r + height, c+ width ), Scalar(0,255,0), 1,8,0 );
            }
            else
            {
                cout<<"\n No Match found.\n";
                if (trial > 60)
                    rectangle( backup, Point (r,c), Point (r + height, c+ width), Scalar(0,0,0), 0,8,0 );
            }
        
            cout<<"\n";
        }
    }


    imshow("Final",backup);
    waitKey(0);
    destroyAllWindows();
    

    cout<<"\n";
    return 0;
}
 
//g++ -std=c++11 classification.cpp -o main -I/usr/local/include/opencv4 -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs
