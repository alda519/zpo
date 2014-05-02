#include <opencv/cv.h>
#include <opencv/highgui.h>

const std::string WINDOW = "ZPO-colors";

typedef unsigned char uchar;


// HSV
void rgb2hsv(uchar r, uchar g, uchar b, uchar &h, uchar &s, uchar &v) {
    double R = r / 255.0;
    double G = g / 255.0;
    double B = b / 255.0;

    double cmax = std::max(R, std::max(G, B));
    double cmin = std::min(R, std::min(G, B));

    double H;

    // H
    if(r > b) {
        if(r > g) {
            // r > b, g
            H = 60.0 * (G - B) / (cmax - cmin);
        } else {
            // g > r > b
            H = 60.0 * ((B - R) / (cmax - cmin) + 2);
        }
    } else {
        if(b > g) {
            // b > r, g
            H = 60.0 * ((R - G) / (cmax - cmin) + 4);
        } else {
            // g > r > b
            H = 60.0 * ((B - R) / (cmax - cmin) + 2);
        }
    }
    h = H / 360.0 * 255;

    // S
    if(cmax - cmin == 0)
        s = 0;
    else
        s = (cmax - cmin) / cmax * 100; // 255

    // V
    v = cmax * 100; // 255
}

void hsv2rgb(uchar h, uchar s, uchar v, uchar &r, uchar &g, uchar &b) {

    double      hh, p, q, t, ff;
    long        i;

    // seda
    if(s == 0.0) {
        r = v;
        g = v;
        b = v;
        return;
    }
    hh = h / 255.0 * 360;

    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = v * (1.0 - s);
    q = v * (1.0 - (s * ff));
    t = v * (1.0 - (s * (1.0 - ff)));

    switch(i) {
    case 0:
        r = v;
        g = t;
        b = p;
        break;
    case 1:
        r = q;
        g = v;
        b = p;
        break;
    case 2:
        r = p;
        g = v;
        b = t;
        break;

    case 3:
        r = p;
        g = q;
        b = v;
        break;
    case 4:
        r = t;
        g = p;
        b = v;
        break;
    case 5:
    default:
        r = v;
        g = p;
        b = q;
        break;
    }
}

// HSL
void rgb2hsl(uchar r, uchar g, uchar b, uchar &h, uchar &s, uchar &l);
void hsl2rgb(uchar h, uchar s, uchar l, uchar &r, uchar &g, uchar &b);


// YUV

// RG

// CMY



/*
void show_rgb(cv::Mat vita) {
    cv::Mat a, b, c;
    a = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
    b = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
    c = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);

    uchar * data = vita.data;
    uchar * adata = a.data;
    uchar * bdata = b.data;
    uchar * cdata = c.data;

    for(int y = 0; y < vita.rows; ++y) {
        for(int x = 0; x < vita.cols; ++x) {
            uchar r = data[x*vita.step + 3*y+0];
            uchar g = data[x*vita.step + 3*y+0];
            uchar b = data[x*vita.step + 3*y+0];

            // b
            adata[y*vita.step + 3*x+1] = 0;
            adata[y*vita.step + 3*x+2] = 0;
            // g
            bdata[y*vita.step + 3*x+0] = 0;
            bdata[y*vita.step + 3*x+2] = 0;
            // r
            cdata[y*vita.step + 3*x+0] = 0;
            cdata[y*vita.step + 3*x+1] = 0;
        }
    }
}
*/


int main(int argc, char *argv[]) {

    if(argc < 2)
        return 1;

    cv::Mat vita;

    // nacist obrazek
    vita = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
    cv::Mat vitove(cv::Size(vita.cols * 4, vita.rows),CV_8UC3);
    cv::namedWindow(WINDOW, cv::WINDOW_AUTOSIZE * 0);

    // zprasit obrazek
    cv::Mat a, b, c;
    a = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
    b = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
    c = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);

    uchar * data = vita.data;
    uchar * adata = a.data;
    uchar * bdata = b.data;
    uchar * cdata = c.data;

    for(int y = 0; y < vita.rows; ++y) {
        for(int x = 0; x < vita.cols; ++x) {
            uchar b = data[y*vita.step + 3*x+0];
            uchar g = data[y*vita.step + 3*x+1];
            uchar r = data[y*vita.step + 3*x+2];

            uchar h, s, v;
            rgb2hsv(r, g, b, h, s, v);
            
            hsv2rgb(h, 255, 255, r, g, b);

            // H
            adata[y*vita.step + 3*x+0] = b;
            adata[y*vita.step + 3*x+1] = g;
            adata[y*vita.step + 3*x+2] = r;
            // S
            bdata[y*vita.step + 3*x+0] = s;
            bdata[y*vita.step + 3*x+1] = s;
            bdata[y*vita.step + 3*x+2] = s;
            // V
            cdata[y*vita.step + 3*x+0] = v;
            cdata[y*vita.step + 3*x+1] = v;
            cdata[y*vita.step + 3*x+2] = v;
        }
    }

    cv::Rect roi;
    cv::Mat roiImg;
    
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

    /*
    for(int i = 0; i < 4; ++i) {
        cv::Rect roi = cv::Rect(vita.cols * i, 0, vita.cols, vita.rows);
        cv::Mat roiImg = vitove(roi);

        vita.copyTo(roiImg);
    }
    */

    // zobrazit
    cv::imshow(WINDOW, vitove);
    cv::waitKey();

    return 0;
}
