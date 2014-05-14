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
    MODEL_RGB,
    MODEL_HSV,
    MODEL_YUV,
    MODEL_CMY,
    MODEL_RG,
    MODEL_HSL,

    MODELS, // pocitadlo modelu
};

std::string names[] = {
    "RGB", "HSV", "YUV", "CMY", "RG", "HSL"
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
    cv::namedWindow(WINDOW, cv::WINDOW_AUTOSIZE * 0);


    int model = MODEL_YUV;// MODEL_RGB;

    while(1) {
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
        case MODEL_RG:
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

        // vypsat na obrazek popisky kanalu
        switch(model) {
        case MODEL_RGB:
            cv::putText(vitove, "B", cv::Point(vita.cols*0+30,30), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.4, cvScalar(255,255,255), 2, CV_AA);
            cv::putText(vitove, "G", cv::Point(vita.cols*1+30,30), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.4, cvScalar(255,255,255), 2, CV_AA);
            cv::putText(vitove, "R", cv::Point(vita.cols*2+30,30), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.4, cvScalar(255,255,255), 2, CV_AA);
            break;
        case MODEL_HSV:
            cv::putText(vitove, "H", cv::Point(vita.cols*0+30,30), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.4, cvScalar(0,0,0), 2, CV_AA);
            cv::putText(vitove, "S", cv::Point(vita.cols*1+30,30), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.4, cvScalar(255,255,255), 2, CV_AA);
            cv::putText(vitove, "V", cv::Point(vita.cols*2+30,30), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.4, cvScalar(255,255,255), 2, CV_AA);
            break;
        case MODEL_CMY:
            cv::putText(vitove, "Y", cv::Point(vita.cols*0+30,30), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.4, cvScalar(0,0,0), 2, CV_AA);
            cv::putText(vitove, "M", cv::Point(vita.cols*1+30,30), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.4, cvScalar(0,0,0), 2, CV_AA);
            cv::putText(vitove, "C", cv::Point(vita.cols*2+30,30), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.4, cvScalar(0,0,0), 2, CV_AA);
            break;
        case MODEL_HSL:
            cv::putText(vitove, "H", cv::Point(vita.cols*0+30,30), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.4, cvScalar(0,0,0), 2, CV_AA);
            cv::putText(vitove, "S", cv::Point(vita.cols*1+30,30), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.4, cvScalar(255,255,255), 2, CV_AA);
            cv::putText(vitove, "L", cv::Point(vita.cols*2+30,30), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.4, cvScalar(255,255,255), 2, CV_AA);
            break;
        case MODEL_YUV: 
            cv::putText(vitove, "Y", cv::Point(vita.cols*0+30,30), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.4, cvScalar(0,0,255), 2, CV_AA);
            cv::putText(vitove, "U", cv::Point(vita.cols*1+30,30), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.4, cvScalar(0,0,255), 2, CV_AA);
            cv::putText(vitove, "V", cv::Point(vita.cols*2+30,30), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.4, cvScalar(0,0,255), 2, CV_AA);
            break;
        case MODEL_RG:
            cv::putText(vitove, "R", cv::Point(vita.cols*0+30,30), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.4, cvScalar(0,0,255), 2, CV_AA);
            cv::putText(vitove, "G", cv::Point(vita.cols*1+30,30), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.4, cvScalar(0,0,255), 2, CV_AA);
            break;
        }

        // zobrazit
        cv::imshow(WINDOW, vitove);
        int key = cv::waitKey();

        // prepnout na dalsi model
        if(key == 65363) // sipka ->
            model = (model + 1) % MODELS;
        else if(key == 65361) // sipka <-
            model = (model - 1 + MODELS) % MODELS;
        else if(key == 113 || key == 27) // Q nebo ESC konec
            break;
        else
            model = (model + 1) % MODELS; // implicitne dalsi

    }

    return 0;
}
