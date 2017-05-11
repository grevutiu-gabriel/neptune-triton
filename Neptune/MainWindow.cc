// 
// File:   MainWindow.cc
// Author: trevone
// 
// Created on 19 March 2013, 10:42
//

#include <iostream>
#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QImage>
#include <QMessageBox>
#include <QMimeData>
#include <QScrollArea>
#include <QStatusBar>
#include <QUrl>
#include <QVBoxLayout>

#include "Config.h"
#include "Fractal.h"
#include "MainWindow.h"
#include "Utilities.h"

MainWindow::MainWindow(QWidget *parent)
:
QMainWindow(parent),
m_iterations_label(new QLabel("Maximum iterations")),
m_iterations_line(new NumberEntry(NumberEntry::UINT)),
m_scroll(new QScrollArea()),
m_fractal_image(new FractalDrawing()),
m_navigation_combo(new NavigationCombobox()),
m_zoom_button(new QPushButton("Zoom")),
m_centre_button(new QPushButton("Centre")),
m_status_label(new QLabel()),
m_fractal_settings(new FractalSettingsWindow()),
m_export_colour(new ExportColourMap()),
m_import_colour(new ImportColourMap()),
m_fractal(new Fractal(m_status_label)),
m_opening_image(false),
m_drop_ok(false)
{
    setMenuBar(0);
    setWindowTitle("Neptune");
    setup_menu();
    setup_ui_layout();
    m_navigation_combo->setValue(N_QUICK);
    m_zoom_button->hide();
    m_centre_button->hide();
    
    connect_signals();
    setAcceptDrops(true);

    m_fractal_settings->hide();
    show();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setup_menu()
{
    auto menu = new QMenu("File");
    auto action = menu->addAction("Open...");
    connect(action, SIGNAL(triggered()), this, SLOT(open()));
    action = menu->addAction("Save Image ...");
    connect(action, SIGNAL(triggered()), this, SLOT(save_image()));
    action = menu->addAction("Save Parameters ...");
    connect(action, SIGNAL(triggered()), this, SLOT(save_parameters()));
    action = menu->addAction("Import Colour Maps ...");
    connect(action, SIGNAL(triggered()), this, SLOT(import_maps()));
    action = menu->addAction("Export Colour Maps ...");
    connect(action, SIGNAL(triggered()), this, SLOT(export_maps()));
    action = menu->addAction("Quit");
    connect(action, SIGNAL(triggered()), this, SLOT(quit()));
    menuBar()->addMenu(menu);
    menu = new QMenu("Edit");
    action = menu->addAction("Fractal Settings ...");
    connect(action, SIGNAL(triggered()), this, SLOT(settings()));
    action = menu->addAction("Revert");
    connect(action, SIGNAL(triggered()), this, SLOT(revert()));
    menuBar()->addMenu(menu);
    menu = new QMenu("Help");
    action = menu->addAction("About Qt");
    connect(action, SIGNAL(triggered()), this, SLOT(aboutQt()));
    action = menu->addAction("About");
    connect(action, SIGNAL(triggered()), this, SLOT(about()));
    menuBar()->addMenu(menu);
}

void MainWindow::setup_ui_layout()
{
    auto main_layout = new QVBoxLayout();
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    auto layout1 = new QHBoxLayout();
    layout1->setSpacing(5);
    layout1->setContentsMargins(10, 0, 0, 0);
    layout1->addWidget(m_iterations_label);
    auto layout2 = new QHBoxLayout();
    layout2->setSpacing(0);
    layout2->setContentsMargins(0, 0, 0, 0);
    layout2->addWidget(m_iterations_line);
    layout2->addWidget(m_navigation_combo);
    layout2->addWidget(m_zoom_button);
    layout2->addWidget(m_centre_button);
    layout2->addStretch();
    layout1->addLayout(layout2);
    main_layout->addLayout(layout1);
    m_scroll = new QScrollArea();
    m_config.SetFractalType(Config::instance()->getFractalType());
    int w = m_config.getWidth();
    int h = m_config.getHeight();
    black_image(w, h);
    m_fractal_image->setFixedSize(w, h);
    m_scroll->setWidget(m_fractal_image);
    resize(w+10, h+80);
    main_layout->addWidget(m_scroll);
    
    m_iterations_line->setMaxLength(9);
    m_iterations_line->setFixedWidth(100);
    display_iterations();

    auto hlayout = new QHBoxLayout();
    hlayout->setSpacing(0);
    hlayout->setContentsMargins(5, 0, 0, 0);
    hlayout->addWidget(m_status_label);
    hlayout->addStretch();
    main_layout->addLayout(hlayout);
    auto widget = new QWidget();
    widget->setLayout(main_layout);
    setCentralWidget(widget);
}

void MainWindow::connect_signals()
{
    connect(m_fractal.get(), SIGNAL(signal_row_by_row_progress()), 
            this,      SLOT(on_row_by_row_progress()));
    connect(m_iterations_line, SIGNAL(changed()),
            this,      SLOT(on_iterations_changed()));
    connect(m_fractal_image, SIGNAL(signal_drawing_changed(FractalDrawing::EVENT)),
            this,      SLOT(on_drawing_changed(FractalDrawing::EVENT)));
    connect(m_fractal_settings->image_size(), SIGNAL(new_size(unsigned int, unsigned int)),
            this,      SLOT(on_new_size(unsigned int, unsigned int)));
    connect(m_fractal_settings->image_tab(), SIGNAL(complex_plane_changed()),
            this,      SLOT(on_config_changed()));
    connect(m_fractal_settings, SIGNAL(fractal_type(FRACTAL_TYPE)),
            this,      SLOT(on_fractal_type(FRACTAL_TYPE)));
    connect(m_fractal_settings->fractal_tab(), SIGNAL(config_changed()),
            this,      SLOT(on_config_changed()));
    connect(m_fractal_settings, SIGNAL(colouring_changed()),
            this,      SLOT(on_colouring_changed()));
    connect(m_navigation_combo, SIGNAL(currentIndexChanged(int)),
            this,      SLOT(on_navigation(int)));
    connect(m_zoom_button, SIGNAL(clicked(bool)),
            this,      SLOT(on_zoom(bool)));
    connect(m_centre_button, SIGNAL(clicked(bool)),
            this,      SLOT(on_centre(bool)));
}

void MainWindow::on_row_by_row_progress()
{
    m_fractal->update_progress();
    m_fractal_image->draw();
}

void MainWindow::on_iterations_changed()
{
    unsigned int iterations = stringToUint(m_iterations_line->Get());
    if (iterations != 0)
    {
        if (m_config.isOrbitPlotting())
        {
            unsigned int start_orbit_iteration = m_config.getStartOrbitIteration();
            if (iterations <= start_orbit_iteration)
            {
                m_config.setStartOrbitIteration(iterations > 0 ? iterations - 1 : 0);
            }
            m_fractal_settings->fractal_tab()->startOrbitIterationRangeChanged(iterations - 1);
        }
        m_fractal->changeIterations(iterations);
    }
}

void MainWindow::on_config_changed()
{
    display_iterations();
    m_fractal->Stop();
    black_image();
    m_fractal->Start();    
    m_fractal_settings->image_tab()->display();
    m_fractal_settings->fractal_tab()->display();
}

void MainWindow::on_transforms_changed()
{
    m_fractal->Stop();
    m_fractal->Start();
}

void MainWindow::on_new_size(unsigned int w, unsigned int h)
{
    m_fractal->Stop();
    m_config.setWidth(w);
    m_config.setHeight(h);
    black_image(w, h);
    m_fractal_image->setFixedSize(w, h);
    resize(w+10, h+80);
    m_fractal->Start();
}

void MainWindow::on_fractal_type(FRACTAL_TYPE ft)
{
    if (ft != m_config.GetFractalType())
    {
        Config::instance()->setFractalType(ft);
        m_config.SetFractalType(ft);
        display_iterations();
        black_image();
        m_fractal->SetFractalType(ft);
    }
}

void MainWindow::on_navigation(int value)
{
    auto type = static_cast<NAVIGATION>(value);
    bool hide_zoom_button = ((type == N_QUICK) || (type == N_OFF));
    bool hide_centre_button = (type != N_CENTRE_IN) && (type != N_CENTRE_OUT);
    (hide_zoom_button ? m_zoom_button->hide() : m_zoom_button->show());
    (hide_centre_button ? m_centre_button->hide() : m_centre_button->show());
    m_fractal_image->setNavigation(type);
}

void MainWindow::on_zoom(bool)
{
    if (m_fractal_image->isSelection())
    {
        m_fractal_image->selectionOff();
        QPoint centre = m_fractal_image->currentSelectionCentre();
        if (m_navigation_combo->getValue() == N_ZOOM_IN)
        {
            int width = m_fractal_image->currentSelectionWidth();
            m_fractal->ZoomIn(centre, width);
        }
        else if (m_navigation_combo->getValue() == N_CENTRE_IN)
        {
            m_fractal->ZoomIn(centre);
        }
        else if (m_navigation_combo->getValue() == N_CENTRE_OUT)
        {
            m_fractal->ZoomOut(centre);
        }
        m_fractal_settings->redisplayLocation();
        black_image();
    }
}

void MainWindow::on_centre(bool)
{
    if (m_fractal_image->isSelection())
    {
        QPoint centre = m_fractal_image->currentSelectionCentre();
        m_fractal_image->selectionOff();
        m_fractal->Centre(centre);
        m_fractal_settings->redisplayLocation();
        black_image();
    }
}

void MainWindow::on_colouring_changed()
{
    if (!m_opening_image)
    {
        m_fractal->changeColours();
        m_fractal_image->draw();
    }
}

void MainWindow::open()
{
    bool file_ok = false;
    QFileDialog dialogue;
    dialogue.setNameFilter("png files (*.png);;npf files (*.npf)");
    dialogue.setWindowTitle("Open seed or parameter file");
    dialogue.setDirectory(QDir::homePath());
    dialogue.setFileMode(QFileDialog::ExistingFile);
    while (!file_ok)
    {
        if (dialogue.exec() == 0)
        {
            break;
        }
        auto file_name = dialogue.selectedFiles().first();
        if (file_name.length() > 4)
        {
            auto extension = file_name.right(4).toLower();
            if (extension == ".png")
            {
                FractalConfig seed_config;
                seed_config.setFromSeed(file_name);
                if (seed_config.isOk())
                {
                    process_image(seed_config);
                    file_ok = true;
                }
            }
            else if (extension == ".npf")
            {
                file_ok = loadParameters(file_name);
            }
        }
        if (!file_ok)
        {
            file_not_valid(file_name);
        }
    }
}

void MainWindow::save_image()
{
    QFileDialog dialogue;
    dialogue.setNameFilter("png files (*.png)");
    dialogue.setWindowTitle("Save image ...");
    dialogue.setDirectory(QDir::homePath());
    dialogue.setAcceptMode(QFileDialog::AcceptSave);
    dialogue.setOptions(QFileDialog::DontConfirmOverwrite);
    bool saved = false;
    while (!saved)
    {
        if (dialogue.exec() == 0)
        {
            break;
        }
        auto file_name = dialogue.selectedFiles().first();
        if (!file_name.isEmpty())
        {
            file_name = addExtension(file_name, ".png");
            QDir directory(QDir::homePath());
            if (directory.exists(file_name))
            {
                file_exists(file_name);
            }
            else
            {
                QImage image = StaticCalculatingUnit::s_image;
                m_config.MetaData(image);
                saved = image.save(file_name, "PNG");
                if (!saved)
                {
                    save_failed(file_name);
                }
            }
        }
    }
}

void MainWindow::save_parameters()
{
    QFileDialog dialogue;
    dialogue.setNameFilter("npf files (*.npf)");
    dialogue.setWindowTitle("Save parameters ...");
    dialogue.setDirectory(QDir::homePath());
    dialogue.setAcceptMode(QFileDialog::AcceptSave);
    dialogue.setOptions(QFileDialog::DontConfirmOverwrite);
    bool exists = true;
    while (exists)
    {
        if (dialogue.exec() == 0)
        {
            break;
        }
        auto file_name = dialogue.selectedFiles().first();
        if (!file_name.isEmpty())
        {
            file_name = addExtension(file_name, ".npf");
            QDir directory(QDir::homePath());
            exists = directory.exists(file_name);
            if (exists)
            {
                file_exists(file_name);
            }
            else
            {
                Config::instance()->save_parameter_file(m_config.GetFractalType(), file_name);
            }
        }
    }
}

void MainWindow::import_maps()
{
    m_import_colour->show();
    m_import_colour->raise();
    m_import_colour->setFocus(Qt::OtherFocusReason);
}

void MainWindow::export_maps()
{
    m_export_colour->show();
    m_export_colour->raise();
    m_export_colour->setFocus(Qt::OtherFocusReason);
}

void MainWindow::quit()
{
    close();
}

void MainWindow::settings()
{
    m_fractal_settings->setVisible(true);
    m_fractal_settings->raise();
    m_fractal_settings->setFocus(Qt::OtherFocusReason);
}

void MainWindow::revert()
{
    m_fractal->Reset();
    m_fractal_settings->redisplayLocation();
    black_image();
}

void MainWindow::aboutQt()
{
    QApplication::aboutQt();
}

void MainWindow::about()
{
    QString message("<big><b>Neptune Version 1.1.0</b></big>");
    message += "<br><br>Neptune is a program for exploring multiple<br>"
               "critical point fractals.<br><br>"
               "Visit <a href=\"http://element90.wordpress.com\">Element90 Fractals</a> "
               "for more information.<br><br>";
    message += QString(QChar(0x00a9)) + "2014 Mark R. Eggleston";
    QMessageBox::about(this, "About Neptune", message);
}

void MainWindow::display_iterations()
{
    if (m_config.isOrbitPlotting())
    {
        m_iterations_label->setText("Orbit length");
    }
    else
    {
        m_iterations_label->setText("Maximum iterations");
    }
    m_iterations_line->Set(QString::number(m_config.getMaxIterations()).toStdString());
}

void MainWindow::start()
{
    m_fractal->Start();
    black_image();
}

void MainWindow::on_drawing_changed(FractalDrawing::EVENT event)
{
    black_image();
    switch (event)
    {
    case FractalDrawing::ZOOM_IN:
        {
            QPoint press = m_fractal_image->getPressButton();
            QPoint release = m_fractal_image->getReleaseButton();
            m_fractal->ZoomIn(press, release);
        }
        m_fractal_settings->redisplayLocation();
        break;
    case FractalDrawing::ZOOM_OUT:
        {
            QPoint press = m_fractal_image->getPressButton();
            m_fractal->ZoomOut(press);
            m_fractal_settings->redisplayLocation();
        }
        break;
    case FractalDrawing::CENTRE:
        {
            QPoint press = m_fractal_image->getPressButton();
            m_fractal->Centre(press);
            m_fractal_settings->redisplayLocation();
        }
        break;
    default:
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    m_fractal_settings->close();
    m_export_colour->close();
    m_import_colour->close();
    event->accept();
}

void  MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls())
    {
        QList<QUrl> urlList = mimeData->urls();
        if (urlList.size() == 1)
        {
            event->acceptProposedAction();
        }
    }
}

void MainWindow::dragMoveEvent(QDragMoveEvent* event)
{
    m_drop_ok = (event->answerRect().intersects(m_fractal_image->geometry()));
}

void MainWindow::dropEvent(QDropEvent* event)
{
    const QMimeData *mimeData = event->mimeData();

    if (m_drop_ok && mimeData->hasUrls())
    {
        bool file_ok = false;
        QString file_name = mimeData->urls().at(0).toLocalFile();
        if (file_name.length() > 4)
        {
            auto extension = file_name.right(4).toLower();
            if (extension == ".png")
            {
                FractalConfig seed_config;
                seed_config.setFromSeed(file_name);
                if (seed_config.isOk())
                {
                    process_image(seed_config);
                    file_ok = true;
                }
            }
            else if (extension == ".npf")
            {
                file_ok = loadParameters(file_name);
            }
            if (!file_ok)
            {
                file_not_valid(file_name);
            }
        }
    }
}

void MainWindow::process_image(FractalConfig& seed)
{
    m_opening_image = true;
    m_config.SetFractal(seed);
    int w = m_config.getWidth();
    int h = m_config.getHeight();
    black_image(w, h);
    m_fractal_image->setFixedSize(w, h);
    resize(w+10, h+80);
    auto ft = m_config.GetFractalType();
    m_fractal_settings->setFractalType(ft);
    m_fractal->SetFractalType(ft);
    display_iterations();
    m_opening_image = false;
}

bool MainWindow::loadParameters(const QString &filename)
{
    m_opening_image = true;
    bool ok = Config::instance()->load_parameter_file(filename);
    if (ok)
    {
        process_image(Config::instance()->fractal_config());
    }
    m_opening_image = false;
    return ok;
}

void MainWindow::file_not_valid(const QString& file_name)
{
    auto file = name_only(file_name);
    QString msg("File " + file +
                " is not a Neptune seed or parameter file that can"
                " be loaded by this version of Neptune.");
    QMessageBox::information(this, "Neptune", msg); // discard return value
}

void MainWindow::file_exists(const QString& file_name)
{
    auto file = name_only(file_name);
    QString msg("File " + file + " already exists");
    QMessageBox::information(this, "Neptune", msg); // discard return value
}

void MainWindow::save_failed(const QString& file_name)
{
    auto file = name_only(file_name);
    QString msg("Failed to save " + file);
    QMessageBox::information(this, "Neptune", msg); // discard return value
}

void MainWindow::black_image(int w, int h)
{
    auto pixmap = QPixmap(w, h);
    pixmap.fill(Qt::black);
    m_fractal_image->setPixmap(pixmap);
}

void MainWindow::black_image()
{
    int w = m_config.getWidth();
    int h = m_config.getHeight();
    black_image(w, h);
}