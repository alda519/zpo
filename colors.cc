/**
 * ZPO projekt
 *
 * xdujic01, xmokra00, xrysav17
 */

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "show.h"


const std::string WINDOW = "ZPO-colors ";

typedef unsigned char uchar;


enum {
    MODEL_RGB, // OK
    MODEL_HSV, // OK
    MODEL_HSL,
    MODEL_YUV,
    MODEL_CMY, // OK
    MODEL_RB,
};

std::string names[] = {
    "RGB", "HSV", "HSL", "YUV", "CMY", "RB"
};




/*
 * main
 */
int main(int argc, char *argv[]) {

    if(argc < 2)
        return 1;

    cv::Mat vita;

    // nacist obrazek
    vita = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
    cv::Mat vitove(cv::Size(vita.cols * 4, vita.rows),CV_8UC3);
    //cv::namedWindow(WINDOW, cv::WINDOW_AUTOSIZE * 0); TODO vratit


    int model = MODEL_RB;

    while(model <= MODEL_RB) {
        // 3 obrazky
        cv::Mat a, b, c;
        a = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
        b = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
        c = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);

        // prepocitat obrazky podle vybraneho modelu
        switch(model) {
        case MODEL_RGB:
            show_rgb(vita, a, b, c);
            break;
        case MODEL_HSV:
            show_hsv(vita, a, b, c);
            break;
        case MODEL_CMY:
            show_cmy(vita, a, b, c);
            break;
        case MODEL_HSL:
            show_hsl(vita, a, b, c);
            break;
        case MODEL_YUV: 
            show_yuv(vita, a, b, c);
            break;
        case MODEL_RB:
            show_rg(vita, a, b, c);
            break;
        }

        // vytvorit 1 obrazek ze 4
        cv::Rect roi;
        cv::Mat roiImg;

        // oznacit se region a nakopiruje dilci obrazek
        roi = cv::Rect(vita.cols * 0, 0, vita.cols, vita.rows);
        roiImg = vitove(roi);
        a.copyTo(roiImg);
        roi = cv::Rect(vita.cols * 1, 0, vita.cols, vita.rows);
        roiImg = vitove(roi);
        b.copyTo(roiImg);
        roi = cv::Rect(vita.cols * 2, 0, vita.cols, vita.rows);
        roiImg = vitove(roi);
        c.copyTo(roiImg);
        roi = cv::Rect(vita.cols * 3, 0, vita.cols, vita.rows);
        roiImg = vitove(roi);
        vita.copyTo(roiImg);

        // zobrazit
        cv::imshow(WINDOW + names[model], vitove); // TODO bez jmena
        cv::waitKey();
        cv::destroyWindow(WINDOW + names[model]); // TODO dd

        // prepnout na dalsi model
        model += 1;
    }

    return 0;
}
