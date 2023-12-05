#ifndef SET_FILTERNAME_WINDOW_H
#define SET_FILTERNAME_WINDOW_H

#include "ui_set_FilterName_window.h"
#include "MyRing.h"
#include "image_filters.h"
#include <QString>

class Filter;

class FilterName_window : public QWidget, protected Ui::Form
{
    Q_OBJECT
    MyRing<Filter *> *filters;
    std::string filter_name;

public:
    explicit FilterName_window(QWidget *parent = nullptr);
    void set_filters(MyRing<Filter *> *);
    bool is_name_in_filters(std::string);
    std::string get_filter_name();

public slots:
    void save_filter_name();

signals:
    void filter_name_got();
};

#endif