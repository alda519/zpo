/**
 * ZPO projekt
 *
 * xdujic01, xmokra00, xrysav17
 */

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "show.h"


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



// HSL
void rgb2hsl(uchar r, uchar g, uchar b, uchar &h, uchar &s, uchar &l) {
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

    // L
    l = (cmax + cmin) / 2 * 255;

    // S
    if(cmax - cmin == 0)
        s = 0;
    else
        s = (cmax - cmin) / (1 - abs(2*l/255.0 -1)) * 255;
}
//void hsl2rgb(uchar h, uchar s, uchar l, uchar &r, uchar &g, uchar &b) {
//}

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

// RG
void rgb2rg(uchar r, uchar g, uchar b, uchar &R, uchar &G) {
    R = 255 * r / (r + g + b + 1);
    G = 255 * g / (r + g + b + 1);
}

// CMY
void rgb2cmy(uchar r, uchar g, uchar b, uchar &c, uchar &m, uchar &y) {
    c = 255 - r;
    m = 255 - g;
    y = 255 - b;
}
void cmy2rgb(uchar c, uchar m, uchar y, uchar &r, uchar &g, uchar &b) {
    rgb2cmy(c, m, y, r, g, b);
}
