#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "main.h"
#include <math.h>

#include "opencv2/opencv.hpp"
#include <opencv2/core.hpp>

using namespace std;
using namespace cv;


void original(Mat image, BITMAPINFOHEADER bmiHeader, bgrQUAD** bgr);
void grayscale(Mat image, BITMAPINFOHEADER bmiHeader, bgrQUAD** bgr);
void binary(Mat image, BITMAPINFOHEADER bmiHeader, bgrQUAD** bgr);
void negativeGray(Mat image, BITMAPINFOHEADER bmiHeader, bgrQUAD** bgr);
void negativeColor(Mat image, BITMAPINFOHEADER bmiHeader, bgrQUAD** bgr);
void luminance(Mat image, BITMAPINFOHEADER bmiHeader, bgrQUAD** bgr, int b);
void contrast(Mat image, BITMAPINFOHEADER bmiHeader, bgrQUAD** bgr);
void gammaCorrection(Mat image, BITMAPINFOHEADER bmiHeader, bgrQUAD** bgr);

void showHist(Mat image, BITMAPINFOHEADER bmiHeader, bgrQUAD** bgr);
void showHistRGB(Mat image, BITMAPINFOHEADER bmiHeader, bgrQUAD** bgr);


int main()
{
    setlocale(LC_ALL, "rus");

    string imageName = "C:\\Users\\kucep\\OneDrive - volsu.ru\\������� ����\\������\\������������\\3 ���� 2 �������\\���������� � ������������ �������\\lab5\\kras.bmp";
    const char* imageNameFile = "C:\\Users\\kucep\\OneDrive - volsu.ru\\������� ����\\������\\������������\\3 ���� 2 �������\\���������� � ������������ �������\\lab5\\kras.bmp";

    FILE* pFile;
    fopen_s(&pFile, imageNameFile, "rb");

    // ��������� ��������� �����
    BITMAPFILEHEADER header;
    header.bfType[0]    = getc(pFile);
    header.bfType[1]    = getc(pFile);
    header.bfSize       = read_u32(pFile);
    header.bfReserved1  = read_u16(pFile);
    header.bfReserved2  = read_u16(pFile);
    header.bfOffBits    = read_u32(pFile);

    // ��������� ��������� �����������
    BITMAPINFOHEADER bmiHeader;
    bmiHeader.biSize            = read_u32(pFile);
    bmiHeader.biWidth           = read_s32(pFile);
    bmiHeader.biHeight          = read_s32(pFile);
    bmiHeader.biPlanes          = read_u16(pFile);
    bmiHeader.biBitCount        = read_u16(pFile);
    bmiHeader.biCompression     = read_u32(pFile);
    bmiHeader.biSizeImage       = read_u32(pFile);
    bmiHeader.biXPelsPerMeter   = read_s32(pFile);
    bmiHeader.biYPelsPerMeter   = read_s32(pFile);
    bmiHeader.biClrUsed         = read_u32(pFile);
    bmiHeader.biClrImportant    = read_u32(pFile);

    if (bmiHeader.biBitCount <= 8)
    {
        
        const int colorsCount = pow(2, bmiHeader.biBitCount);

        // ��������� ������� �� 2^bitCount ������
        bgrQUAD* bgrI = new bgrQUAD[colorsCount];
        for (int i = 0; i < colorsCount; i++)
        {
            bgrI[i].blue = getc(pFile);
            bgrI[i].green = getc(pFile);
            bgrI[i].red = getc(pFile);
            // ���������� ��������� ����
            getc(pFile);

        }

        // ������ ������ ��� �������� �������� ��������� ������
        int** colorIndex = new int* [bmiHeader.biHeight];
        for (int i = 0; i < bmiHeader.biHeight; i++) {
            colorIndex[i] = new int[bmiHeader.biWidth];
        }

        // ��������� ������ ��������� ��������� ������
        for (int i = 0; i < bmiHeader.biHeight; i++)
        {
            // ������ ������
            for (int j = 0; j < bmiHeader.biWidth; j++)
            {
                colorIndex[i][j] = getc(pFile);
            }
            // ���������� ������ ����� � ����� ������
            getc(pFile);
            getc(pFile);
            getc(pFile);
        }

        // ������ � ������� ������������ �����������
        Mat image = imread(imageName, IMREAD_COLOR);
        if (image.empty())
        {
            cout << "\nImage cannot be loaded..!!" << endl;
            return -1;
        }
        imshow("ORIGINAL IMAGE", image);

        // ������ �������������� �����������
        Mat res(image.size(), CV_8UC3, Scalar(255, 255, 255));


        for (int i = 0; i < bmiHeader.biHeight; i++)
        {
            for (int j = 0; j < bmiHeader.biWidth; j++)
            {
                // � ������� ����� ��������� bgr-����������
                uchar b = 255;
                uchar g = 255;
                uchar r = 255;
                // ���������� ���� ������� �� ��� �������
                for (int k = 0; k < colorsCount; k++)
                {
                    if (colorIndex[i][j] == k)
                    {
                        b = bgrI[k].blue;
                        g = bgrI[k].green;
                        r = bgrI[k].red;
                    }
                }
                // ������������ ����������� ����������� � ������� �-�� at()
                res.at<Vec3b>(i, j)[0] = b;
                res.at<Vec3b>(i, j)[1] = g;
                res.at<Vec3b>(i, j)[2] = r;
            }
        }

        // �������������� �����������, �.�. � opencv ��� �������� ��-�������. ��� 0 - ������� �� �������.
        flip(res, res, 0);
        imshow("RESULT IMAGE", res);
        imwrite("C:\\Users\\kucep\\OneDrive - volsu.ru\\������� ����\\������\\������������\\3 ���� 2 �������\\���������� � ������������ �������\\lab5\\res.bmp", res);

        waitKey(0); //wait for key press
        destroyAllWindows(); //destroy all open windows

    }
    else if (bmiHeader.biBitCount > 8)
    {
        // ������ ������ ������
        bgrQUAD** bgr = new bgrQUAD * [bmiHeader.biHeight];
        for (int i = 0; i < bmiHeader.biHeight; i++) {
            bgr[i] = new bgrQUAD[bmiHeader.biWidth];
        }
        // ��������� �����
        for (int i = 0; i < bmiHeader.biHeight; i++) {
            for (int j = 0; j < bmiHeader.biWidth; j++) {
                bgr[i][j].blue = getc(pFile);
                bgr[i][j].green = getc(pFile);
                bgr[i][j].red = getc(pFile);
                bgr[i][j].alpha = getc(pFile);
            }
        }


        Mat image = imread(imageName, IMREAD_COLOR);
        if (image.empty())
        {
            cout << "\nImage cannot be loaded..!!" << endl;
            return -1;
        }

        int choise;
        while (true)
        {
            cout << "������� ������� -- ��� ����������� --:" << endl;
            cout << "\t1. �������� ���������� � �����������" << endl;
            cout << "\t2. ����������� (�������� ������) � �����������" << endl;
            cout << "\t3. �����������" << endl;
            cout << "\t4. ���������� �������������� (����)" << endl;
            cout << "\t5. ���������� �������������� (�����)" << endl;
            cout << "\t6. �����������" << endl;
            cout << "\t7. ����������" << endl;
            cout << "\t8. ���������� ���������" << endl;
            cout << "\t9. ���������� ������� (�����-���������)" << endl;

            cin >> choise;
            cout << endl;

            // ��������� �����
            switch (choise)
            {
            case 1:
                original(image, bmiHeader, bgr);
                break;
            case 2:
                
                grayscale(image, bmiHeader, bgr);
                break;
            case 3:
               
                binary(image, bmiHeader, bgr);
                break;
            case 4:
                original(image, bmiHeader, bgr);
                negativeColor(image, bmiHeader, bgr);
                break;
            case 5:
                
                negativeGray(image, bmiHeader, bgr);
                break;
            case 6:
                original(image, bmiHeader, bgr);
                luminance(image, bmiHeader, bgr, 100);
                break;
            case 7:
                original(image, bmiHeader, bgr);
                luminance(image, bmiHeader, bgr, -100);
                break;
            case 8:
                original(image, bmiHeader, bgr);
                contrast(image, bmiHeader, bgr);
                break;
            case 9:
                original(image, bmiHeader, bgr);
                gammaCorrection(image, bmiHeader, bgr);
                break;
            default:
                return 0;
            }
            waitKey(0); //wait for key press
            destroyAllWindows(); //destroy all open windows
        }
    }

    fclose(pFile);
    return 0;
}


// ����� ������������� �����������
void original(Mat image, BITMAPINFOHEADER bmiHeader, bgrQUAD** bgr)
{
    Mat res(image.size(), CV_8UC4, Scalar(255, 255, 255));
    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        for (int j = 0; j < bmiHeader.biWidth; j++)
        {
            res.at<Vec4b>(i, j)[0] = bgr[i][j].blue;
            res.at<Vec4b>(i, j)[1] = bgr[i][j].green;
            res.at<Vec4b>(i, j)[2] = bgr[i][j].red;
            res.at<Vec4b>(i, j)[3] = bgr[i][j].alpha;
        }
    }
    flip(res, res, 0);
    imshow("�������� �����������", res);
    imwrite("C:\\Users\\kucep\\OneDrive - volsu.ru\\������� ����\\������\\������������\\3 ���� 2 �������\\���������� � ������������ �������\\lab5\\res.bmp", res);

    showHistRGB(image, bmiHeader, bgr);

}


// ������� (�������� ������) � ����� �� �����
void grayscale(Mat image, BITMAPINFOHEADER bmiHeader, bgrQUAD** bgr)
{
    // ������ ����������� �����������
    Mat grayscale(image.size(), CV_8UC4, Scalar(255, 255, 255));
    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        for (int j = 0; j < bmiHeader.biWidth; j++)
        {
            int I = 0.11 * bgr[i][j].blue + 0.59 * bgr[i][j].green + 0.30 * bgr[i][j].red;

            grayscale.at<Vec4b>(i, j)[0] = I;
            grayscale.at<Vec4b>(i, j)[1] = I;
            grayscale.at<Vec4b>(i, j)[2] = I;
            grayscale.at<Vec4b>(i, j)[3] = I;
        }
    }
    flip(grayscale, grayscale, 0);
    imshow("����������� �����������", grayscale);
    imwrite("C:\\Users\\kucep\\OneDrive - volsu.ru\\������� ����\\������\\������������\\3 ���� 2 �������\\���������� � ������������ �������\\lab5\\grayscale.bmp", grayscale);

    showHist(grayscale, bmiHeader, bgr);
}


// ����������� � ����� �� �����
void binary(Mat image, BITMAPINFOHEADER bmiHeader, bgrQUAD** bgr)
{
    int pBinary = 48;
    // ����������� (�����)
    Mat binaryGray = image.clone();
    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        for (int j = 0; j < bmiHeader.biWidth; j++)
        {
            int I = 0.11 * bgr[i][j].blue + 0.59 * bgr[i][j].green + 0.30 * bgr[i][j].red;

            if (I < pBinary)
            {
                binaryGray.at<Vec3b>(i, j)[0] = 0;
                binaryGray.at<Vec3b>(i, j)[1] = 0;
                binaryGray.at<Vec3b>(i, j)[2] = 0;
            }
            else
            {
                binaryGray.at<Vec3b>(i, j)[0] = 255;
                binaryGray.at<Vec3b>(i, j)[1] = 255;
                binaryGray.at<Vec3b>(i, j)[2] = 255;
            }
        }
    }
    flip(binaryGray, binaryGray, 0);
    imshow("���������������� ����� �����������", binaryGray);
    imwrite("C:\\Users\\kucep\\OneDrive - volsu.ru\\������� ����\\������\\������������\\3 ���� 2 �������\\���������� � ������������ �������\\lab5\\binary.bmp", binaryGray);

    

}


// �������������� �������� � ������� � ����� �� �����
void negativeGray(Mat image, BITMAPINFOHEADER bmiHeader, bgrQUAD** bgr)
{
    int pNegative = 1;
    // ������� (�����)
    Mat negativeGray = image.clone();
    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        for (int j = 0; j < bmiHeader.biWidth; j++)
        {
            int I = 0.11 * bgr[i][j].blue + 0.59 * bgr[i][j].green + 0.30 * bgr[i][j].red;

            if (255 - I < pNegative)
            {
                negativeGray.at<Vec3b>(i, j)[0] = I;
                negativeGray.at<Vec3b>(i, j)[1] = I;
                negativeGray.at<Vec3b>(i, j)[2] = I;
            }
            else
            {
                negativeGray.at<Vec3b>(i, j)[0] = 255 - I;
                negativeGray.at<Vec3b>(i, j)[1] = 255 - I;
                negativeGray.at<Vec3b>(i, j)[2] = 255 - I;
            }
        }
    }
    flip(negativeGray, negativeGray, 0);
    imshow("���������� ����� �����������", negativeGray);
    imwrite("C:\\Users\\kucep\\OneDrive - volsu.ru\\������� ����\\������\\������������\\3 ���� 2 �������\\���������� � ������������ �������\\lab5\\negativeGray.bmp", negativeGray);
    
    showHist(negativeGray, bmiHeader, bgr);
}


// ���������� � ����������� ������������� � ����� �� �����
void negativeColor(Mat image, BITMAPINFOHEADER bmiHeader, bgrQUAD** bgr)
{
    int pNegative = 1;
    // ������� (����)
    Mat negative = image.clone();
    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        for (int j = 0; j < bmiHeader.biWidth; j++)
        {
            // �����
            if (255 - bgr[i][j].blue < pNegative)
            {
                negative.at<Vec3b>(i, j)[0] = bgr[i][j].blue;
            }
            else    negative.at<Vec3b>(i, j)[0] = 255 - bgr[i][j].blue;
            // ������
            if (255 - bgr[i][j].green < pNegative)
            {
                negative.at<Vec3b>(i, j)[1] = bgr[i][j].green;
            }
            else    negative.at<Vec3b>(i, j)[1] = 255 - bgr[i][j].green;
            // �������
            if (bgr[i][j].red < pNegative)
            {
                negative.at<Vec3b>(i, j)[2] = bgr[i][j].red;
            }
            else    negative.at<Vec3b>(i, j)[2] = 255 - bgr[i][j].red;
        }
    }
    flip(negative, negative, 0);
    imshow("���������� ������� �����������", negative);
    imwrite("C:\\Users\\kucep\\OneDrive - volsu.ru\\������� ����\\������\\������������\\3 ���� 2 �������\\���������� � ������������ �������\\lab5\\negative.bmp", negative);

    showHistRGB(negative, bmiHeader, bgr);
}


// ��������� ������� � ����� �� �����
void gammaCorrection(Mat image, BITMAPINFOHEADER bmiHeader, bgrQUAD** bgr)
{
    // �����-���������
    double gamma = 0.5;

    Mat gammaCorrection = image.clone();
    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        for (int j = 0; j < bmiHeader.biWidth; j++)
        {
            gammaCorrection.at<Vec3b>(i, j)[0] = 255.0 * pow((bgr[i][j].blue / 255.0), gamma);
            gammaCorrection.at<Vec3b>(i, j)[1] = 255.0 * pow((bgr[i][j].green / 255.0), gamma);
            gammaCorrection.at<Vec3b>(i, j)[2] = 255.0 * pow((bgr[i][j].red / 255.0), gamma);
        }
    }
    flip(gammaCorrection, gammaCorrection, 0);
    imshow("�����-��������� �����������", gammaCorrection);
    imwrite("C:\\Users\\kucep\\OneDrive - volsu.ru\\������� ����\\������\\������������\\3 ���� 2 �������\\���������� � ������������ �������\\lab5\\gammaCorrection.bmp", gammaCorrection);

    showHistRGB(gammaCorrection, bmiHeader, bgr);

}


// ��������� ������� � ����� �� �����
void luminance(Mat image, BITMAPINFOHEADER bmiHeader, bgrQUAD** bgr, int b)
{
    Mat luminance(image.size(), CV_8UC3, Scalar(255, 255, 255));
    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        for (int j = 0; j < bmiHeader.biWidth; j++)
        {

            luminance.at<Vec3b>(i, j)[0] = (bgr[i][j].blue + b) < 255 ? ((bgr[i][j].blue + b) > 0 ? (bgr[i][j].blue + b) : 0) : 255;
            luminance.at<Vec3b>(i, j)[1] = bgr[i][j].green + b < 255 ? ((bgr[i][j].green + b) > 0 ? (bgr[i][j].green + b) : 0) : 255;
            luminance.at<Vec3b>(i, j)[2] = bgr[i][j].red + b < 255 ? ((bgr[i][j].red + b) > 0 ? (bgr[i][j].red + b) : 0) : 255;
        }
    }
    flip(luminance, luminance, 0);
    imshow("��������� �������", luminance);
    imwrite("C:\\Users\\kucep\\OneDrive - volsu.ru\\������� ����\\������\\������������\\3 ���� 2 �������\\���������� � ������������ �������\\lab5\\luminance.bmp", luminance);

    showHistRGB(luminance, bmiHeader, bgr);

}


// ��������� ��������� � ����� �� �����
void contrast(Mat image, BITMAPINFOHEADER bmiHeader, bgrQUAD** bgr)
{
    int q1 = 50, q2 = 250;
    Mat contrast(image.size(), CV_8UC3, Scalar(255, 255, 255));
    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        for (int j = 0; j < bmiHeader.biWidth; j++)
        {
            int b = bgr[i][j].blue;
            int g = bgr[i][j].green;
            int r = bgr[i][j].red;

            if ((bgr[i][j].blue - q1) * (255 / (q2 - q1)) < 0) b = 0;
            else b = (bgr[i][j].blue - q1) * (255 / (q2 - q1));

            if ((bgr[i][j].red - q1) * (255 / (q2 - q1)) < 0) r = 0;
            else r = (bgr[i][j].red - q1) * (255 / (q2 - q1));

            if ((bgr[i][j].green - q1) * (255 / (q2 - q1)) < 0) g = 0;
            else g = (bgr[i][j].green - q1) * (255 / (q2 - q1));

            contrast.at<Vec3b>(i, j)[0] = b;
            contrast.at<Vec3b>(i, j)[1] = g;
            contrast.at<Vec3b>(i, j)[2] = r;
        }
    }
    flip(contrast, contrast, 0);
    imshow("��������� ���������", contrast);
    imwrite("C:\\Users\\kucep\\OneDrive - volsu.ru\\������� ����\\������\\������������\\3 ���� 2 �������\\���������� � ������������ �������\\lab5\\contrast.bmp", contrast);

    showHistRGB(contrast, bmiHeader, bgr);

}


/// ������� � ����� �� ����� ����������� ��� ��� ������� - R, G, B
void showHist(Mat image, BITMAPINFOHEADER bmiHeader, bgrQUAD** bgr)
{
    int* H = new int[256];

    int** I = new int* [bmiHeader.biHeight];
    for (int i = 0; i < bmiHeader.biHeight; i++) {
        I[i] = new int[bmiHeader.biWidth];
    }

    int max = 0;
    for (int k = 0; k < 256; k++)
    {
        H[k] = 0;
        for (int i = 0; i < bmiHeader.biHeight; i++)
        {
            for (int j = 0; j < bmiHeader.biWidth; j++)
            {
                int b = image.at<Vec3b>(i, j)[0];
                int g = image.at<Vec3b>(i, j)[1];
                int r = image.at<Vec3b>(i, j)[2];

                I[i][j] = 0.11 * b + 0.59 * g + 0.30 * r;

                if (I[i][j] == k)
                {
                    H[k]++;
                }
            }
        }
        if (max < H[k])     max = H[k] + 1;
    }

    Mat src(max, 256, CV_8UC3);
    for (int i = 0; i < max; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            if (i == H[j])
            {
                for (int k = i; k > 0; k--)
                {
                    src.at<Vec3b>(k, j)[0] = 255;
                    src.at<Vec3b>(k, j)[1] = 255;
                    src.at<Vec3b>(k, j)[2] = 255;
                }
            }
        }
    }
    resize(src, src, { 400, 256 }, 0, 0);
    flip(src, src, 0);
    imshow("����������", src);

    delete[] H;
}


// ������� � ����� �� ����� ����������� ��� ��� ������� - R, G, B
void showHistRGB(Mat image, BITMAPINFOHEADER bmiHeader, bgrQUAD** bgr)
{
    int* HBlue = new int[256];
    int* HGreen = new int[256];
    int* HRed = new int[256];

    int** I = new int* [bmiHeader.biHeight];
    for (int i = 0; i < bmiHeader.biHeight; i++) {
        I[i] = new int[bmiHeader.biWidth];
    }

    int max = 0;
    for (int k = 0; k < 256; k++)
    {
        HBlue[k] = 0;
        HGreen[k] = 0;
        HRed[k] = 0;

        for (int i = 0; i < bmiHeader.biHeight; i++)
        {
            for (int j = 0; j < bmiHeader.biWidth; j++)
            {
                HBlue[k] += image.at<Vec3b>(i, j)[0] == k ? 1 : 0;
                HGreen[k] += image.at<Vec3b>(i, j)[1] == k ? 1 : 0;
                HRed[k] += image.at<Vec3b>(i, j)[2] == k ? 1 : 0;

            }
        }
        max = HBlue[k] > max ? HBlue[k] : max;
        max = HGreen[k] > max ? HGreen[k] : max;
        max = HRed[k] > max ? HRed[k] : max;

    }
    max++;


    Mat srcB(max, 256, CV_8UC3);
    Mat srcG(max, 256, CV_8UC3);
    Mat srcR(max, 256, CV_8UC3);
    for (int i = 0; i < max; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            if (i == HBlue[j])
            {
                for (int k = i; k > 0; k--)
                {
                    srcB.at<Vec3b>(k, j)[0] = 255;
                    srcB.at<Vec3b>(k, j)[1] = 0;
                    srcB.at<Vec3b>(k, j)[2] = 0;
                }
            }
            else if (i == HGreen[j])
            {
                for (int k = i; k > 0; k--)
                {
                    srcG.at<Vec3b>(k, j)[0] = 0;
                    srcG.at<Vec3b>(k, j)[1] = 255;
                    srcG.at<Vec3b>(k, j)[2] = 0;
                }
            }
            else if (i == HRed[j])
            {
                for (int k = i; k > 0; k--)
                {
                    srcR.at<Vec3b>(k, j)[0] = 0;
                    srcR.at<Vec3b>(k, j)[1] = 0;
                    srcR.at<Vec3b>(k, j)[2] = 255;
                }
            }

        }
    }
    resize(srcB, srcB, { 400, 256 }, 0, 0);
    flip(srcB, srcB, 0);
    imshow("����������� blue", srcB);
    
    resize(srcG, srcG, { 400, 256 }, 0, 0);
    flip(srcG, srcG, 0);
    imshow("����������� green", srcG);
    
    resize(srcR, srcR, { 400, 256 }, 0, 0);
    flip(srcR, srcR, 0);
    imshow("����������� red", srcR);

    delete[] HBlue;
    delete[] HGreen;
    delete[] HRed;

}