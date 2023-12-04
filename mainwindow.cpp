#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    image_info.start_image = new QPixmap;
    image_info.image_in_proc = new QPixmap;
    image_info.brightness = 0;
    image_info.contrast = 0;
    image_info.saturation = 0;
    image_info.clarity = 0;
    image_info.temperture = 0;
    image_info.filter = FILTER::ORIGINAL;
    graphicsView_main_im->hide();
    pushButton_Rec_open_1->hide();
    pushButton_Rec_open_2->hide();
    pushButton_Rec_open_3->hide();
    pushButton_Rec_open_4->hide();
    pushButton_Rec_open_5->hide();
    pushButton_brightness->hide();
    pushButton_clarity->hide();
    pushButton_temperature->hide();
    pushButton_contrast->hide();
    pushButton_crop->hide();
    pushButton_saturation->hide();
    regulation->hide();
    out_amount->hide();
    pushButton_left->hide();
    pushButton_right->hide();
    actionExport->setEnabled(false);
    actionNew_image->setEnabled(false);
    pushButton_applyFilter->hide();
    pushButton_filters->hide();
    pushButton_centerF->hide();
    pushButton_leftF->hide();
    pushButton_rightF->hide();
    pushButton_toLeft->hide();
    pushButton_toRight->hide();
    label_centerFname->hide();
    label_leftFname->hide();
    label_rightFname->hide();
    pushButton_back->hide();
    pushButton_addF->hide();
    pushButton_deleteF->hide();
    set_curr_proc(PROCESSES::NON);
    set_rec_opened_butts();
    mythread = new MyThread();
    QObject::connect(QApplication::instance(), &QApplication::aboutToQuit, mythread, &MyThread::terminateThread);
    mythread->start();
    QObject::connect(mythread, &MyThread::signalGUI, this, &MainWindow::change_image);
    QObject::connect(pushButton_New, &QPushButton::clicked, this, [&]()
                     { QString tmp; start_proc(tmp); });
    QObject::connect(pushButton_Rec_open_1, &QPushButton::clicked, this, [&]()
                     { start_proc(pushButton_Rec_open_1->get_image_path()); });
    QObject::connect(pushButton_Rec_open_2, &QPushButton::clicked, this, [&]()
                     { start_proc(pushButton_Rec_open_2->get_image_path()); });
    QObject::connect(pushButton_Rec_open_3, &QPushButton::clicked, this, [&]()
                     { start_proc(pushButton_Rec_open_3->get_image_path()); });
    QObject::connect(pushButton_Rec_open_4, &QPushButton::clicked, this, [&]()
                     { start_proc(pushButton_Rec_open_4->get_image_path()); });
    QObject::connect(pushButton_Rec_open_5, &QPushButton::clicked, this, [&]()
                     { start_proc(pushButton_Rec_open_5->get_image_path()); });
    QObject::connect(pushButton_brightness, &QPushButton::clicked, this, [&]()
                     {  end_main_proc();
                        set_curr_proc(PROCESSES::BRIGHTNESS);
                        set_slider_limits(); 
                        prepare_image(); });
    QObject::connect(pushButton_contrast, &QPushButton::clicked, this, [&]()
                     {  end_main_proc();
                        set_curr_proc(PROCESSES::CONTRAST);
                        set_slider_limits();
                        prepare_image(); });
    QObject::connect(pushButton_saturation, &QPushButton::clicked, this, [&]()
                     {  end_main_proc();
                        set_curr_proc(PROCESSES::SATURATUIN);
                        set_slider_limits();
                        prepare_image(); });
    QObject::connect(pushButton_clarity, &QPushButton::clicked, this, [&]()
                     {  end_main_proc();
                        set_curr_proc(PROCESSES::CLARITY);
                        set_slider_limits();
                        prepare_image(); });
    QObject::connect(pushButton_temperature, &QPushButton::clicked, this, [&]()
                     {  end_main_proc();
                        set_curr_proc(PROCESSES::TEMPERATURE);
                        set_slider_limits();
                        prepare_image(); });
    QObject::connect(pushButton_crop, &QPushButton::clicked, this, [&]()
                     {  end_main_proc();
                        set_curr_proc(PROCESSES::CROP); });
    QObject::connect(regulation, SIGNAL(sliderMoved(int)), out_amount, SLOT(setNum(int)));
    QObject::connect(regulation, &QSlider::sliderMoved, this, &MainWindow::main_proc);
    QObject::connect(actionExport, &QAction::triggered, this, &MainWindow::save_image);
    QObject::connect(actionNew_image, &QAction::triggered, this, &MainWindow::set_new_image);
    QObject::connect(pushButton_left, &QPushButton::clicked, this, [&]()
                     {  end_main_proc();
                        set_curr_proc(PROCESSES::ROTATION);
                        rotate_left(); });
    QObject::connect(pushButton_right, &QPushButton::clicked, this, [&]()
                     {  end_main_proc();
                        set_curr_proc(PROCESSES::ROTATION);
                        rotate_right(); });
    QObject::connect(pushButton_filters, &QPushButton::clicked, this, [&]()
                     {  set_filters();
                        set_filters_buttons(); });
    QObject::connect(pushButton_toLeft, &QPushButton::clicked, this, [&]()
                     {  next_prev_filter(-1);
                        set_filters_buttons(); });
    QObject::connect(pushButton_toRight, &QPushButton::clicked, this, [&]()
                     {  next_prev_filter(1);
                        set_filters_buttons(); });
    QObject::connect(pushButton_leftF, &QPushButton::clicked, this, [&]()
                     { set_filter_number(0);
                       change_image(get_filtered_image(0)); 
                       set_deleteF_enabled(get_filter_name(0)); });
    QObject::connect(pushButton_centerF, &QPushButton::clicked, this, [&]()
                     { set_filter_number(1);
                       change_image(get_filtered_image(1)); 
                       set_deleteF_enabled(get_filter_name(1)); });
    QObject::connect(pushButton_rightF, &QPushButton::clicked, this, [&]()
                     { set_filter_number(2);
                       change_image(get_filtered_image(2)); 
                       set_deleteF_enabled(get_filter_name(2)); });
    QObject::connect(pushButton_back, &QPushButton::clicked, this, &MainWindow::back_from_filters);
    QObject::connect(pushButton_applyFilter, &QPushButton::clicked, this, &MainWindow::apply_filter);
    QObject::connect(pushButton_deleteF, &QPushButton::clicked, this, &MainWindow::delete_filter);
}

MainWindow::~MainWindow()
{
    delete image_info.start_image;
    delete image_info.image_in_proc;
    delete pixmap;
    delete graphicsScene;
}

void MainWindow::change_image(cv::Mat cv_im)
{
    QPixmap Qimage = QPixmap::fromImage(QtOcv::mat2Image(cv_im));
    pixmap->setPixmap(Qimage);
    // graphicsView_main_im->fitInView(pixmap, Qt::KeepAspectRatio);
}

void MainWindow::start_proc(QString &QPath)
{
    QString Qpath_from;
    if (QPath.isEmpty())
    {
        Qpath_from = QFileDialog::getOpenFileName(this, QObject::tr("Open file"), "D:\\", QObject::tr("Images (*.png *.jpg)"), nullptr /* , QFileDialog::DontUseNativeDialog */);
        for (int i = 0; i < Qpath_from.size(); i++)
        {
            if (Qpath_from[i] == '/')
                Qpath_from[i] = '\\';
        }
        if (Qpath_from.isEmpty())
            return;
    }
    else
        Qpath_from = QPath;
    QString Qpath_to, Qfile_name;
    for (auto it = Qpath_from.rbegin(); *it != '\\'; it++)
        Qfile_name.push_back(*it);
    std::reverse(Qfile_name.begin(), Qfile_name.end());
    int i = 0;
    for (auto it = Qfile_name.begin(); *it != '.'; it++, i++)
        ;
    Qfile_name.remove(0, i);
    Qpath_to = "image_in_processing\\input" + Qfile_name;
    fs::create_directory("image_in_processing");
    QFile::remove(Qpath_to);
    QFile image(Qpath_from);
    image.copy(Qpath_to);
    QPixmap Qimage(Qpath_to);
    *(image_info.start_image) = Qimage;
    *(image_info.image_in_proc) = Qimage;
    graphicsView_main_im->show();
    pixmap = new QGraphicsPixmapItem;
    pixmap->setPixmap(Qimage);
    graphicsScene = new QGraphicsScene;
    graphicsScene->addItem(pixmap);
    graphicsView_main_im->setScene(graphicsScene);
    graphicsView_main_im->fitInView(pixmap, Qt::KeepAspectRatio);
    graphicsView_main_im->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView_main_im->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView_main_im->setAlignment(Qt::AlignCenter);
    label_greeting->hide();
    pushButton_brightness->show();
    pushButton_clarity->show();
    pushButton_temperature->show();
    pushButton_contrast->show();
    pushButton_crop->show();
    pushButton_saturation->show();
    pushButton_left->show();
    pushButton_right->show();
    pushButton_filters->show();
    actionExport->setEnabled(true);
    actionNew_image->setEnabled(true);
    QFile file;
    file.setFileName("D:\\University\\cs\\sem3\\cursach\\photored\\recently_opened.json");
    file.open(QIODevice::ReadWrite);
    QString s;
    s = file.readAll();
    QJsonDocument d;
    d = QJsonDocument::fromJson(s.toUtf8());
    QJsonArray pathes = d.array();
    QJsonValue path(Qpath_from);
    if (!pathes.contains(path))
    {
        int n = pathes.size();
        if (n < 5)
        {
            pathes.push_back(path);
        }
        else
        {
            pathes.pop_front();
            pathes.push_back(path);
        }
        d.setArray(pathes);
        s = d.toJson();
        file.resize(0);
        file.write(s.toUtf8());
    }
    file.close();
}

void MainWindow::main_proc(int value)
{
    PROCESSES proc = get_curr_proc();
    QPixmap image = (*image_info.image_in_proc);
    switch (proc)
    {
    case PROCESSES::BRIGHTNESS:
    {
        mythread->push(new Oper_brightness(value, QtOcv::image2Mat(image.toImage())));
        break;
    }
    case PROCESSES::CONTRAST:
    {
        mythread->push(new Oper_contrast(value, QtOcv::image2Mat(image.toImage())));
        break;
    }
    case PROCESSES::SATURATUIN:
    {
        mythread->push(new Oper_saturation(value, QtOcv::image2Mat(image.toImage())));
        break;
    }
    case PROCESSES::CLARITY:
    {
        mythread->push(new Oper_clarity(value, QtOcv::image2Mat(image.toImage())));
        break;
    }
    case PROCESSES::TEMPERATURE:
    {
        mythread->push(new Oper_temperature(value, QtOcv::image2Mat(image.toImage())));
        break;
    }
    }
}

void MainWindow::end_main_proc()
{
    PROCESSES proc = get_curr_proc();
    int value = regulation->value();
    switch (proc)
    {
    case PROCESSES::BRIGHTNESS:
    {
        image_info.brightness = value;
        break;
    }
    case PROCESSES::CONTRAST:
    {
        image_info.contrast = value;
        break;
    }
    case PROCESSES::SATURATUIN:
    {
        image_info.saturation = value;
        break;
    }
    case PROCESSES::CLARITY:
    {
        image_info.clarity = value;
        break;
    }
    case PROCESSES::TEMPERATURE:
    {
        image_info.temperture = value;
        break;
    }
    }
}

void MainWindow::save_image()
{
    QString filter = "PNG(*.png);;JPG(*.jpg)", selected_filter;
    QString filename;
    filename = filename.toUtf8();
    filename = QFileDialog::getSaveFileName(this, QObject::tr("Save File"), "D:\\", filter, &selected_filter /* , QFileDialog::DontUseNativeDialog */);
    if (!filename.isEmpty())
    {
        for (int i = 0; i < filename.size(); i++)
            if (filename[i] == '/')
                filename[i] = '\\';
        set_curr_proc(PROCESSES::NON);
        cv::imwrite(filename.toStdString(), QtOcv::image2Mat(image_info.image_in_proc->toImage()));
    }
}

void MainWindow::set_new_image()
{
    graphicsView_main_im->hide();
    regulation->hide();
    pushButton_brightness->hide();
    pushButton_contrast->hide();
    pushButton_saturation->hide();
    pushButton_clarity->hide();
    pushButton_temperature->hide();
    pushButton_crop->hide();
    out_amount->hide();
    pushButton_left->hide();
    pushButton_right->hide();
    pushButton_filters->hide();
    pushButton_toLeft->hide();
    pushButton_leftF->hide();
    label_leftFname->hide();
    pushButton_centerF->hide();
    label_centerFname->hide();
    pushButton_rightF->hide();
    label_rightFname->hide();
    pushButton_toRight->hide();
    pushButton_addF->hide();
    pushButton_deleteF->hide();
    pushButton_back->hide();
    pushButton_applyFilter->hide();
    pushButton_New->show();
    pushButton_Rec_open_1->show();
    pushButton_Rec_open_2->show();
    pushButton_Rec_open_3->show();
    pushButton_Rec_open_4->show();
    pushButton_Rec_open_5->show();
    label_greeting->show();
    actionExport->setEnabled(false);
    actionNew_image->setEnabled(false);
    delete pixmap;
    delete graphicsScene;
}

void MainWindow::apply_filter()
{
    std::string filter_name = filters[filter_number]->get_filter_name();
    if (filter_name == "Inverse")
    {
        image_info.filter = FILTER::INVERSE;
    }
    else if (filter_name == "Gray")
    {
        image_info.filter = FILTER::GRAY;
    }
    else if (filter_name == "Original")
    {
        image_info.filter = FILTER::ORIGINAL;
    }
    else
    {
        image_info.filter = FILTER::CUSTOM;
        image_info.brightness = filters[filter_number]->get_brightness();
        image_info.contrast = filters[filter_number]->get_contrast();
        image_info.saturation = filters[filter_number]->get_saturation();
        image_info.clarity = filters[filter_number]->get_clarity();
        image_info.temperture = filters[filter_number]->get_temperature();
    }
    prepare_image();
    back_from_filters();
}
