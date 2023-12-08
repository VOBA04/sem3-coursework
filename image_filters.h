#ifndef FILTERS_H
#define FILTERS_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "image_processing.h"

using namespace cv;

class Filter
{
protected:
    Mat image;
    std::string name;
    int brightness, contrast, saturation, clarity, temperature;

public:
    Mat apply();
    std::string get_filter_name();
    int get_brightness();
    int get_contrast();
    int get_saturation();
    int get_clarity();
    int get_temperature();
    virtual ~Filter() = 0;
};

class Inverse : public Filter
{
public:
    Inverse(Mat);
};

class Original : public Filter
{
public:
    Original(Mat);
};

class Gray : public Filter
{
public:
    Gray(Mat);
};

class CustomFilter : public Filter
{
public:
    CustomFilter(std::string, Mat, int, int, int, int, int);
};

#endif