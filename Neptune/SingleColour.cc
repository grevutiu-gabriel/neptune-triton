// 
// File:   SingleColour.h
// Author: M.R.Eggleston
//
// Created on 12 April 2013, 10:35
//

#include <QGridLayout>
#include <QHBoxLayout>
#include "SingleColour.h"

SingleColour::SingleColour(COLOUR_TYPE type, QWidget* parent)
:
QWidget(parent),
m_colour_image_label(new QLabel()),
m_red_spin(new QSpinBox()),
m_blue_spin(new QSpinBox()),
m_green_spin(new QSpinBox()),
m_colour_label(new QLabel()),
m_type(type),
m_red(0),
m_green(0),
m_blue(0),
m_ignore_signals(false)
{
    m_colour_image = QImage(100, 100, QImage::Format_RGB32);
    m_colour_image.fill(0);
    m_colour_image_label->setPixmap(QPixmap::fromImage(m_colour_image));

    m_red_spin->setRange(0, 255);
    m_red_spin->setWrapping(true);
    m_red_spin->setEnabled((m_type == CT_RED) || (m_type == CT_ALL));
    m_green_spin->setRange(0, 255);
    m_green_spin->setWrapping(true);
    m_green_spin->setEnabled((m_type == CT_GREEN) || (m_type == CT_ALL));
    m_blue_spin->setRange(0, 255);
    m_blue_spin->setWrapping(true);
    m_blue_spin->setEnabled((m_type == CT_BLUE) || (m_type == CT_ALL));
    m_colour_label->setText("#000000");

    setup_ui_layout();
    connect_signals();
}

SingleColour::~SingleColour()
{
}

void SingleColour::setup_ui_layout()
{
    auto main_layout = new QHBoxLayout();
    main_layout->setContentsMargins(5, 5, 5, 5);
    main_layout->addWidget(m_colour_image_label);
    auto grid = new QGridLayout();
    grid->setVerticalSpacing(0);
    grid->addWidget(new QLabel("red"), 0, 0);
    grid->addWidget(m_red_spin, 0, 1);
    grid->addWidget(new QLabel("green"), 1, 0);
    grid->addWidget(m_green_spin, 1, 1);
    grid->addWidget(new QLabel("blue"), 2, 0);
    grid->addWidget(m_blue_spin, 2, 1);
    grid->addWidget(new QLabel("colour"), 3, 0);
    grid->addWidget(m_colour_label, 3, 1);
    m_colour_label->setFixedWidth(75);
    main_layout->addLayout(grid);
    main_layout->addStretch();
    
    setLayout(main_layout);
}

void SingleColour::connect_signals()
{
    connect(m_red_spin,   SIGNAL(valueChanged(int)),
            this,         SLOT(on_red_changed(int)));
    connect(m_green_spin, SIGNAL(valueChanged(int)),
            this,         SLOT(on_green_changed(int)));
    connect(m_blue_spin,  SIGNAL(valueChanged(int)),
            this,         SLOT(on_blue_changed(int)));
}

void SingleColour::setColour(const QColor& colour)
{
    m_ignore_signals = true;
    m_red = colour.red();
    m_green = colour.green();
    m_blue = colour.blue();
    m_red_spin->setValue(m_red);
    m_green_spin->setValue(m_green);
    m_blue_spin->setValue(m_blue);
    drawColour();
    setColour();
    m_ignore_signals = false;
}

void SingleColour::on_red_changed(int value)
{
    if (!m_ignore_signals)
    {
        m_red = value;
        drawColour();
        setColour();
        emit changed(QColor(m_red, m_green, m_blue));
    }
}

void SingleColour::on_green_changed(int value)
{
    if (!m_ignore_signals)
    {
        m_green = value;
        drawColour();
        setColour();
        emit changed(QColor(m_red, m_green, m_blue));
    }
}

void SingleColour::on_blue_changed(int value)
{
    if (!m_ignore_signals)
    {
        m_blue = value;
        drawColour();
        setColour();
        emit changed(QColor(m_red, m_green, m_blue));
    }
}

void SingleColour::drawColour()
{
    m_colour_image.fill(QColor(m_red, m_green, m_blue));
    m_colour_image_label->setPixmap(QPixmap::fromImage(m_colour_image));
}

void SingleColour::setColour()
{
    m_colour_label->setText(QColor(m_red, m_green, m_blue).name());
}

QColor SingleColour::value()
{
    return QColor(m_red, m_green, m_blue);
}

void SingleColour::setEnabled(bool value)
{
    if (value)
    {
        m_red_spin->setEnabled((m_type == CT_RED) || (m_type == CT_ALL));
        m_green_spin->setEnabled((m_type == CT_GREEN) || (m_type == CT_ALL));
        m_blue_spin->setEnabled((m_type == CT_BLUE) || (m_type == CT_ALL));
    }
    else
    {
        m_red_spin->setEnabled(false);
        m_green_spin->setEnabled(false);
        m_blue_spin->setEnabled(false);
    }
}
