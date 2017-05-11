//
// File:   MainWindow.cc
// Author: M.R.Eggleston
//
// Created on 02 July 2012, 08:41
//

#include <iostream>
#include "MainWindow.h"
#include <QDir>
#include <QFileDialog>
#include <QGridLayout>
#include <QMessageBox>
#include <QMimeData>
#include <QPixmap>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QUrl>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
:
QTabWidget(parent),
m_input_line(new QLineEdit()),
m_browse_button(new QPushButton("Browse")),
m_output_line(new QLineEdit()),
m_width_spin(new QSpinBox()),
m_old_size_label(new QLabel()),
m_new_size_label(new QLabel()),
m_old_precision_label(new QLabel()),
m_new_precision_label(new QLabel()),
m_illustration(new QLabel()),
m_status_label(new QLabel()),
m_save_as_button(new QPushButton("Save as")),
m_control_button(new QPushButton("Expand")),
m_progress_bar(new QProgressBar()),
m_summary_text(new QTextEdit()),
m_scrolled(new QScrollArea()),
m_summary(Summary::create()),
m_picture(new Picture),
m_state(Picture::PIC_IDLE),
m_width(750),
m_drop_ok(false)
{
    setWindowTitle("Triton");
    setup_ui_layout();
    connect_signals();
}

void MainWindow::setup_ui_layout()
{
    auto main_layout = new QVBoxLayout();
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    auto layout1 = new QGridLayout();
    layout1->setHorizontalSpacing(5);
    layout1->setContentsMargins(5, 0, 0, 0);
    auto input_label = new QLabel("Input file");
    auto output_label = new QLabel("Output file");
    layout1->addWidget(input_label, 0, 0);
    layout1->addWidget(m_input_line, 0, 1);
    layout1->addWidget(m_browse_button, 0, 2);
    m_browse_button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    layout1->addWidget(output_label, 1, 0);
    layout1->addWidget(m_output_line, 1, 1);
    m_output_line->setEnabled(false);
    main_layout->addLayout(layout1);

    auto layout2 = new QHBoxLayout();
    layout2->setSpacing(5);
    layout2->setContentsMargins(5, 0, 0, 0);
    layout2->addWidget(m_width_spin);
    m_width_spin->setRange(100, 40000);
    m_width_spin->setValue(750);
    auto width_label = new QLabel("width in pixels");
    layout2->addWidget(width_label);
    layout2->addStretch();
    main_layout->addLayout(layout2);

    auto layout3 = new QHBoxLayout();
    layout3->setContentsMargins(5, 3, 5, 3);
    auto grid = new QGridLayout();
    grid->setHorizontalSpacing(5);
    grid->setVerticalSpacing(3);
    auto size_label = new QLabel("Image size");
    auto new_size_label = new QLabel("New image size");
    grid->addWidget(size_label, 0, 0);
    grid->addWidget(m_old_size_label, 0, 1);
    grid->addWidget(new_size_label, 1, 0);
    grid->addWidget(m_new_size_label, 1, 1);
    layout3->addLayout(grid);
    layout3->addStretch();
    auto layout4 = new QVBoxLayout();
    layout4->addWidget(m_old_precision_label);
    layout4->addWidget(m_new_precision_label);
    m_old_precision_label->setAlignment(Qt::AlignRight);
    m_new_precision_label->setAlignment(Qt::AlignRight);
    layout3->addLayout(layout4);
    main_layout->addLayout(layout3);

    m_scrolled->setWidgetResizable(false);
    m_scrolled->setBackgroundRole(QPalette::Dark);
    m_scrolled->setMinimumSize(500, 333);
    m_scrolled->setWidget(m_illustration);
    setAcceptDrops(true);
    main_layout->addWidget(m_scrolled);

    auto layout5 = new QHBoxLayout();
    layout5->setSpacing(5);
    layout5->setContentsMargins(5, 0, 0, 0);
    layout5->addWidget(m_status_label);
    layout5->addStretch();
    layout5->addWidget(m_save_as_button);
    m_save_as_button->setVisible(false);
    m_save_as_button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    layout5->addWidget(m_control_button);
    m_control_button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    main_layout->addLayout(layout5);

    main_layout->addWidget(m_progress_bar);
    m_progress_bar->setRange(0, 10000);
    m_progress_bar->setValue(0);
    m_progress_bar->setTextVisible(false);

    auto main_widget = new QWidget();
    main_widget->setLayout(main_layout);
    addTab(main_widget, "Expand");

    auto summary_layout = new QVBoxLayout();
    m_summary_text->setWordWrapMode(QTextOption::WrapAnywhere);
    summary_layout->addWidget(m_summary_text);
    auto summary_widget = new QWidget();
    summary_widget->setLayout(summary_layout);
    addTab(summary_widget, "Summary");

    setObjectName("Triton");

    show();

    setFixedSize(size().width(), size().height());
}

void MainWindow::connect_signals()
{
    connect(m_picture.get(), SIGNAL(signal_state()), this, SLOT(on_state_changed()));
    connect(m_picture.get(), SIGNAL(signal_progress(double)), this, SLOT(on_progress_changed(double)));
    connect(m_picture.get(), SIGNAL(signal_final_status()), SLOT(on_final_status()));
    connect(m_browse_button, SIGNAL(clicked()), this, SLOT(on_browse_button()));
    connect(m_save_as_button, SIGNAL(clicked()), this, SLOT(on_save_as_button()));
    connect(m_control_button, SIGNAL(clicked()), this, SLOT(on_control_button()));
    connect(m_width_spin, SIGNAL(valueChanged(int)), this, SLOT(on_width_spin(int)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_width_spin(int value)
{
    m_state = Picture::PIC_IDLE;
    m_picture->setWidthInPixels(value);
    if (!m_base_out_file_name.isEmpty())
    {
        set_new_image_size(m_picture->newImageSize());
        m_new_precision_label->setText(m_picture->newPrecision());
        set_output_filename();
    }
    m_status_label->setText("");
    change_state(Picture::PIC_IDLE);
}

void MainWindow::on_save_as_button()
{
    QFileDialog dialogue;
    dialogue.setNameFilter("png files (*.png)");
    dialogue.setWindowTitle("Save as ...");
    dialogue.setDirectory(QDir::homePath());
    dialogue.setAcceptMode(QFileDialog::AcceptSave);
    dialogue.setOptions(QFileDialog::DontConfirmOverwrite);
    bool ok = false;
    while (!ok)
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
                m_picture->setOutputFile(file_name);
                ok = m_picture->save();
                if (ok)
                {
                    change_state(Picture::PIC_IDLE);
                }
                else
                {
                    save_failed(file_name);
                }
                ok = true;
            }
        }
        enable_widgets();
    }
}

void MainWindow::on_control_button()
{
    switch (m_state)
    {
    case Picture::PIC_IDLE:
        m_picture->setOutputFile(m_output_line->text());
        m_picture->expand();
        m_status_label->setText("0.00%");
        break;
    case Picture::PIC_EXPANDING:
        m_picture->abort();
        m_progress_bar->setValue(1);
        m_status_label->setText("0.00%");
        break;
    case Picture::PIC_EXPANDED:
        if (save()) change_state(Picture::PIC_IDLE);
        break;
    }
    enable_widgets();
}

void MainWindow::on_browse_button()
{
    QFileDialog dialogue;
    dialogue.setNameFilter("png files (*.png)");
    dialogue.setWindowTitle("Open seed file");
    dialogue.setDirectory(QDir::homePath());
    dialogue.setFileMode(QFileDialog::ExistingFile);
    bool ok = false;
    while (!ok)
    {
        if (dialogue.exec() == 0)
        {
            break;
        }
        auto file_name = dialogue.selectedFiles().first();
        if (!file_name.isEmpty())
        {
            ok = valid_file(file_name);
        }
        if (ok)
        {
            process_file(file_name);
        }
    }
    enable_widgets();
}

void MainWindow::on_progress_changed(double value)
{
    QString str = QString::fromStdString(longdoubleToString(100.0*value, true, 2));
    str += "%";
    m_status_label->setText(str);
    m_progress_bar->setValue(static_cast<int>(value*10000.0));
}

void MainWindow::on_final_status()
{
    Picture::STATE state = m_picture->state();
    if (state == Picture::PIC_EXPANDED)
    {
        m_status_label->setText(m_picture->final_status());
        m_progress_bar->setValue(10000);
    }
    else
    {
        m_status_label->setText("0.00%");
        m_progress_bar->setValue(0);
    }
    change_state(state);
}

bool MainWindow::valid_file(const QString& str)
{
    bool valid = false;
    auto extension = str.right(4);
    if ((extension == ".png") || (extension == ".PNG"))
    {
        QImage seed(str, "PNG");
        FractalConfig seed_config;
        seed_config.setFromSeed(str);
        if (seed_config.isOk())
        {
            // Check for Single Orbit fractals - they can't be expanded.
            m_picture->setConfig(seed_config);
            double w = static_cast<double>(seed.width());
            double h = static_cast<double>(seed.height());
            double ratio = w/h;
            int small_width = static_cast<int>(w > 500.0 ? 500.0 : w);
            int small_height = static_cast<int>(small_width/ratio);
            QSize size(small_width, small_height);
            m_image = seed.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            m_illustration->setGeometry(QRect(0, 0, small_width, small_height));
            m_illustration->setPixmap(QPixmap::fromImage(m_image));
            valid = true;
        }
    }
    if (!valid)
    {
        file_not_valid(str);
    }
    return valid;
}

void MainWindow::process_file(const QString& file)
{
    m_picture->setWidthInPixels(m_width_spin->value());
    m_input_line->setText(file);
    m_summary_text->setPlainText(m_summary->get(m_picture.get()));
    if (m_width != m_picture->width())
    {
        m_width = m_picture->width();
        m_width_spin->setValue(static_cast<int>(m_width));
    }
    set_image_size(m_picture->imageSize());
    set_new_image_size(m_picture->newImageSize());
    m_old_precision_label->setText(m_picture->precision());
    m_new_precision_label->setText(m_picture->newPrecision());
    set_base_filename(file);
    set_output_filename();
    m_status_label->setText("0.00%");
    change_state(Picture::PIC_IDLE);
}

void MainWindow::set_image_size(const QString& size)
{
    m_old_size_label->setText(size);
}

void MainWindow::set_new_image_size(const QString& size)
{
    m_new_size_label->setText(size);
}

void MainWindow::set_base_filename(const QString& str)
{
    int pos1 = str.lastIndexOf('/') + 1;
    int pos2 = str.lastIndexOf('.');
    m_base_out_file_name = str.mid(pos1, pos2 - pos1);
}

void MainWindow::on_state_changed()
{
    change_state(m_picture->state());
}

void MainWindow::change_state(Picture::STATE state)
{
    QString str;
    m_state = state;
    switch (m_state)
    {
    case Picture::PIC_IDLE:
        str = "Expand";
        break;
    case Picture::PIC_EXPANDING:
        str = "Abort";
        break;
    case Picture::PIC_EXPANDED:
        str = "Save";
        break;
    }
    m_control_button->setText(str);
    enable_widgets();
}

void MainWindow::enable_widgets()
{
    m_width_spin->setEnabled(m_state != Picture::PIC_EXPANDING);
    m_browse_button->setEnabled(m_state != Picture::PIC_EXPANDING);
    m_save_as_button->setVisible(m_state == Picture::PIC_EXPANDED);
}

void MainWindow::set_output_filename()
{
    QString file_name = m_base_out_file_name + "." + m_picture->newImageSize() + ".png";
    QString full_name = QDir::homePath() + "/" + file_name;
    m_output_line->setText(full_name);
    m_picture->setOutputFile(full_name);
}

bool MainWindow::save()
{
    bool saved = false;
    QString filename = m_output_line->text();
    QDir directory(QDir::homePath());
    if (directory.exists(filename))
    {
        file_exists(filename);
    }
    else
    {
        saved = m_picture->save();
        if (!saved)
        {
            save_failed(filename);
        }
    }
    return saved;
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
    m_drop_ok = (event->answerRect().intersects(m_scrolled->geometry()));
}

void MainWindow::dropEvent(QDropEvent* event)
{
    const QMimeData *mimeData = event->mimeData();

    if (m_drop_ok && mimeData->hasUrls())
    {
        QString file_name = mimeData->urls().at(0).toLocalFile();
        if (!file_name.isEmpty())
        {
            if (valid_file(file_name))
            {
                process_file(file_name);
            }
        }
        enable_widgets();
    }
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
    QMessageBox::information(this, "Triton", msg); // discard return value
}

void MainWindow::save_failed(const QString& file_name)
{
    auto file = name_only(file_name);
    QString msg("Failed to save " + file);
    QMessageBox::information(this, "Triton", msg); // discard return value
}
