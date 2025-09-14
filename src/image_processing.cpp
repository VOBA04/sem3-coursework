#include "image_processing.h"

Mat Oper_brightness::exec() // яркость
{
  image.convertTo(image, -1, 1, value);
  return image;
}

Mat Oper_contrast::exec() // контрастность
{
  double alpha = 1;
  alpha += value / 100.0;
  image.convertTo(image, -1, alpha, 0);
  return image;
}

Mat Oper_saturation::exec() // насыщенность
{
  cvtColor(image, image, COLOR_BGR2HSV);
  std::vector<Mat> channels;
  split(image, channels);
  double alpha = 1;
  alpha += value / 100.0;
  channels[1].convertTo(channels[1], -1, alpha, 0);
  merge(channels, image);
  cvtColor(image, image, COLOR_HSV2BGR);
  return image;
}

Mat Oper_clarity::exec() // четкость
{
  if (value > 0) {
    double alpha = 1;
    alpha += value / 100.0;
    Mat kernel = (Mat_<double>(3, 3) << 0, -1 * alpha, 0, -1 * alpha, 5 * alpha,
                  -1 * alpha, 0, -1 * alpha, 0);
    filter2D(image, image, image.depth(), kernel);
  }
  return image;
}

Mat Oper_temperature::exec() // температура
{
  double gamma = 1;
  gamma += abs(value) / 100.0;
  Mat lookUpTable(1, 256, CV_8U);
  uchar *p = lookUpTable.ptr();
  for (int i = 0; i < 256; ++i)
    p[i] = saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);
  std::vector<Mat> bgr;
  split(image, bgr);
  if (value < 0)
    LUT(bgr[2], lookUpTable, bgr[2]);
  if (value > 0)
    LUT(bgr[0], lookUpTable, bgr[0]);
  merge(bgr, image);
  return image;
}
