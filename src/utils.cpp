#include "cvmatandqimage.h"
#include "mainwindow.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QStandardPaths>
#include <QString>

void MainWindow::set_rec_opened_butts() // задает кнопкам ранее открытые
                                        // изображения
{
  // Храним историю открытых файлов в пользовательской папке данных приложения
  const QString dataDir =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  if (!dataDir.isEmpty())
    QDir().mkpath(dataDir);
  const QString recentsPath =
      (dataDir.isEmpty() ? QString("recently_opened.json")
                         : QDir(dataDir).filePath("recently_opened.json"));
  QFile file(recentsPath);
  if (file.open(QIODevice::ReadOnly)) {
    QString s;
    s = file.readAll();
    file.close();
    QJsonDocument d;
    d = QJsonDocument::fromJson(s.toUtf8());
    QJsonArray pathes = d.array();
    QJsonValue path;
    int n = pathes.size(), button_n = 0;
    for (int i = 0; i < n; i++) {
      path = pathes[i];
      s = path.toString();
      QPixmap im(s);
      if (!im.isNull()) {
        QIcon but_im(im);
        switch (button_n) {
        case 0:
          pushButton_Rec_open_1->set_image_path(s);
          pushButton_Rec_open_1->setIcon(but_im);
          pushButton_Rec_open_1->show();
          break;
        case 1:
          pushButton_Rec_open_2->set_image_path(s);
          pushButton_Rec_open_2->setIcon(but_im);
          pushButton_Rec_open_2->show();
          break;
        case 2:
          pushButton_Rec_open_3->set_image_path(s);
          pushButton_Rec_open_3->setIcon(but_im);
          pushButton_Rec_open_3->show();
          break;
        case 3:
          pushButton_Rec_open_4->set_image_path(s);
          pushButton_Rec_open_4->setIcon(but_im);
          pushButton_Rec_open_4->show();
          break;
        case 4:
          pushButton_Rec_open_5->set_image_path(s);
          pushButton_Rec_open_5->setIcon(but_im);
          pushButton_Rec_open_5->show();
          break;
        }
        button_n++;
      }
    }
  }
}

void MainWindow::set_curr_proc(PROCESSES n) { current_process = n; }

PROCESSES MainWindow::get_curr_proc() { return current_process; }

void MainWindow::set_slider_limits() // задает пределы ползунка в зависимости от
                                     // процесса
{
  PROCESSES n = get_curr_proc();
  if (n == PROCESSES::CLARITY) {
    regulation->setMinimum(0);
    regulation->setMaximum(100);
  } else {
    regulation->setMinimum(-100);
    regulation->setMaximum(100);
  }
  switch (n) {
  case PROCESSES::BRIGHTNESS: {
    regulation->setValue(image_info.brightness);
    out_amount->setNum(image_info.brightness);
    break;
  }
  case PROCESSES::CONTRAST: {
    regulation->setValue(image_info.contrast);
    out_amount->setNum(image_info.contrast);
    break;
  }
  case PROCESSES::SATURATION: {
    regulation->setValue(image_info.saturation);
    out_amount->setNum(image_info.saturation);
    break;
  }
  case PROCESSES::CLARITY: {
    regulation->setValue(image_info.clarity);
    out_amount->setNum(image_info.clarity);
    break;
  }
  case PROCESSES::TEMPERATURE: {
    regulation->setValue(image_info.temperture);
    out_amount->setNum(image_info.temperture);
    break;
  }
  case PROCESSES::ROTATION:
  case PROCESSES::FILTER:
  case PROCESSES::NON:
    break;
  }
}

void MainWindow::prepare_image() // применяет к начальному изображению
                                 // сохраненные данные для последующей работы
{
  QPixmap image = *(image_info.start_image);
  if (current_process != PROCESSES::BRIGHTNESS) {
    Oper_brightness oper(image_info.brightness,
                         QtOcv::image2Mat(image.toImage()));
    image = QPixmap::fromImage(QtOcv::mat2Image(oper.exec()));
  }
  if (current_process != PROCESSES::CONTRAST) {
    Oper_contrast oper(image_info.contrast, QtOcv::image2Mat(image.toImage()));
    image = QPixmap::fromImage(QtOcv::mat2Image(oper.exec()));
  }
  if (current_process != PROCESSES::SATURATION) {
    Oper_saturation oper(image_info.saturation,
                         QtOcv::image2Mat(image.toImage()));
    image = QPixmap::fromImage(QtOcv::mat2Image(oper.exec()));
  }
  if (current_process != PROCESSES::CLARITY) {
    Oper_clarity oper(image_info.clarity, QtOcv::image2Mat(image.toImage()));
    image = QPixmap::fromImage(QtOcv::mat2Image(oper.exec()));
  }
  if (current_process != PROCESSES::TEMPERATURE) {
    Oper_temperature oper(image_info.temperture,
                          QtOcv::image2Mat(image.toImage()));
    image = QPixmap::fromImage(QtOcv::mat2Image(oper.exec()));
  }
  if (image_info.filter == FILTER::INVERSE) {
    Inverse filt(QtOcv::image2Mat(image.toImage()));
    image = QPixmap::fromImage(QtOcv::mat2Image(filt.apply()));
  } else if (image_info.filter == FILTER::GRAY) {
    Gray filt(QtOcv::image2Mat(image.toImage()));
    image = QPixmap::fromImage(QtOcv::mat2Image(filt.apply()));
  }
  (*image_info.image_in_proc) = image;
}

void MainWindow::set_filter_number(int n) { filter_number = n; }

void MainWindow::set_filters() {
  pushButton_deleteF->setEnabled(false);
  FILTER temp = image_info.filter;
  image_info.filter = FILTER::CUSTOM;
  prepare_image();
  image_info.filter = temp;
  filters.clean();
  Mat image = QtOcv::image2Mat(image_info.image_in_proc->toImage());
  Mat start_image = QtOcv::image2Mat(image_info.start_image->toImage());
  filters.push(new Inverse(image));
  filters.push(new Original(start_image.clone()));
  filters.push(new Gray(image));
  // Загружаем пользовательские фильтры из пользовательской папки данных
  // приложения
  const QString dataDir =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  if (!dataDir.isEmpty())
    QDir().mkpath(dataDir);
  const QString filtersPath =
      (dataDir.isEmpty() ? QString("filters_inform.json")
                         : QDir(dataDir).filePath("filters_inform.json"));
  QFile file(filtersPath);
  if (file.open(QIODevice::ReadOnly)) {
    QString s;
    s = file.readAll();
    file.close();
    QJsonDocument d;
    d = QJsonDocument::fromJson(s.toUtf8());
    QJsonArray js_filters = d.array();
    QJsonObject js_filter;
    std::string filter_name;
    int br, co, st, cl, tmp;
    for (int i = 0; i < js_filters.size(); i++) {
      js_filter = js_filters[i].toObject();
      filter_name = js_filter["name"].toString().toStdString();
      br = js_filter["brightness"].toInt();
      co = js_filter["contrast"].toInt();
      st = js_filter["saturation"].toInt();
      cl = js_filter["clarity"].toInt();
      tmp = js_filter["temperature"].toInt();
      filters.push(new CustomFilter(filter_name, start_image.clone(), br, co,
                                    st, cl, tmp));
    }
  }
}

void MainWindow::save_filters() {
  // Сохраняем пользовательские фильтры в пользовательской папке данных
  // приложения
  const QString dataDir =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  if (!dataDir.isEmpty())
    QDir().mkpath(dataDir);
  const QString filtersPath =
      (dataDir.isEmpty() ? QString("filters_inform.json")
                         : QDir(dataDir).filePath("filters_inform.json"));
  QFile file(filtersPath);
  file.open(QIODevice::WriteOnly);
  file.resize(0);
  QJsonObject js_filter;
  QJsonArray js_filters;
  QJsonDocument d;
  for (int i = 0; i < filters.size(); i++) {
    std::string filter_name = filters[i]->get_filter_name();
    if (filter_name != "Inverse" && filter_name != "Original" &&
        filter_name != "Gray") {
      js_filter["name"] = QString::fromStdString(filter_name);
      js_filter["brightness"] = filters[i]->get_brightness();
      js_filter["contrast"] = filters[i]->get_contrast();
      js_filter["saturation"] = filters[i]->get_saturation();
      js_filter["clarity"] = filters[i]->get_clarity();
      js_filter["temperature"] = filters[i]->get_temperature();
      js_filters.push_back(js_filter);
    }
  }
  d.setArray(js_filters);
  QString s = d.toJson();
  file.write(s.toUtf8());
  file.close();
}

void MainWindow::set_filters_buttons() {
  QIcon leftF(QPixmap::fromImage(QtOcv::mat2Image(filters[0]->apply())));
  pushButton_leftF->setIcon(leftF);
  pushButton_leftF->setIconSize(pushButton_leftF->icon().actualSize(
      QSize(pushButton_leftF->size().width() - 10,
            pushButton_leftF->size().height() - 10)));
  label_leftFname->setText(
      QString::fromStdString(filters[0]->get_filter_name()));
  QIcon centerF(QPixmap::fromImage(QtOcv::mat2Image(filters[1]->apply())));
  pushButton_centerF->setIcon(centerF);
  pushButton_centerF->setIconSize(pushButton_centerF->icon().actualSize(
      QSize(pushButton_centerF->size().width() - 10,
            pushButton_centerF->size().height() - 10)));
  label_centerFname->setText(
      QString::fromStdString(filters[1]->get_filter_name()));
  QIcon rightF(QPixmap::fromImage(QtOcv::mat2Image(filters[2]->apply())));
  pushButton_rightF->setIcon(rightF);
  pushButton_rightF->setIconSize(pushButton_rightF->icon().actualSize(
      QSize(pushButton_rightF->size().width() - 10,
            pushButton_rightF->size().height() - 10)));
  label_rightFname->setText(
      QString::fromStdString(filters[2]->get_filter_name()));
}

void MainWindow::next_prev_filter(int n) {
  if (n > 0)
    filters.next_node();
  if (n < 0)
    filters.prev_node();
}

Mat MainWindow::get_filtered_image(int n) { return filters[n]->apply(); }

std::string MainWindow::get_filter_name(int n) {
  return filters[n]->get_filter_name();
}

void MainWindow::set_deleteF_enabled(std::string filter_name) {
  if (filter_name == "Inverse" || filter_name == "Original" ||
      filter_name == "Gray")
    pushButton_deleteF->setEnabled(false);
  else
    pushButton_deleteF->setEnabled(true);
}

void MainWindow::back_from_filters() {
  prepare_image();
  change_image(QtOcv::image2Mat((*image_info.image_in_proc).toImage()));
  save_filters();
  filters.clean();
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
  pushButton_brightness->show();
  pushButton_contrast->show();
  pushButton_saturation->show();
  pushButton_clarity->show();
  pushButton_temperature->show();
  pushButton_filters->show();
  pushButton_left->show();
  pushButton_right->show();
}

void MainWindow::delete_filter() {
  switch (filter_number) {
  case 0: {
    filters.pop_head();
    break;
  }
  case 1: {
    filters.next_node();
    filters.pop_head();
    filters.prev_node();
    break;
  }
  case 2: {
    filters.next_node();
    filters.next_node();
    filters.pop_head();
    filters.prev_node();
    filters.prev_node();
    break;
  }
  }
  set_filters_buttons();
  prepare_image();
  change_image(QtOcv::image2Mat((*image_info.image_in_proc).toImage()));
}

void MainWindow::add_filter() {
  std::string filter_name = FN_W->get_filter_name();
  filters.push(new CustomFilter(
      filter_name, QtOcv::image2Mat((*image_info.start_image).toImage()),
      image_info.brightness, image_info.contrast, image_info.saturation,
      image_info.clarity, image_info.temperture));
}

void MainWindow::show_pressed_button() {
  QString pressed = "background-color: gray";
  QString not_pressed = "";
  if (current_process == PROCESSES::BRIGHTNESS)
    pushButton_brightness->setStyleSheet(pressed);
  else
    pushButton_brightness->setStyleSheet(not_pressed);
  if (current_process == PROCESSES::CONTRAST)
    pushButton_contrast->setStyleSheet(pressed);
  else
    pushButton_contrast->setStyleSheet(not_pressed);
  if (current_process == PROCESSES::SATURATION)
    pushButton_saturation->setStyleSheet(pressed);
  else
    pushButton_saturation->setStyleSheet(not_pressed);
  if (current_process == PROCESSES::CLARITY)
    pushButton_clarity->setStyleSheet(pressed);
  else
    pushButton_clarity->setStyleSheet(not_pressed);
  if (current_process == PROCESSES::TEMPERATURE)
    pushButton_temperature->setStyleSheet(pressed);
  else
    pushButton_temperature->setStyleSheet(not_pressed);
}

void MainWindow::set_connections() {
  QObject::connect(pushButton_New, &QPushButton::clicked, this, [&]() {
    QString tmp;
    start_proc(tmp);
  });
  QObject::connect(pushButton_Rec_open_1, &QPushButton::clicked, this, [&]() {
    start_proc(pushButton_Rec_open_1->get_image_path());
  });
  QObject::connect(pushButton_Rec_open_2, &QPushButton::clicked, this, [&]() {
    start_proc(pushButton_Rec_open_2->get_image_path());
  });
  QObject::connect(pushButton_Rec_open_3, &QPushButton::clicked, this, [&]() {
    start_proc(pushButton_Rec_open_3->get_image_path());
  });
  QObject::connect(pushButton_Rec_open_4, &QPushButton::clicked, this, [&]() {
    start_proc(pushButton_Rec_open_4->get_image_path());
  });
  QObject::connect(pushButton_Rec_open_5, &QPushButton::clicked, this, [&]() {
    start_proc(pushButton_Rec_open_5->get_image_path());
  });
  QObject::connect(pushButton_brightness, &QPushButton::clicked, this, [&]() {
    end_main_proc();
    set_curr_proc(PROCESSES::BRIGHTNESS);
    show_pressed_button();
    set_slider_limits();
    prepare_image();
  });
  QObject::connect(pushButton_contrast, &QPushButton::clicked, this, [&]() {
    end_main_proc();
    set_curr_proc(PROCESSES::CONTRAST);
    show_pressed_button();
    set_slider_limits();
    prepare_image();
  });
  QObject::connect(pushButton_saturation, &QPushButton::clicked, this, [&]() {
    end_main_proc();
    set_curr_proc(PROCESSES::SATURATION);
    show_pressed_button();
    set_slider_limits();
    prepare_image();
  });
  QObject::connect(pushButton_clarity, &QPushButton::clicked, this, [&]() {
    end_main_proc();
    set_curr_proc(PROCESSES::CLARITY);
    show_pressed_button();
    set_slider_limits();
    prepare_image();
  });
  QObject::connect(pushButton_temperature, &QPushButton::clicked, this, [&]() {
    end_main_proc();
    set_curr_proc(PROCESSES::TEMPERATURE);
    show_pressed_button();
    set_slider_limits();
    prepare_image();
  });
  QObject::connect(regulation, SIGNAL(sliderMoved(int)), out_amount,
                   SLOT(setNum(int)));
  QObject::connect(regulation, &QSlider::sliderMoved, this,
                   &MainWindow::main_proc);
  QObject::connect(actionExport, &QAction::triggered, this, [&]() {
    end_main_proc();
    set_curr_proc(PROCESSES::NON);
    save_image();
  });
  QObject::connect(actionNew_image, &QAction::triggered, this,
                   &MainWindow::set_new_image);
  QObject::connect(pushButton_left, &QPushButton::clicked, this, [&]() {
    end_main_proc();
    set_curr_proc(PROCESSES::ROTATION);
    show_pressed_button();
    prepare_image();
    rotate_left();
  });
  QObject::connect(pushButton_right, &QPushButton::clicked, this, [&]() {
    end_main_proc();
    set_curr_proc(PROCESSES::ROTATION);
    show_pressed_button();
    prepare_image();
    rotate_right();
  });
  QObject::connect(pushButton_filters, &QPushButton::clicked, this, [&]() {
    end_main_proc();
    set_curr_proc(PROCESSES::FILTER);
    show_pressed_button();
    set_filters();
    set_filters_buttons();
  });
  QObject::connect(pushButton_toLeft, &QPushButton::clicked, this, [&]() {
    next_prev_filter(-1);
    set_filters_buttons();
  });
  QObject::connect(pushButton_toRight, &QPushButton::clicked, this, [&]() {
    next_prev_filter(1);
    set_filters_buttons();
  });
  QObject::connect(pushButton_leftF, &QPushButton::clicked, this, [&]() {
    set_filter_number(0);
    change_image(get_filtered_image(0));
    set_deleteF_enabled(get_filter_name(0));
  });
  QObject::connect(pushButton_centerF, &QPushButton::clicked, this, [&]() {
    set_filter_number(1);
    change_image(get_filtered_image(1));
    set_deleteF_enabled(get_filter_name(1));
  });
  QObject::connect(pushButton_rightF, &QPushButton::clicked, this, [&]() {
    set_filter_number(2);
    change_image(get_filtered_image(2));
    set_deleteF_enabled(get_filter_name(2));
  });
  QObject::connect(pushButton_back, &QPushButton::clicked, this,
                   &MainWindow::back_from_filters);
  QObject::connect(pushButton_applyFilter, &QPushButton::clicked, this,
                   &MainWindow::apply_filter);
  QObject::connect(pushButton_deleteF, &QPushButton::clicked, this,
                   &MainWindow::delete_filter);
  QObject::connect(pushButton_addF, &QPushButton::clicked, this, [&]() {
    FN_W->set_filters(&filters);
    FN_W->show();
  });
  QObject::connect(FN_W, &FilterName_window::filter_name_got, this,
                   &MainWindow::add_filter);
  QObject::connect(actionRussian, &QAction::triggered, this, [&]() {
    change_language("ru");
    actionRussian->setEnabled(false);
    actionEnglish->setEnabled(true);
    actionBelarusian->setEnabled(true);
  });
  QObject::connect(actionEnglish, &QAction::triggered, this, [&]() {
    change_language("en");
    actionRussian->setEnabled(true);
    actionEnglish->setEnabled(false);
    actionBelarusian->setEnabled(true);
  });
  QObject::connect(actionBelarusian, &QAction::triggered, this, [&]() {
    change_language("be");
    actionRussian->setEnabled(true);
    actionEnglish->setEnabled(true);
    actionBelarusian->setEnabled(false);
  });
}

void MainWindow::changeEvent(QEvent *e) // отлавливает изменение языка
{
  if (e->type() == QEvent::LanguageChange) {
    retranslateUi(this);
  }
}

void MainWindow::change_language(const char *lng) // изменияет язык приложения
{
  // Перезагружаем перевод: сначала пытаемся найти .qm рядом с бинарником, затем
  // в ресурсах (вшитых через CMake)
  QCoreApplication::removeTranslator(&qtlangtransl);
  const QString base = QString("photored_%1").arg(QString::fromUtf8(lng));
  const QString appDir = QCoreApplication::applicationDirPath();
  const QString fsCandidate = appDir + "/" + base + ".qm";
  const QString rsrcCandidate = ":/" + base + ".qm";
  if (qtlangtransl.load(fsCandidate) || qtlangtransl.load(rsrcCandidate)) {
    qApp->installTranslator(&qtlangtransl);
  }
}
