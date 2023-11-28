#include "image_processing.h"

Mat Oper_brightness::exec()
{
    image.convertTo(image, -1, 1, value);
    return image;
}

Mat Oper_contrast::exec()
{
    double alpha = 1;
    alpha += value / 100.0;
    image.convertTo(image, -1, alpha, 0);
    return image;
}

Mat Oper_saturation::exec()
{
    cvtColor(image, image, COLOR_BGR2HSV);
    /* for (int i = 0; i < image.rows; i++)
        for (int j = 0; j < image.cols; j++)
            image.at<Vec3b>(i, j)[0] += value; */
    image.forEach<Pixel>([&](Pixel &p, const int *position) -> void
                         { p.y += value; });
    cvtColor(image, image, COLOR_HSV2BGR);
    return image;
}

Mat Oper_clarity::exec()
{
    return image;
}

Mat Oper_colorfulness::exec()
{
    return image;
}
