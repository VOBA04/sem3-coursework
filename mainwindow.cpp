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
    image_info.colorfulness = 0;
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
    pushButton_colorfulness->hide();
    pushButton_contrast->hide();
    pushButton_crop->hide();
    pushButton_saturation->hide();
    regulation->hide();
    out_amount->hide();
    set_rec_opened_butts(this);
    mythread = new MyThread();
    QObject::connect(QApplication::instance(), &QApplication::aboutToQuit, mythread, &MyThread::terminateThread);
    mythread->start();
    QObject::connect(mythread, &MyThread::signalGUI, this, &MainWindow::change_image);
    QObject::connect(pushButton_New, &QPushButton::clicked, this, [&]()
                     { QString tmp; start_proc(this, tmp); });
    QObject::connect(pushButton_Rec_open_1, &QPushButton::clicked, this, [&]()
                     { start_proc(this, pushButton_Rec_open_1->get_image_path()); });
    QObject::connect(pushButton_Rec_open_2, &QPushButton::clicked, this, [&]()
                     { start_proc(this, pushButton_Rec_open_2->get_image_path()); });
    QObject::connect(pushButton_Rec_open_3, &QPushButton::clicked, this, [&]()
                     { start_proc(this, pushButton_Rec_open_3->get_image_path()); });
    QObject::connect(pushButton_Rec_open_4, &QPushButton::clicked, this, [&]()
                     { start_proc(this, pushButton_Rec_open_4->get_image_path()); });
    QObject::connect(pushButton_Rec_open_5, &QPushButton::clicked, this, [&]()
                     { start_proc(this, pushButton_Rec_open_5->get_image_path()); });
    QObject::connect(pushButton_brightness, &QPushButton::clicked, this, [&]()
                     {  end_main_proc(this);
                        this->set_curr_proc(PROCESSES::BRIGHTNESS);
                        set_slider_limits(this); });
    QObject::connect(pushButton_contrast, &QPushButton::clicked, this, [&]()
                     {  end_main_proc(this);
                        this->set_curr_proc(PROCESSES::CONTRAST);
                        set_slider_limits(this); });
    QObject::connect(pushButton_saturation, &QPushButton::clicked, this, [&]()
                     {  end_main_proc(this);
                        this->set_curr_proc(PROCESSES::SATURATUIN);
                        set_slider_limits(this); });
    QObject::connect(pushButton_clarity, &QPushButton::clicked, this, [&]()
                     {  end_main_proc(this);
                        this->set_curr_proc(PROCESSES::CLARITY);
                        set_slider_limits(this); });
    QObject::connect(pushButton_colorfulness, &QPushButton::clicked, this, [&]()
                     {  end_main_proc(this);
                        this->set_curr_proc(PROCESSES::COLORFULNESS);
                        set_slider_limits(this); });
    QObject::connect(pushButton_crop, &QPushButton::clicked, this, [&]()
                     {  end_main_proc(this);;
                        this->set_curr_proc(PROCESSES::CROP); });
    QObject::connect(regulation, SIGNAL(sliderMoved(int)), out_amount, SLOT(setNum(int)));
    QObject::connect(regulation, &QSlider::sliderMoved, this, [&]()
                     { main_proc(this); });
}

MainWindow::~MainWindow()
{
}

void MainWindow::set_curr_proc(PROCESSES n)
{
    current_process = n;
}

PROCESSES MainWindow::get_curr_proc()
{
    return current_process;
}

void MainWindow::change_image(cv::Mat cv_im)
{
    QPixmap Qimage = QPixmap::fromImage(QtOcv::mat2Image(cv_im));
    pixmap->setPixmap(Qimage);
    graphicsView_main_im->fitInView(pixmap, Qt::KeepAspectRatio);
}

void start_proc(MainWindow *w, QString &QPath)
{
    QString Qpath_from;
    if (QPath.isEmpty())
    {
        Qpath_from = QFileDialog::getOpenFileName(w, QObject::tr("Open file"), "D:\\", QObject::tr("Images (*.png *.jpg)"), nullptr, QFileDialog::DontUseNativeDialog);
        for (int i = 0; i < Qpath_from.size(); i++)
        {
            if (Qpath_from[i] == '/')
                Qpath_from[i] = '\\';
        }
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
    *(w->image_info.start_image) = Qimage;
    w->graphicsView_main_im->show();
    w->pixmap->setPixmap(Qimage);
    w->graphicsView_main_im->fitInView(w->pixmap, Qt::KeepAspectRatio);
    w->label_greeting->hide();
    w->pushButton_brightness->show();
    w->pushButton_clarity->show();
    w->pushButton_colorfulness->show();
    w->pushButton_contrast->show();
    w->pushButton_crop->show();
    w->pushButton_saturation->show();
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

void set_slider_limits(MainWindow *w)
{
    PROCESSES n = w->get_curr_proc();
    w->out_amount->setNum(0);
    if (n != PROCESSES::CLARITY)
    {
        w->regulation->setMinimum(-100);
        w->regulation->setMaximum(100);
    }
    else
    {
        w->regulation->setMinimum(0);
        w->regulation->setMaximum(100);
    }
    switch (n)
    {
    case PROCESSES::BRIGHTNESS:
    {
        w->regulation->setValue(w->image_info.brightness);
        break;
    }
    case PROCESSES::CONTRAST:
    {
        w->regulation->setValue(w->image_info.contrast);
        break;
    }
    case PROCESSES::SATURATUIN:
    {
        w->regulation->setValue(w->image_info.saturation);
        break;
    }
    case PROCESSES::CLARITY:
    {
        w->regulation->setValue(w->image_info.clarity);
        break;
    }
    case PROCESSES::COLORFULNESS:
    {
        w->regulation->setValue(w->image_info.colorfulness);
        break;
    }
    }
}

void main_proc(MainWindow *w)
{
    MyThread *thread = w->mythread;
    PROCESSES proc = w->get_curr_proc();
    int value = w->regulation->value();
    QPixmap image = *(w->image_info.start_image);
    switch (proc)
    {
    case PROCESSES::BRIGHTNESS:
    {
        thread->push(new Oper_brightness(value, QtOcv::image2Mat(image.toImage())));
        break;
    }
    case PROCESSES::CONTRAST:
    {
        thread->push(new Oper_contrast(value, QtOcv::image2Mat(image.toImage())));
        break;
    }
    case PROCESSES::SATURATUIN:
    {
        thread->push(new Oper_saturation(value, QtOcv::image2Mat(image.toImage())));
        break;
    }
    case PROCESSES::CLARITY:
    {
        thread->push(new Oper_clarity(value, QtOcv::image2Mat(image.toImage())));
        break;
    }
    case PROCESSES::COLORFULNESS:
    {
        thread->push(new Oper_colorfulness(value, QtOcv::image2Mat(image.toImage())));
        break;
    }
    }
}

void end_main_proc(MainWindow *w)
{
    PROCESSES proc = w->get_curr_proc();
    int value = w->regulation->value();
    *(w->image_info.start_image) = w->pixmap->pixmap();
    switch (proc)
    {
    case PROCESSES::BRIGHTNESS:
    {
        w->image_info.brightness = value;
        break;
    }
    case PROCESSES::CONTRAST:
    {
        w->image_info.contrast = value;
        break;
    }
    case PROCESSES::SATURATUIN:
    {
        w->image_info.saturation = value;
        break;
    }
    case PROCESSES::CLARITY:
    {
        w->image_info.clarity = value;
        break;
    }
    case PROCESSES::COLORFULNESS:
    {
        w->image_info.colorfulness = value;
        break;
    }
    }
}