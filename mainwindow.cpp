#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    graphicsScene = new QGraphicsScene;
    pixmap = new QGraphicsPixmapItem;
    image_info.start_image = new QPixmap;
    image_info.brightness = 0;
    image_info.contrast = 0;
    image_info.saturation = 0;
    image_info.clarity = 0;
    image_info.temperture = 0;
    graphicsScene->addItem(pixmap);
    graphicsView_main_im->hide();
    graphicsView_main_im->setScene(graphicsScene);
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
                        set_slider_limits(); });
    QObject::connect(pushButton_contrast, &QPushButton::clicked, this, [&]()
                     {  end_main_proc();
                        set_curr_proc(PROCESSES::CONTRAST);
                        set_slider_limits(); });
    QObject::connect(pushButton_saturation, &QPushButton::clicked, this, [&]()
                     {  end_main_proc();
                        set_curr_proc(PROCESSES::SATURATUIN);
                        set_slider_limits(); });
    QObject::connect(pushButton_clarity, &QPushButton::clicked, this, [&]()
                     {  end_main_proc();
                        set_curr_proc(PROCESSES::CLARITY);
                        set_slider_limits(); });
    QObject::connect(pushButton_temperature, &QPushButton::clicked, this, [&]()
                     {  end_main_proc();
                        set_curr_proc(PROCESSES::TEMPERATURE);
                        set_slider_limits(); });
    QObject::connect(pushButton_crop, &QPushButton::clicked, this, [&]()
                     {  end_main_proc();
                        set_curr_proc(PROCESSES::CROP); });
    QObject::connect(regulation, SIGNAL(sliderMoved(int)), out_amount, SLOT(setNum(int)));
    QObject::connect(regulation, &QSlider::sliderMoved, this, &MainWindow::main_proc);
    QObject::connect(actionExport, &QAction::triggered, this, &MainWindow::save_image);
    QObject::connect(actionNew_image, &QAction::triggered, this, &MainWindow::set_new_image);
    QObject::connect(pushButton_left, &QPushButton::clicked, this, &MainWindow::rotate_left);
    QObject::connect(pushButton_right, &QPushButton::clicked, this, &MainWindow::rotate_right);
}

MainWindow::~MainWindow()
{
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
    graphicsView_main_im->show();
    pixmap->setPixmap(Qimage);
    graphicsView_main_im->fitInView(pixmap, Qt::KeepAspectRatio);
    label_greeting->hide();
    pushButton_brightness->show();
    pushButton_clarity->show();
    pushButton_temperature->show();
    pushButton_contrast->show();
    pushButton_crop->show();
    pushButton_saturation->show();
    pushButton_left->show();
    pushButton_right->show();
    actionExport->setEnabled(true);
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
    QPixmap image = *(image_info.start_image);
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
    *(image_info.start_image) = pixmap->pixmap();
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
        cv::imwrite(filename.toStdString(), QtOcv::image2Mat(image_info.start_image->toImage()));
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
    pushButton_New->show();
    pushButton_Rec_open_1->show();
    pushButton_Rec_open_2->show();
    pushButton_Rec_open_3->show();
    pushButton_Rec_open_4->show();
    pushButton_Rec_open_5->show();
    label_greeting->show();
    actionExport->setEnabled(false);
}
