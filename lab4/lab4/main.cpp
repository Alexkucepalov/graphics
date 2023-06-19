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

int main()
{
    setlocale(LC_ALL, "rus");

    string imageName = "C:\\Users\\kucep\\OneDrive - volsu.ru\\Рабочий стол\\Универ\\Лабараторные\\3 курс 2 семестр\\Инженерная и компьютерная графика\\lab4\\kras.bmp";

    FILE* pFile;
    fopen_s(&pFile, "C:\\Users\\kucep\\OneDrive - volsu.ru\\Рабочий стол\\Универ\\Лабараторные\\3 курс 2 семестр\\Инженерная и компьютерная графика\\lab4\\kras.bmp", "rb");

    // считываем заголовок файла
    BITMAPFILEHEADER header;
    header.bfType[0]    = getc(pFile);
    header.bfType[1]    = getc(pFile);
    header.bfSize       = read_u32(pFile);
    header.bfReserved1  = read_u16(pFile);
    header.bfReserved2  = read_u16(pFile);
    header.bfOffBits    = read_u32(pFile);

    // считываем заголовок изображения
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

    if (bmiHeader.biXPelsPerMeter == 0 or bmiHeader.biYPelsPerMeter == 0)
    {
        double pysicalW = bmiHeader.biWidth / 96;
        double pysicalH = bmiHeader.biHeight / 96;
        bmiHeader.biXPelsPerMeter = int(bmiHeader.biWidth * 254 / (pysicalW * 25.4));
        bmiHeader.biYPelsPerMeter = int(bmiHeader.biHeight * 254 / (pysicalH * 25.4));
    }

    if (bmiHeader.biSizeImage == 0)
    {
        bmiHeader.biSizeImage = ceil(bmiHeader.biWidth * 
            bmiHeader.biHeight * bmiHeader.biBitCount / 8);
    }

    // выводим результат
    printf("struct BITMAPFILEHEADER {\n");
    printf("\tbfType = %c%c\n", header.bfType[0], header.bfType[1]);
    printf("\tbfSize = %u\n", header.bfSize);
    printf("\tbfReserved1 = %u\n", header.bfReserved1);
    printf("\tbfReserved2 = %u\n", header.bfReserved2);
    printf("\tbfOffBits = %u\n", header.bfOffBits);
    printf("}");
    printf("\n\n");

    printf("struct BITMAPINFOHEADER {\n");
    printf("\tbiSize = %u\n", bmiHeader.biSize);
    printf("\tbiWidth = %d\n", bmiHeader.biWidth);
    printf("\tbiHeight = %d\n", bmiHeader.biHeight);
    printf("\tbiPlanes = %u\n", bmiHeader.biPlanes);
    printf("\tbiBitCount = %u\n", bmiHeader.biBitCount);
    printf("\tbiCompression = %u\n", bmiHeader.biCompression);
    printf("\tbiSizeImage = %u\n", bmiHeader.biSizeImage);
    printf("\tbiXPelsPerMeter = %d\n", bmiHeader.biXPelsPerMeter);
    printf("\tbiYPelsPerMeter = %d\n", bmiHeader.biYPelsPerMeter);
    printf("\tbiClrUsed = %u\n", bmiHeader.biClrUsed);
    printf("\tbiClrImportant = %u\n", bmiHeader.biClrImportant);
    printf("}");
    printf("\n\n");



    if (bmiHeader.biBitCount <= 8)
    {
        
        const int colorsCount = pow(2, bmiHeader.biBitCount);

        // считываем палитру из 2^bitCount цветов
        bgrQUAD* bgrI = new bgrQUAD[colorsCount];
        for (int i = 0; i < colorsCount; i++)
        {
            bgrI[i].blue = getc(pFile);
            bgrI[i].green = getc(pFile);
            bgrI[i].red = getc(pFile);
            // пропускаем последний байт
            getc(pFile);
            
        }

        // создаём массив для хранения индексов считанных цветов
        int** colorIndex = new int* [bmiHeader.biHeight];
        for (int i = 0; i < bmiHeader.biHeight; i++) {
            colorIndex[i] = new int[bmiHeader.biWidth];
        }

        // заполняем массив индексами считанных цветов
        for (int i = 0; i < bmiHeader.biHeight; i++)
        {
            // читаем индекс
            for (int j = 0; j < bmiHeader.biWidth; j++)
            {
                colorIndex[i][j] = getc(pFile);
            }
            // пропускаем лишние байты в конце строки
            getc(pFile);
            getc(pFile);
            getc(pFile);


        }

        // создаём и выводим оригинальное изображение
        Mat image = imread(imageName, IMREAD_COLOR);
        if (image.empty())
        {
            cout << "\tImage cannot be uploaded" << endl;
            return -1;
        }
        imshow("ORIGINAL IMAGE", image);

        // создаём результирующее изображение
        Mat res(image.size(), CV_8UC3, Scalar(255, 255, 255));


        for (int i = 0; i < bmiHeader.biHeight; i++)
        {
            for (int j = 0; j < bmiHeader.biWidth; j++)
            {
                // в двойном цикле объявляем bgr-компоненты
                uchar b = 255;
                uchar g = 255;
                uchar r = 255;
                // определяем цвет пикселя по его индексу
                for (int k = 0; k < colorsCount; k++)
                {
                    if (colorIndex[i][j] == k)
                    {
                        b = bgrI[k].blue;
                        g = bgrI[k].green;
                        r = bgrI[k].red;
                    }
                }
                // раскрашиваем попиксельно изображение с помощью ф-ии at()
                res.at<Vec3b>(i, j)[0] = b;
                res.at<Vec3b>(i, j)[1] = g;
                res.at<Vec3b>(i, j)[2] = r;
            }
        }

        // переворачиваем изображение, т.к. в opencv оно строится по-другому. код 0 - поворот по горизнт.
        flip(res, res, 0);
        imshow("RESULT IMAGE", res);
        imwrite("C:\\Users\\kucep\\OneDrive - volsu.ru\\Рабочий стол\\Универ\\Лабараторные\\3 курс 2 семестр\\Инженерная и компьютерная графика\\lab4\\res.bmp", res);

        waitKey(0); //wait for key press
        destroyAllWindows(); //destroy all open windows

    }
    else if (bmiHeader.biBitCount > 8)
    {
        // создаём массив цветов
        bgrQUAD** bgr = new bgrQUAD * [bmiHeader.biHeight];
        for (int i = 0; i < bmiHeader.biHeight; i++) {
            bgr[i] = new bgrQUAD[bmiHeader.biWidth];
        }
        // считываем цвета
        for (int i = 0; i < bmiHeader.biHeight; i++) {
            for (int j = 0; j < bmiHeader.biWidth; j++) {
                bgr[i][j].blue = getc(pFile);
                bgr[i][j].green = getc(pFile);
                bgr[i][j].red = getc(pFile);
                bgr[i][j].alpha = getc(pFile);


            }
        }

        //
        Mat image = imread(imageName, IMREAD_COLOR);
        if (image.empty())
        {
            cout << "\nImage cannot be loaded..!!" << endl;
            return -1;
        }
        imshow("ORIGINAL2 IMAGE", image);

        //
        Mat res(image.size(), CV_8UC4, Scalar(255, 255, 255));

        //
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

        //
        flip(res, res, 0);
        imshow("RESULT2 IMAGE", res);
        imwrite("C:\\Users\\kucep\\OneDrive - volsu.ru\\Рабочий стол\\Универ\\Лабараторные\\3 курс 2 семестр\\Инженерная и компьютерная графика\\lab4\\res.bmp", res);

        waitKey(0); //wait for key press
        destroyAllWindows(); //destroy all open windows
    }

    fclose(pFile);


    return 0;
}


static unsigned short read_u16(FILE* fp)
{
    unsigned char b0, b1;

    b0 = getc(fp);
    b1 = getc(fp);

    return (unsigned short)((b1 << 8) | b0);
}


static unsigned int read_u32(FILE* fp)
{
    unsigned char b0, b1, b2, b3;

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return (unsigned int)((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}


static signed long read_s32(FILE* fp)
{
    unsigned char b0, b1, b2, b3;

    b0 = (signed char)getc(fp);
    b1 = (signed char)getc(fp);
    b2 = (signed char)getc(fp);
    b3 = (signed char)getc(fp);

    return (int)((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

