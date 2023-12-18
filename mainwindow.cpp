#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    FN_W = new FilterName_window();
    image_info.start_image = new QPixmap;
    image_info.image_in_proc = new QPixmap;
    pixmap = new QGraphicsPixmapItem;
    graphicsScene = new QGraphicsScene;
    qApp->installTranslator(&qtlangtransl);
    graphicsScene->addItem(pixmap);
    graphicsView_main_im->setScene(graphicsScene);
    image_info.brightness = 0;
    image_info.contrast = 0;
    image_info.saturation = 0;
    image_info.clarity = 0;
    image_info.temperture = 0;
    image_info.filter = FILTER::ORIGINAL;
    FN_W->hide();
    FN_W->setWindowModality(Qt::WindowModality::ApplicationModal);
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
    pushButton_saturation->hide();
    regulation->hide();
    out_amount->hide();
    pushButton_left->hide();
    pushButton_right->hide();
    actionExport->setEnabled(false);
    actionNew_image->setEnabled(false);
    actionEnglish->setEnabled(false);
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
    set_connections();
}

MainWindow::~MainWindow()
{
    if (!filters.empty())
        save_filters();
    delete image_info.start_image;
    delete image_info.image_in_proc;
    delete pixmap;
    delete graphicsScene;
}

void MainWindow::change_image(cv::Mat cv_im) // изменение отображаемого изображения
{
    QPixmap Qpixmap = QPixmap::fromImage(QtOcv::mat2Image(cv_im));
    pixmap->setPixmap(Qpixmap);
    QSize sz = Qpixmap.size();
    graphicsView_main_im->fitInView(pixmap, Qt::KeepAspectRatio);
}

void MainWindow::start_proc(QString &QPath) // открытие изображения и сохранение его в программу
{
    QString Qpath_from;
    if (QPath.isEmpty())
    {
        Qpath_from = QFileDialog::getOpenFileName(this, QObject::tr("Open file"), "", QObject::tr("Images (*.png *.jpg)"), nullptr);
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
    if (!Qpath_from.isEmpty())
    {
        QPixmap Qpixmap(Qpath_from);
        *(image_info.start_image) = Qpixmap;
        *(image_info.image_in_proc) = Qpixmap;
        graphicsView_main_im->show();
        pixmap = new QGraphicsPixmapItem;
        graphicsScene = new QGraphicsScene;
        graphicsScene->addItem(pixmap);
        graphicsView_main_im->setScene(graphicsScene);
        pixmap->setPixmap(Qpixmap);
        graphicsView_main_im->fitInView(pixmap, Qt::KeepAspectRatio);
        label_greeting->hide();
        pushButton_Rec_open_1->hide();
        pushButton_Rec_open_2->hide();
        pushButton_Rec_open_3->hide();
        pushButton_Rec_open_4->hide();
        pushButton_Rec_open_5->hide();
        pushButton_New->hide();
        pushButton_brightness->show();
        pushButton_clarity->show();
        pushButton_temperature->show();
        pushButton_contrast->show();
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
}

void MainWindow::main_proc(int value) // обработка изображения
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

void MainWindow::end_main_proc() // сохранения измененного значения
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

void MainWindow::save_image() // сохранения изображения на компьютер
{
    QString filter = "PNG(*.png);;JPG(*.jpg)", selected_filter;
    QString filename;
    filename = filename.toUtf8();
    filename = QFileDialog::getSaveFileName(this, QObject::tr("Save File"), "D:\\", filter, &selected_filter);
    if (!filename.isEmpty())
    {
        for (int i = 0; i < filename.size(); i++)
            if (filename[i] == '/')
                filename[i] = '\\';
        prepare_image();
        image_info.image_in_proc->save(filename);
    }
}

void MainWindow::set_new_image() // возвращение к окну открытия изображения
{
    current_process = PROCESSES::NON;
    show_pressed_button();
    image_info.brightness = 0;
    image_info.contrast = 0;
    image_info.saturation = 0;
    image_info.clarity = 0;
    image_info.temperture = 0;
    graphicsView_main_im->hide();
    regulation->hide();
    pushButton_brightness->hide();
    pushButton_contrast->hide();
    pushButton_saturation->hide();
    pushButton_clarity->hide();
    pushButton_temperature->hide();
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
    set_rec_opened_butts();
    delete pixmap;
    delete graphicsScene;
}

void MainWindow::apply_filter() // применение фильтра
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
        image_info.brightness = 0;
        image_info.contrast = 0;
        image_info.saturation = 0;
        image_info.clarity = 0;
        image_info.temperture = 0;
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

void MainWindow::rotate_left() // поворот влево на 90
{
    Mat image = QtOcv::image2Mat(image_info.image_in_proc->toImage());
    Mat start_image = QtOcv::image2Mat(image_info.start_image->toImage());
    rotate(image, image, ROTATE_90_COUNTERCLOCKWISE);
    rotate(start_image, start_image, ROTATE_90_COUNTERCLOCKWISE);
    (*image_info.image_in_proc) = QPixmap::fromImage(QtOcv::mat2Image(image));
    (*image_info.start_image) = QPixmap::fromImage(QtOcv::mat2Image(start_image));
    change_image(image);
}

void MainWindow::rotate_right() // поворот вправо на 90
{
    Mat image = QtOcv::image2Mat(image_info.image_in_proc->toImage());
    Mat start_image = QtOcv::image2Mat(image_info.start_image->toImage());
    rotate(image, image, ROTATE_90_CLOCKWISE);
    rotate(start_image, start_image, ROTATE_90_CLOCKWISE);
    (*image_info.image_in_proc) = QPixmap::fromImage(QtOcv::mat2Image(image));
    (*image_info.start_image) = QPixmap::fromImage(QtOcv::mat2Image(start_image));
    change_image(image);
}
