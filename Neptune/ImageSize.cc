// 
// File:   ImageSize.h
// Author: M.R.Eggleston
//
// Created on 02 May 2013, 11.02
//

#include <QHBoxLayout>
#include <QLabel>
#include "ImageSize.h"

const int NUMBER_OF_SIZES = 10;
unsigned int sizes[] = { 360, 400, 450, 480, 500, 640, 750, 800, 960, 1280 };

ImageSize::ImageSize(QWidget* parent)
:
QWidget(parent),
m_height_combo(new QComboBox()),
m_height_spin(new QSpinBox()),
m_width_combo(new QComboBox()),
m_width_spin(new QSpinBox()),
m_custom_check(new QCheckBox()),
m_linked_check(new QCheckBox()),
m_set_button(new QPushButton("Set")),
m_height(500),
m_width(750),
m_old_height(500),
m_old_width(750),
m_aspect_ratio(750.0/500.0),
m_linked(true)
{
    for (int i = 0; i < NUMBER_OF_SIZES; i++)
    {
        m_height_combo->addItem(QString::number(sizes[i]));
        m_width_combo->addItem(QString::number(sizes[i]));
    }
    if (m_aspect_ratio > 1.0)
    {
        int adjusted_min = static_cast<int>(100.0*m_aspect_ratio + 0.5);
        int adjusted_max = static_cast<int>(1280.0/m_aspect_ratio + 0.5);
        m_height_spin->setRange(100, adjusted_max);
        m_width_spin->setRange(adjusted_min, 1280);
    }
    else
     {
        unsigned int adjusted_min = static_cast<unsigned int>(100.0/m_aspect_ratio + 0.5);
        unsigned int adjusted_max = static_cast<unsigned int>(1280.0*m_aspect_ratio + 0.5);
        m_height_spin->setRange(adjusted_min, 1280);
        m_width_spin->setRange(100, adjusted_max);
    }
    set_ui_layout();
    setSize(m_width, m_height);
    connect_signals();
}
        
ImageSize::~ImageSize()
{
}
    
void ImageSize::set_ui_layout()
{
    auto main_layout = new QVBoxLayout();
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(5, 0, 5, 0);
    auto grid = new QGridLayout();
    grid->setHorizontalSpacing(5);
    grid->addWidget(new QLabel("Width"), 0, 0);
    grid->addWidget(m_width_combo, 0, 1);
    grid->addWidget(m_width_spin, 0, 2);
    grid->addWidget(new QLabel("pixels"), 0, 3);
    grid->addWidget(m_custom_check, 0, 4);
    grid->addWidget(new QLabel("custom"), 0, 5);
    grid->addWidget(new QLabel("Height"), 1, 0);
    grid->addWidget(m_height_combo, 1, 1);
    grid->addWidget(m_height_spin, 1, 2);
    grid->addWidget(new QLabel("pixels"), 1, 3);
    grid->addWidget(m_linked_check, 1, 4);
    grid->addWidget(new QLabel("linked"), 1, 5);
    auto layout = new QHBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addLayout(grid);
    layout->addStretch();
    main_layout->addLayout(layout);
    layout = new QHBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addStretch();
    layout->addWidget(m_set_button);
    main_layout->addLayout(layout);
    setLayout(main_layout);
}
    
void ImageSize::connect_signals()
{
    connect(m_height_spin,  SIGNAL(valueChanged(int)),
            this,           SLOT(on_height_changed(int)));
    connect(m_width_spin,   SIGNAL(valueChanged(int)),
            this,           SLOT(on_width_changed(int)));
    connect(m_custom_check, SIGNAL(stateChanged(int)),
            this,           SLOT(on_custom_changed(int)));
    connect(m_linked_check, SIGNAL(stateChanged(int)),
            this,           SLOT(on_linked_changed(int)));
    connect(m_set_button,   SIGNAL(clicked(bool)),
            this,           SLOT(on_set(bool)));
}

void ImageSize::on_height_changed(int value)
{
    if (!m_ignore_signal)
    {
        m_height = static_cast<unsigned int>(value);
        if (m_linked)
        {
            m_width = static_cast<unsigned int>(static_cast<double>(m_height)*m_aspect_ratio + 0.5);
            m_ignore_signal = true;
            m_width_spin->setValue(m_width);
            m_ignore_signal = false;
        }
    }
}

void ImageSize::on_width_changed(int value)
{
    if (!m_ignore_signal)
    {
        m_width = static_cast<unsigned int>(value);
        if (m_linked)
        {
            m_height = static_cast<unsigned int>(static_cast<double>(m_width)/m_aspect_ratio + 0.5);
            m_ignore_signal = true;
            m_height_spin->setValue(m_height);
            m_ignore_signal = false;
        }
    }
}

void ImageSize::on_custom_changed(int state)
{
    bool custom = (state == Qt::Checked);
    m_height_combo->setEnabled(!custom);
    m_height_spin->setEnabled(custom);
    m_width_combo->setEnabled(!custom);
    m_width_spin->setEnabled(custom);
    if (custom)
    {
        m_width = m_width_spin->value();
        m_height = m_height_spin->value();
        m_width_combo->hide();
        m_height_combo->hide();
        m_width_spin->show();
        m_height_spin->show();
        m_ignore_signal = true;
        m_linked_check->setCheckState(m_linked ? Qt::Checked : Qt::Unchecked);
        m_linked_check->setEnabled(true);
        m_ignore_signal = false;
    }
    else
    {
        m_width = m_width_combo->currentText().toUInt();
        m_height = m_height_combo->currentText().toUInt();;
        m_width_spin->hide();
        m_height_spin->hide();
        m_width_combo->show();
        m_height_combo->show();
        m_ignore_signal = true;
        m_linked_check->setCheckState(Qt::Unchecked);
        m_linked_check->setEnabled(false);
        m_ignore_signal = false;
    }
}

void ImageSize::on_linked_changed(int state)
{
    m_linked = (state == Qt::Checked);
    if (m_linked)
    {
        m_aspect_ratio = static_cast<double>(m_width)/static_cast<double>(m_height);
        if (m_aspect_ratio > 1.0)
        {
            unsigned int adjusted_min = static_cast<unsigned int>(100.0*m_aspect_ratio + 0.5);
            unsigned int adjusted_max = static_cast<unsigned int>(1280.0/m_aspect_ratio + 0.5);
            m_height_spin->setRange(100, adjusted_max);
            m_width_spin->setRange(adjusted_min, 1280);
        }
        else
        {
            unsigned int adjusted_min = static_cast<unsigned int>(100.0/m_aspect_ratio + 0.5);
            unsigned int adjusted_max = static_cast<unsigned int>(1280.0*m_aspect_ratio + 0.5);
            m_height_spin->setRange(adjusted_min, 1280);
            m_width_spin->setRange(100, adjusted_max);
        }
    }
    else
    {
        m_height_spin->setRange(100, 1280);
        m_width_spin->setRange(100, 1280);
    }
}

void ImageSize::on_set(bool)
{
    if (m_custom_check->checkState() == Qt::Checked)
    {
        m_old_width = m_width;
        m_old_height = m_height;
    }
    emit new_size(width(), height());
}

void ImageSize::setSize(unsigned int w, unsigned int h)
{
    bool width_in_combo = false;
    int width_index = 0;
    bool height_in_combo = false;
    int height_index = 0;
    for (int i = 0; i < NUMBER_OF_SIZES; i++)
    {
        if (w == sizes[i])
        {
            width_in_combo = true;
            width_index = i;
            break;
        }
    }
    for (int i = 0; i < NUMBER_OF_SIZES; i++)
    {
        if (h == sizes[i])
        {
            height_in_combo = true;
            height_index = i;
            break;
        }
    }
    m_ignore_signal = true;
    m_width_spin->setValue(static_cast<int>(w));
    m_height_spin->setValue(static_cast<int>(h));
    if (width_in_combo && height_in_combo)
    {
        m_linked_check->setCheckState(Qt::Unchecked);
        m_linked_check->setEnabled(false);
        m_width_combo->setCurrentIndex(width_index);
        m_height_combo->setCurrentIndex(height_index);
        m_width_combo->show();
        m_height_combo->show();
        m_width_spin->hide();
        m_height_spin->hide();
    }
    else
    {
        m_custom_check->setCheckState(Qt::Checked);
        m_linked_check->setCheckState(m_linked ? Qt::Checked : Qt::Unchecked);
        m_linked_check->setEnabled(true);
        m_width_spin->show();
        m_height_spin->show();
        m_width_combo->hide();
        m_height_combo->hide();
    }
    m_ignore_signal = false;
}

unsigned int ImageSize::height()
{
    if (m_custom_check->checkState() == Qt::Checked)
    {
        return m_height;
    }
    else
    {
        return m_height_combo->currentText().toUInt();
    }
}

unsigned int ImageSize::width()
{
    if (m_custom_check->checkState() == Qt::Checked)
    {
        return m_width;
    }
    else
    {
        return m_width_combo->currentText().toUInt();
    }
}

