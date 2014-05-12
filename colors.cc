#include <opencv/cv.h>
#include <opencv/highgui.h>

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
    unsigned char region, p, q, t;
    unsigned int remainder;

    if (s == 0)
    {
        r = v;
        g = v;
        b = v;
        return;
    }

    region = h / 43;
    remainder = (h - (region * 43)) * 6; 

    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
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
        default:
            r = v;
            g = p;
            b = q;
            break;
    }
}



// HSL TODO
void rgb2hsl(uchar r, uchar g, uchar b, uchar &h, uchar &s, uchar &l);
void hsl2rgb(uchar h, uchar s, uchar l, uchar &r, uchar &g, uchar &b);

// YUV
void rgb2yuv(uchar r, uchar g, uchar b, uchar &y, uchar &u, uchar &v) {
    y =   r * 0.299 + g * 0.587 + b * 0.114;
    u = - r * 0.147 - g * 0.289 + b * 0.436;
    v =   r * 0.615 - g * 0.515 - b * 0.100;
}
void yuv2rgb(uchar y, uchar u, uchar v, uchar &r, uchar &g, uchar &b) {
    r =   y * 1.000 + u * 0.000 + v * 1.137;
    g =   y * 1.000 - u * 0.397 - v * 0.580;
    b =   y * 1.000 + u * 2.034 + v * 0.000;
}

// RG TODO

// CMY
void rgb2cmy(uchar r, uchar g, uchar b, uchar &c, uchar &m, uchar &y) {
    c = 255 - r;
    m = 255 - g;
    y = 255 - b;
}
void cmy2rgb(uchar c, uchar m, uchar y, uchar &r, uchar &g, uchar &b) {
    rgb2cmy(c, m, y, r, g, b);
}



void show_rgb(cv::Mat vita, cv::Mat &a, cv::Mat &b, cv::Mat &c) {

    uchar * adata = a.data;
    uchar * bdata = b.data;
    uchar * cdata = c.data;

    for(int y = 0; y < vita.rows; ++y) {
        for(int x = 0; x < vita.cols; ++x) {
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

void show_hsv(cv::Mat vita, cv::Mat &a, cv::Mat &b, cv::Mat &c) {

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
}

void show_cmy(cv::Mat vita, cv::Mat &am, cv::Mat &bm, cv::Mat &cm) {

    uchar * adata = am.data;
    uchar * bdata = bm.data;
    uchar * cdata = cm.data;

    uchar r, g, b;

    for(int y = 0; y < vita.rows; ++y) {
        for(int x = 0; x < vita.cols; ++x) {
            // b - y
            b = adata[y*vita.step + 3*x+0];
            g = adata[y*vita.step + 3*x+1] = 0;
            r = adata[y*vita.step + 3*x+2] = 0;
            rgb2cmy(r, g, b, adata[y*vita.step + 3*x+2],
                             adata[y*vita.step + 3*x+1],
                             adata[y*vita.step + 3*x+0]);
            // g - m
            b = bdata[y*vita.step + 3*x+0] = 0;
            g = bdata[y*vita.step + 3*x+1];
            r = bdata[y*vita.step + 3*x+2] = 0;
            rgb2cmy(r, g, b, bdata[y*vita.step + 3*x+2],
                             bdata[y*vita.step + 3*x+1],
                             bdata[y*vita.step + 3*x+0]);
            // r - c
            b = cdata[y*vita.step + 3*x+0] = 0;
            g = cdata[y*vita.step + 3*x+1] = 0;
            r = cdata[y*vita.step + 3*x+2];
            rgb2cmy(r, g, b, cdata[y*vita.step + 3*x+2],
                             cdata[y*vita.step + 3*x+1],
                             cdata[y*vita.step + 3*x+0]);
        }
    }
}


// TODO
void show_hsl(cv::Mat vita, cv::Mat &am, cv::Mat &bm, cv::Mat &cm) {

    uchar * adata = am.data;
    uchar * bdata = bm.data;
    uchar * cdata = cm.data;

    uchar r, g, b;

    for(int y = 0; y < vita.rows; ++y) {
        for(int x = 0; x < vita.cols; ++x) {
            // b - y
            b = adata[y*vita.step + 3*x+0];
            g = adata[y*vita.step + 3*x+1] = 0;
            r = adata[y*vita.step + 3*x+2] = 0;
            rgb2cmy(r, g, b, adata[y*vita.step + 3*x+2],
                             adata[y*vita.step + 3*x+1],
                             adata[y*vita.step + 3*x+0]);
            // g - m
            b = bdata[y*vita.step + 3*x+0] = 0;
            g = bdata[y*vita.step + 3*x+1];
            r = bdata[y*vita.step + 3*x+2] = 0;
            rgb2cmy(r, g, b, bdata[y*vita.step + 3*x+2],
                             bdata[y*vita.step + 3*x+1],
                             bdata[y*vita.step + 3*x+0]);
            // r - c
            b = cdata[y*vita.step + 3*x+0] = 0;
            g = cdata[y*vita.step + 3*x+1] = 0;
            r = cdata[y*vita.step + 3*x+2];
            rgb2cmy(r, g, b, cdata[y*vita.step + 3*x+2],
                             cdata[y*vita.step + 3*x+1],
                             cdata[y*vita.step + 3*x+0]);
        }
    }
}

void show_yuv(cv::Mat vita, cv::Mat &am, cv::Mat &bm, cv::Mat &cm) {

    uchar * adata = am.data;
    uchar * bdata = bm.data;
    uchar * cdata = cm.data;

    uchar y, u, v;

    for(int y__ = 0; y__ < vita.rows; ++y__) {
        for(int x = 0; x < vita.cols; ++x) {

            rgb2yuv(cdata[y__*vita.step + 3*x+2],
                    cdata[y__*vita.step + 3*x+1],
                    cdata[y__*vita.step + 3*x+0], y, u, v);
            // y
            adata[y*vita.step + 3*x+0] = y;
            adata[y*vita.step + 3*x+1] = y;
            adata[y*vita.step + 3*x+2] = y;

            // u
            bdata[y*vita.step + 3*x+0] = 0;
            bdata[y*vita.step + 3*x+1];
            bdata[y*vita.step + 3*x+2] = 0;
            // v
            cdata[y*vita.step + 3*x+0] = 0;
            cdata[y*vita.step + 3*x+1] = 0;
            cdata[y*vita.step + 3*x+2];
        }
    }
}
// TODO
void show_rb(cv::Mat vita, cv::Mat &am, cv::Mat &bm, cv::Mat &cm) {
    // rgb sede: 215, 213, 211

    uchar * adata = am.data;
    uchar * bdata = bm.data;
    uchar * cdata = cm.data;

    uchar r, g, b;

    for(int y = 0; y < vita.rows; ++y) {
        for(int x = 0; x < vita.cols; ++x) {
            // b - y
            b = adata[y*vita.step + 3*x+0];
            g = adata[y*vita.step + 3*x+1] = 0;
            r = adata[y*vita.step + 3*x+2] = 0;
            rgb2cmy(r, g, b, adata[y*vita.step + 3*x+2],
                             adata[y*vita.step + 3*x+1],
                             adata[y*vita.step + 3*x+0]);
            // g - m
            b = bdata[y*vita.step + 3*x+0] = 0;
            g = bdata[y*vita.step + 3*x+1];
            r = bdata[y*vita.step + 3*x+2] = 0;
            rgb2cmy(r, g, b, bdata[y*vita.step + 3*x+2],
                             bdata[y*vita.step + 3*x+1],
                             bdata[y*vita.step + 3*x+0]);
            // r - c
            b = cdata[y*vita.step + 3*x+0] = 0;
            g = cdata[y*vita.step + 3*x+1] = 0;
            r = cdata[y*vita.step + 3*x+2];
            rgb2cmy(r, g, b, cdata[y*vita.step + 3*x+2],
                             cdata[y*vita.step + 3*x+1],
                             cdata[y*vita.step + 3*x+0]);
        }
    }
}




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


    int model = 0;

    while(model < MODEL_RB) {
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
            show_rb(vita, a, b, c);
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
