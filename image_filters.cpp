#include "image_filters.h"

std::string Filter::get_filter_name()
{
    return name;
}

Mat Filter::apply()
{
    // imshow(name, image);
    // waitKey(0);
    return image;
}

Filter::~Filter() {}

Inverse::Inverse(Mat image)
{
    name = "Inverse";
    Mat max255 = image.clone();
    max255 = Scalar(255, 255, 255);
    absdiff(max255, image, this->image);
    // bitwise_not(image, this->image);
    /* imshow("inverse", this->image);
    waitKey(0); */
}

Original::Original(Mat image)
{
    this->image = image;
    name = "Original";
    /* imshow("original", image);
    waitKey(0); */
}

Gray::Gray(Mat image)
{
    this->image = image;
    name = "Gray";
    cvtColor(image, this->image, COLOR_BGR2GRAY);
    /* imshow("gray", image);
    waitKey(0); */
}

CustomFilter::CustomFilter(std::string name, Mat image, int br, int co, int st, int cl, int tmp)
{
    this->name = name;
    this->image = image;
    brightness = br;
    contrast = co;
    saturation = st;
    clarity = cl;
    temperature = tmp;
    if (brightness)
    {
        Oper_brightness oper(brightness, this->image);
        this->image = oper.exec();
    }
    if (contrast)
    {
        Oper_contrast oper(contrast, this->image);
        this->image = oper.exec();
    }
    if (saturation)
    {
        Oper_saturation oper(saturation, this->image);
        this->image = oper.exec();
    }
    if (clarity)
    {
        Oper_clarity oper(clarity, this->image);
        this->image = oper.exec();
    }
    if (temperature)
    {
        Oper_temperature oper(temperature, this->image);
        this->image = oper.exec();
    }
}

int Filter::get_brightness()
{
    return brightness;
}

int Filter::get_contrast()
{
    return contrast;
}

int Filter::get_saturation()
{
    return saturation;
}

int Filter::get_clarity()
{
    return clarity;
}

int Filter::get_temperature()
{
    return temperature;
}