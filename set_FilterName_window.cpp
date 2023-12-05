#include "set_FilterName_window.h"

FilterName_window::FilterName_window(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    QObject::connect(pushButton_cansel, &QPushButton::clicked, this, &FilterName_window::hide);
    QObject::connect(pushButton_save, &QPushButton::clicked, this, &FilterName_window::save_filter_name);
}

void FilterName_window::set_filters(MyRing<Filter *> *main_filters)
{
    filters = main_filters;
}

bool FilterName_window::is_name_in_filters(std::string filter_name)
{
    for (int i = 0; i < filters->size(); i++)
    {
        if ((*filters)[i]->get_filter_name() == filter_name)
            return true;
    }
    return false;
}

void FilterName_window::save_filter_name()
{
    label_2->setText("");
    std::string s = lineEdit->text().toStdString();
    while (s[0] == ' ')
        s.erase(0, 1);
    while (s[s.length() - 1] == ' ')
        s.erase(s.length() - 1, 1);
    if (s.empty())
    {
        label_2->setText("Name is empty");
        return;
    }
    if (is_name_in_filters(s))
    {
        label_2->setText("This name already exists");
        return;
    }
    filter_name = s;
    emit filter_name_got();
    hide();
}

std::string FilterName_window::get_filter_name()
{
    return filter_name;
}