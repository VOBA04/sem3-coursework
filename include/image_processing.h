#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <QPixmap>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

typedef Point3_<uint8_t> Pixel;

class Operation {
protected:
  int value;
  Mat image;

public:
  virtual Mat exec() = 0;
};

class Oper_brightness : public Operation {
public:
  Oper_brightness(int val, Mat image) {
    value = val;
    this->image = image;
  }
  virtual Mat exec() override;
};

class Oper_contrast : public Operation {
public:
  Oper_contrast(int val, Mat image) {
    value = val;
    this->image = image;
  }
  virtual Mat exec() override;
};

class Oper_saturation : public Operation {
public:
  Oper_saturation(int val, Mat image) {
    value = val;
    this->image = image;
  }
  virtual Mat exec() override;
};

class Oper_clarity : public Operation {
public:
  Oper_clarity(int val, Mat image) {
    value = val;
    this->image = image;
  }
  virtual Mat exec() override;
};

class Oper_temperature : public Operation {
public:
  Oper_temperature(int val, Mat image) {
    value = val;
    this->image = image;
  }
  virtual Mat exec() override;
};

#endif