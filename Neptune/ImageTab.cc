//
// File:   PositionTab.cc
// Author: M.R.Eggleston
//
// Created on 05 October 2010, 17:23
//

#include <QFrame>
#include <QGridLayout>
#include <QVBoxLayout>
#include "ImageTab.h"
#include "FractalsConfig.h"
#include "Utilities.h"

ImageTab::ImageTab(QWidget *parent)
:
QWidget(parent),
m_scp_reset_button(new QPushButton("Defaults")),
m_use_ccp_button(new QPushButton("Use current display area")),
m_ccentre_r_entry(new NumberEntry(NumberEntry::FLOAT)),
m_ccentre_i_entry(new NumberEntry(NumberEntry::FLOAT)),
m_width_entry(new NumberEntry(NumberEntry::FLOAT)),
m_start_location_text(new QTextEdit()),
m_current_location_text(new QTextEdit()),
m_crotation_spin(new QSpinBox()),
m_image_size(new ImageSize()),
m_chorizontal_label(new QLabel("r")),
m_cvertical_label(new QLabel("i")),
m_precision_override_check(new QCheckBox("precision override ")),
m_precision_spin(new QSpinBox()),
m_override_widget(new QWidget()),
m_override_button(new QPushButton("Set bits")),
m_config(),
m_ignore_signals(false)
{
    m_config.SetFractalType(Config::instance()->getFractalType());
    m_precision_override_check->setCheckState(Qt::Unchecked);

    setup_ui_layout();
    connect_signals();
    
    setFractalType(Config::instance()->getFractalType());
    display();
}

ImageTab::~ImageTab()
{
}

void ImageTab::setup_ui_layout()
{
    // Background for location QTexyEdit fields
    auto background = palette().window().color();
    QPalette palette;
    palette.setColor(QPalette::Base, background);
    
    auto main_layout = new QVBoxLayout();
    main_layout->setSpacing(0);
    auto label = new QLabel("<b>Current Display Area");
    main_layout->addWidget(label);
    auto grid = new QGridLayout();
    m_current_location_text->setReadOnly(true);
    m_current_location_text->setFrameShape(QFrame::NoFrame);
    m_current_location_text->setFrameShadow(QFrame::Plain);
    m_current_location_text->setPalette(palette);
    m_current_location_text->setWordWrapMode(QTextOption::WrapAnywhere);
    m_current_location_text->setFixedHeight(100);
    grid->addWidget(m_current_location_text, 0, 0, 1, 3);
    label = new QLabel("Centre");
    grid->addWidget(label, 1, 0, 2, 1);
    grid->addWidget(m_chorizontal_label, 1, 1);
    grid->addWidget(m_cvertical_label, 2, 1);
    grid->addWidget(m_ccentre_r_entry, 1, 2);
    grid->addWidget(m_ccentre_i_entry, 2, 2);
    label = new QLabel("Width");
    grid->addWidget(label, 3, 0, 1, 2);
    grid->addWidget(m_width_entry, 3, 2);
    label = new QLabel("Rotation");
    grid->addWidget(label, 4, 0, 1, 2);
    auto hlayout = new QHBoxLayout();
    m_crotation_spin->setRange(-179, 180);
    m_crotation_spin->setValue(0);
    m_crotation_spin->setWrapping(true);
    hlayout->addWidget(m_crotation_spin);
    label = new QLabel("degrees");
    hlayout->addWidget(label);
    hlayout->addStretch();
    auto hlayout2 = new QHBoxLayout();
    hlayout2->setContentsMargins(0, 0, 0, 0);
    hlayout2->setSpacing(0);
    hlayout2->addWidget(m_precision_override_check);
    m_precision_spin->setRange(BASE_PRECISION + 16, 65536);
    m_precision_spin->setSingleStep(16);
    hlayout2->addWidget(m_precision_spin);
    hlayout2->addWidget(m_override_button);
    m_override_widget->setLayout(hlayout2);
    hlayout->addWidget(m_override_widget);
    grid->addLayout(hlayout, 4, 2);
    auto line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    grid->addWidget(line, 5, 0, 1, 3);    
    label = new QLabel("<b>Starting Area</b>");
    grid->addWidget(label, 6, 0, 1, 3);
    m_start_location_text->setReadOnly(true);
    m_start_location_text->setFrameShape(QFrame::NoFrame);
    m_start_location_text->setFrameShadow(QFrame::Plain);
    m_start_location_text->setPalette(palette);
    m_start_location_text->setWordWrapMode(QTextOption::WrapAnywhere);
    m_start_location_text->setFixedHeight(100);
    grid->addWidget(m_start_location_text, 7, 0, 1, 3);
    main_layout->addLayout(grid);
    
    hlayout = new QHBoxLayout();
    hlayout->addStretch();
    hlayout->addWidget(m_use_ccp_button);
    hlayout->addWidget(m_scp_reset_button);
    main_layout->addLayout(hlayout);
    
    // insert horizontal line
    line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    main_layout->addWidget(line);
    label = new QLabel("<b>Size</b>");
    main_layout->addWidget(label);
    main_layout->addWidget(m_image_size);
    main_layout->addStretch();
    setLayout(main_layout);
}

void ImageTab::connect_signals()
{
    connect(m_scp_reset_button, SIGNAL(clicked(bool)),
            this,               SLOT(on_scp_reset_button(bool)));
    connect(m_use_ccp_button,   SIGNAL(clicked(bool)),
            this,               SLOT(on_use_ccp_button(bool)));
    connect(m_ccentre_r_entry,  SIGNAL(changed()),
            this,               SLOT(on_new_centre()));
    connect(m_ccentre_i_entry,  SIGNAL(changed()),
            this,               SLOT(on_new_centre()));
    connect(m_width_entry,      SIGNAL(changed()),
            this,               SLOT(on_new_width()));
    connect(m_crotation_spin,   SIGNAL(editingFinished()),
            this,               SLOT(on_new_rotation()));
    connect(m_precision_spin,   SIGNAL(valueChanged(int)),
            this,               SLOT(on_precision_changed(int))); 
    connect(m_precision_override_check, SIGNAL(stateChanged(int)),
            this,               SLOT(on_override_check(int))); 
    connect(m_override_button,  SIGNAL(clicked(bool)),
            this,               SLOT(on_precision_set(bool)));
}

void ImageTab::on_scp_reset_button(bool)
{
    m_config.defaultStartingComplexPlane();
    display_start_location();
}

void ImageTab::on_use_ccp_button(bool)
{
    m_config.setStartingCentre(m_config.getCentre());
    m_config.setStartingWidth(m_config.getCurrentWidth());
    display_start_location();
}

void ImageTab::on_new_centre()
{
    StringComplex centre(m_ccentre_r_entry->Get(), m_ccentre_i_entry->Get());
    bool changed;
    m_config.setCentre(centre, changed);
    display_current_location();
    if (changed)
    {
       emit complex_plane_changed();
    }
}

void ImageTab::on_new_width()
{
    auto current_width = m_config.getCurrentWidth();
    auto new_width = m_width_entry->Get();
    bool current_ok = (QString::fromStdString(current_width).toDouble() != 0.0);
    bool new_ok = (QString::fromStdString(new_width).toDouble() != 0.0);
    if (new_ok)
    {
        if (current_width != new_width)
        {
            m_config.setCurrentWidth(new_width);
            emit complex_plane_changed();
        }
    }
    else if (!current_ok)
    {
        m_width_entry->Set("4");
    }
    else
    {
        m_width_entry->Set(current_width);
    }
    display_current_location();
}

void ImageTab::on_new_rotation()
{
    m_config.setRotation(m_crotation_spin->value());
    emit complex_plane_changed();
}

void ImageTab::setFractalType(FRACTAL_TYPE ft)
{
    m_config.SetFractalType(ft);
    display();
}

void ImageTab::display_current_location()
{
    QString location("Centre = ");
    location += complexNumberToString(m_config.getCentre()) + "\n";
    location += "Width = ";
    location += QString::fromStdString(m_config.getCurrentWidth());
    m_current_location_text->setText(location);
}

void ImageTab::display_start_location()
{
    QString location("Centre = ");
    location += complexNumberToString(m_config.getStartingCentre()) + "\n";
    location += "Width = " + QString::fromStdString(m_config.getStartingWidth()) + "\n";
    location += "Rotation = " + QString::number(m_config.getStartingRotation()) + " degrees";
    m_start_location_text->setText(location);
}

void ImageTab::display()
{
    m_ignore_signals = true;
    display_start_location();
    auto c = m_config.getCentre();
    m_ccentre_r_entry->Set(c.r);
    m_ccentre_i_entry->Set(c.i);
    m_width_entry->Set(m_config.getCurrentWidth());
    display_current_location();
    m_crotation_spin->setValue(m_config.isOrbitPlotting() ? 0 : m_config.getRotation());
    m_chorizontal_label->setText("r");
    m_cvertical_label->setText("i");
    m_crotation_spin->setEnabled(!m_config.isOrbitPlotting());
    m_image_size->setSize(m_config.getWidth(), m_config.getHeight());
    int precision = m_config.getPrecision();
    m_override_widget->setVisible(!m_config.isOrbitPlotting());
    if (precision != 0)
    {
        m_precision_spin->setValue(precision);
    }
    m_precision_override_check->setCheckState(precision == 0 ? Qt::Unchecked : Qt::Checked);
    m_precision_spin->setEnabled(precision != 0);
    m_override_button->setEnabled(precision != 0);
    m_ignore_signals = false;
}

ImageSize* ImageTab::image_size()
{
    return m_image_size;
}

void ImageTab::on_precision_changed(int value)
{
    m_override_button->setEnabled((value % 16) == 0);
}

void ImageTab::on_override_check(int state)
{
    if (!m_ignore_signals)
    {
        bool enable = (state == Qt::Checked);
        m_precision_spin->setEnabled(enable);
        m_override_button->setEnabled(enable);
        m_config.setPrecision(enable ? m_precision_spin->value() : 0);
        emit complex_plane_changed();
    }
}

void ImageTab::on_precision_set(bool)
{
    int old = m_config.getPrecision();
    int precision = m_precision_spin->value();
    if (precision != old)
    {
        m_config.setPrecision(precision);
        emit complex_plane_changed();
    }
}