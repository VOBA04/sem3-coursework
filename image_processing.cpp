#include "image_processing.h"

// сделать, чтобы у изображение на вход тоже всё-таки было фиксированное название

void Oper_brightness::exec()
{
    Mat image = imread(input_image_path);
    image.convertTo(image, -1, 1, value);
    imwrite(output_image_path, image);
}

void Oper_contrast::exec()
{
    Mat image = imread(input_image_path);
    double alpha;
    if (value < 0)
        alpha = 1.0 / (-value);
    if (value == 0)
        alpha = 1;
    image.convertTo(image, -1, value, 0);
    imwrite(output_image_path, image);
}

void Oper_saturation::exec()
{
    Mat image = imread(input_image_path);
    cvtColor(image, image, COLOR_BGR2HSV);
    for (int i = 0; i < image.rows; i++)
        for (int j = 0; j < image.cols; j++)
            image.at<Vec3b>(i, j)[0] += value;
    imwrite(output_image_path, image);
}
