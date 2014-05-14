/**
 * ZPO projekt
 *
 * xdujic01, xmokra00, xrysav17
 */

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "convert.h"

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

    uchar * data = vita.data;
    uchar * adata = am.data;
    uchar * bdata = bm.data;
    uchar * cdata = cm.data;

    for(int y = 0; y < vita.rows; ++y) {
        for(int x = 0; x < vita.cols; ++x) {
            uchar b = data[y*vita.step + 3*x+0];
            uchar g = data[y*vita.step + 3*x+1];
            uchar r = data[y*vita.step + 3*x+2];

            uchar h, s, l;
            rgb2hsl(r, g, b, h, s, l);

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
            cdata[y*vita.step + 3*x+0] = l;
            cdata[y*vita.step + 3*x+1] = l;
            cdata[y*vita.step + 3*x+2] = l;
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
            adata[y__*vita.step + 3*x+0] = y;
            adata[y__*vita.step + 3*x+1] = y;
            adata[y__*vita.step + 3*x+2] = y;

            // u (B - Y)
            bdata[y__*vita.step + 3*x+0] = u + 128;
            bdata[y__*vita.step + 3*x+1] = u + 128;
            bdata[y__*vita.step + 3*x+2] = u + 128;
            // v (R - Y)
            cdata[y__*vita.step + 3*x+0] = v + 64;
            cdata[y__*vita.step + 3*x+1] = v + 64;
            cdata[y__*vita.step + 3*x+2] = v + 64;
        }
    }
}

void show_rg(cv::Mat vita, cv::Mat &am, cv::Mat &bm, cv::Mat &cm) {

    uchar * adata = am.data;
    uchar * bdata = bm.data;
    uchar * cdata = cm.data;

    uchar r, g;

    for(int y = 0; y < vita.rows; ++y) {
        for(int x = 0; x < vita.cols; ++x) {
            rgb2rg(adata[y*vita.step + 3*x+2],
                   adata[y*vita.step + 3*x+1],
                   adata[y*vita.step + 3*x+0], r, g);

            // r
            adata[y*vita.step + 3*x+0] = r;
            adata[y*vita.step + 3*x+1] = r;
            adata[y*vita.step + 3*x+2] = r;
            // g
            bdata[y*vita.step + 3*x+0] = g;
            bdata[y*vita.step + 3*x+1] = g;
            bdata[y*vita.step + 3*x+2] = g;
            // seda
            cdata[y*vita.step + 3*x+0] = 211;
            cdata[y*vita.step + 3*x+1] = 213;
            cdata[y*vita.step + 3*x+2] = 215;
        }
    }
}
