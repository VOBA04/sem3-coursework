#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    label_image->hide();
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
                     { this->set_curr_proc(0);
                        set_slider_limits(this); });
    QObject::connect(pushButton_contrast, &QPushButton::clicked, this, [&]()
                     { this->set_curr_proc(1);
                        set_slider_limits(this); });
    QObject::connect(pushButton_saturation, &QPushButton::clicked, this, [&]()
                     { this->set_curr_proc(2);
                        set_slider_limits(this); });
    QObject::connect(pushButton_clarity, &QPushButton::clicked, this, [&]()
                     { this->set_curr_proc(3);
                        set_slider_limits(this); });
    QObject::connect(pushButton_colorfulness, &QPushButton::clicked, this, [&]()
                     { this->set_curr_proc(4);
                        set_slider_limits(this); });
    QObject::connect(pushButton_crop, &QPushButton::clicked, this, [&]()
                     { this->set_curr_proc(5); });
    QObject::connect(regulation, SIGNAL(sliderMoved(int)), out_amount, SLOT(setNum(int)));
    QObject::connect(regulation, &QSlider::sliderMoved, this, [&]()
                     { main_proc(this); });
}

MainWindow::~MainWindow()
{
}

void MainWindow::set_curr_proc(int n)
{
    current_process = n;
}

int MainWindow::get_curr_proc()
{
    return current_process;
}

void MainWindow::change_image()
{
    fs::path dir("image_in_processing");
    auto im_path = fs::directory_iterator(dir);
    fs::path path = *im_path;
    if (path.filename().string() != "output.png" && path.filename().string() != "output.jpg")
        im_path++;
    path = *im_path;
    QString Qpath;
    for (auto it : path.string())
        Qpath.push_back(it);
    QPixmap Qimage(Qpath);
    label_image->setPixmap(Qimage);
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
    Qpath_to = "image_in_processing\\" + Qfile_name;
    fs::create_directory("image_in_processing");
    QFile::remove(Qpath_to);
    QFile image(Qpath_from);
    image.copy(Qpath_to);
    QPixmap Qimage(Qpath_to);
    w->label_greeting->hide();
    w->label_image->setPixmap(Qimage);
    w->label_image->show();
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
    int n = w->get_curr_proc();
    w->out_amount->setNum(0);
    if (n != 3)
    {
        w->regulation->setMinimum(-100);
        w->regulation->setMaximum(100);
    }
    else
    {
        w->regulation->setMinimum(0);
        w->regulation->setMaximum(100);
    }
}

void main_proc(MainWindow *w)
{
    MyThread *thread = w->mythread;
    int proc_number = w->get_curr_proc();
    int value = w->regulation->value();
    /* QObject::connect(regulation, &QSlider::sliderMoved, io_amount, [&]()
                     { io_amount->setPlainText(QString::number(regulation->value())); io_amount->setAlignment(Qt::AlignCenter); }); */
    switch (proc_number)
    {
    case 0:
    {
        thread->push(new Oper_brightness(value));
    }
    }
}