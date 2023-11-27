#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include "mainwindow.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
namespace fs = std::filesystem;

class Operation
{
protected:
    int value;
    std::string input_image_path;
    std::string output_image_path;

public:
    virtual void exec() = 0;
};

class Oper_brightness : public Operation
{
public:
    Oper_brightness(int val)
    {
        value = val;
        fs::path dir("image_in_processing");
        auto im_path = fs::directory_iterator(dir);
        fs::path path = *im_path;
        input_image_path = path.string();
        std::string filename = path.filename().string();
        filename.replace(0, filename.find_last_of('.'), "output");
        fs::path fname(filename);
        path.replace_filename(fname);
        output_image_path = path.string();
    }
    virtual void exec() override;
};

class Oper_contrast : public Operation
{
public:
    Oper_contrast(int val)
    {
        value = val;
        fs::path dir("image_in_processing");
        auto im_path = fs::directory_iterator(dir);
        fs::path path = *im_path;
        input_image_path = path.string();
        std::string filename = path.filename().string();
        filename.replace(0, filename.find_last_of('.'), "output");
        fs::path fname(filename);
        path.replace_filename(fname);
        output_image_path = path.string();
    }
    virtual void exec() override;
};

class Oper_saturation : public Operation
{
public:
    Oper_saturation(int val)
    {
        value = val;
        fs::path dir("image_in_processing");
        auto im_path = fs::directory_iterator(dir);
        fs::path path = *im_path;
        input_image_path = path.string();
        std::string filename = path.filename().string();
        filename.replace(0, filename.find_last_of('.'), "output");
        fs::path fname(filename);
        path.replace_filename(fname);
        output_image_path = path.string();
    }
    virtual void exec() override;
};

class Oper_clarity : public Operation
{
public:
    Oper_clarity(int val)
    {
        value = val;
        fs::path dir("image_in_processing");
        auto im_path = fs::directory_iterator(dir);
        fs::path path = *im_path;
        input_image_path = path.string();
        std::string filename = path.filename().string();
        filename.replace(0, filename.find_last_of('.'), "output");
        fs::path fname(filename);
        path.replace_filename(fname);
        output_image_path = path.string();
    }
    // virtual void exec() override;
};

class Oper_colorfulness : public Operation
{
public:
    Oper_colorfulness(int val)
    {
        value = val;
        fs::path dir("image_in_processing");
        auto im_path = fs::directory_iterator(dir);
        fs::path path = *im_path;
        input_image_path = path.string();
        std::string filename = path.filename().string();
        filename.replace(0, filename.find_last_of('.'), "output");
        fs::path fname(filename);
        path.replace_filename(fname);
        output_image_path = path.string();
    }
    // virtual void exec() override;
};

#endif