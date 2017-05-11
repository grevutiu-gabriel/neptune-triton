// 
// File:   OrbitColouring.h
// Author: trevone
//
// Created on 03 April 2013, 13:29
//

#include <QHBoxLayout>
#include <QHBoxLayout>
#include "Config.h"
#include "OrbitColouring.h"

OrbitColouring::OrbitColouring(QWidget* parent)
:
QWidget(parent),
m_method_combo(new OrbitColourCombobox()),
m_red_offset_spin(new QSpinBox()),
m_green_offset_spin(new QSpinBox()),
m_blue_offset_spin(new QSpinBox()),
m_lockstep_check(new QCheckBox("lock step")),
m_scale_widget(new QWidget()),
m_interval_widget(new QWidget()),
m_scale_entry(new NumberEntry(NumberEntry::FLOAT)),
m_interval_spin(new QSpinBox()),
m_info_label(new QLabel()),
m_component_order_combo(new ComponentOrderComboBox()),
m_component_order_widget(new QWidget()),
m_background(new SingleColour(CT_ALL)),
m_use_colour_map_check(new QCheckBox("use colour map")),
m_colour_maps(ColourMaps::instance()),
m_ignore_signals(false),
m_lockstep(true)
{
    m_config.SetFractalType(Config::instance()->getFractalType());
    m_red_offset_spin->setRange(0, 511);
    m_red_offset_spin->setWrapping(true);
    m_green_offset_spin->setRange(0, 511);
    m_green_offset_spin->setWrapping(true);
    m_blue_offset_spin->setRange(0, 511);
    m_blue_offset_spin->setWrapping(true);
    m_lockstep_check->setCheckState(Qt::Checked);
    m_interval_spin->setRange(1, 10000000);
    setup_ui_layout();
    connect_signals();
}

OrbitColouring::~OrbitColouring()
{
}

void OrbitColouring::setup_ui_layout()
{
    auto main_layout = new QVBoxLayout();
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    auto hlayout = new QHBoxLayout();
    hlayout->setSpacing(5);
    hlayout->setContentsMargins(5, 0, 0, 0);
    hlayout->addWidget(m_method_combo);
    hlayout->addStretch();
    auto hlayout2 = new QHBoxLayout();
    hlayout2->setSpacing(5);
    hlayout2->setContentsMargins(0, 0, 0, 0);
    hlayout2->addWidget(new QLabel("Colour Component Order"));
    hlayout2->addWidget(m_component_order_combo);
    m_component_order_widget->setLayout(hlayout2);
    hlayout->addWidget(m_component_order_widget);
    main_layout->addLayout(hlayout);
    hlayout = new QHBoxLayout();
    hlayout->setSpacing(5);
    hlayout->setContentsMargins(5, 0, 0, 0);
    hlayout->addWidget(new QLabel("Offsets  red"));
    hlayout->addWidget(m_red_offset_spin);
    hlayout->addWidget(new QLabel("green"));
    hlayout->addWidget(m_green_offset_spin);
    hlayout->addWidget(new QLabel("blue"));
    hlayout->addWidget(m_blue_offset_spin);
    hlayout->addWidget(m_lockstep_check);
    hlayout->addStretch();
    main_layout->addLayout(hlayout);
    hlayout = new QHBoxLayout();
    hlayout->setSpacing(5);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->addWidget(new QLabel("Interval"));
    hlayout->addWidget(m_interval_spin);
    hlayout->addStretch();
    m_interval_widget->hide();
    m_interval_widget->setLayout(hlayout);
    main_layout->addWidget(m_interval_widget);
    hlayout = new QHBoxLayout();
    hlayout->setSpacing(5);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->addWidget(new QLabel("Scale"));
    hlayout->addWidget(m_scale_entry);
    hlayout->addStretch();
    m_scale_widget->hide();
    m_scale_widget->setLayout(hlayout);
    main_layout->addWidget(m_scale_widget);
    hlayout = new QHBoxLayout();
    hlayout->setSpacing(5);
    hlayout->setContentsMargins(5, 5, 5, 5);
    hlayout->addWidget(new QLabel("<b>Background</b>"));
    hlayout->addStretch();
    main_layout->addLayout(hlayout);
    hlayout = new QHBoxLayout();
    hlayout->setSpacing(5);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->addWidget(m_background);
    hlayout->addWidget(m_use_colour_map_check);
    hlayout->addStretch();
    main_layout->addLayout(hlayout);
    auto label = new QLabel("\nThe colour is based on the number of times "
                            "a location is visited by an orbit.\n");
    label->setWordWrap(true);
    label->setContentsMargins(5, 0, 0, 5);
    main_layout->addWidget(label);
    m_info_label->setText("The interval determines the colour applied to a "
                          "point. The orbit length is the number 0f iterations "
                          "calculated for a point. The offset determine the "
                          "starting colour, a counter is "
                          "incremented every time an iteration is calculated, "
                          "when the interval value is reached the count is reset "
                          "to zero and the next colour in the map is used.\n\n"
                          "Colour values are accumulated every time a point is "
                          "visited by an orbit, the colour used for the final"
                          "final image is the accumulated colour divided by the "
                          "number of visits or the background colour for points "
                          "that have not been visited.\n\n"
                          "Note: changing any of the settings including the "
                          "colour map will cause calculation to restart.");
    m_info_label->setWordWrap(true);
    m_info_label->setContentsMargins(5, 0, 0, 5);
    main_layout->addWidget(m_info_label);
    
    setLayout(main_layout);
}

void OrbitColouring::connect_signals()
{
    connect(m_method_combo, SIGNAL(currentIndexChanged(int)),
            this,           SLOT(on_method_changed(int)));
    connect(m_red_offset_spin, SIGNAL(valueChanged(int)),
            this,          SLOT(on_red_offset_changed(int)));
    connect(m_green_offset_spin, SIGNAL(valueChanged(int)),
            this,          SLOT(on_green_offset_changed(int)));
    connect(m_blue_offset_spin, SIGNAL(valueChanged(int)),
            this,          SLOT(on_blue_offset_changed(int)));
    connect(m_lockstep_check, SIGNAL(stateChanged(int)),
            this,          SLOT(on_lockstep_changed(int)));
    connect(m_interval_spin, SIGNAL(valueChanged(int)),
            this,          SLOT(on_interval_changed(int)));
    connect(m_scale_entry, SIGNAL(changed()),
            this,          SLOT(on_scale_changed())); 
    connect(m_component_order_combo, SIGNAL(currentIndexChanged(int)),    
            this,          SLOT(on_component_colour_order_changed(int)));
    connect(m_background,  SIGNAL(changed(const QColor&)),
            this,          SLOT(on_background_colour_changed(const QColor&)));    
    connect(m_use_colour_map_check, SIGNAL(stateChanged(int)),
            this,          SLOT(on_background_enabled(int)));
}

void OrbitColouring::setFractalType(FRACTAL_TYPE ft)
{
    m_config.SetFractalType(ft);
}

void OrbitColouring::display()
{
    m_ignore_signals = true;
    setOffset();
    ORBIT_COLOUR type;
    char ch = m_config.getOrbitColourType();
    switch(ch)
    {
    default:
        type = OC_ACCUMULATION;
        break;
    case 'a':
    case 'b':
    case 'c':
    case 'd':
        type = static_cast<ORBIT_COLOUR>(ch - 'a');
        break;
    }
    if (type == OC_AVERAGE)
    {
        m_interval_spin->setValue(m_config.getOrbitColourInterval());
    }
    else if (type != OC_ACCUMULATION)
    {
        m_scale_entry->Set(longdoubleToString(m_config.getOrbitScale()));
    }
    (type == OC_AVERAGE ? m_interval_widget->show() : m_interval_widget->hide());
    ((type == OC_SQRT) || (type == OC_LOG)  ? m_scale_widget->show() : m_scale_widget->hide());
    (type == OC_AVERAGE ? m_info_label->show() : m_info_label->hide());
    m_method_combo->setCurrentIndex(type);
    m_component_order_combo->setValue(m_config.getComponentOrder());
    if ((type == OC_SQRT) || (type == OC_LOG))
    {
        bool background = m_config.hasOrbitBackground();
        m_use_colour_map_check->setCheckState(background ? Qt::Unchecked : Qt::Checked);
        m_use_colour_map_check->setEnabled(true);
        m_background->setEnabled(background);
        if (background)
        {
            m_background->setColour(m_config.getOrbitBackground());
        }
    }
    else
    {
        m_use_colour_map_check->setCheckState(Qt::Unchecked);
        m_use_colour_map_check->setEnabled(false);
        m_background->setEnabled(true);
        if (!m_config.hasOrbitBackground())
        {
            m_config.enableOrbitBackground(true);
            m_config.setOrbitBackground(QColor(0, 0, 0));
        }
        m_background->setColour(m_config.getOrbitBackground());
    }
    m_ignore_signals = false;
}

void OrbitColouring::on_method_changed(int index)
{
    if (!m_ignore_signals)
    {
        char old_type = m_config.getOrbitColourType();
        char type = static_cast<char>(index) + 'a';
        if (old_type != type)
        {
            m_config.setOrbitColourType(type);
            emit orbit_colouring_changed();
        }
    }
}

void OrbitColouring::on_interval_changed(int value)
{
    if (!m_ignore_signals)
    {
        m_config.setOrbitColourInterval(value);
        emit orbit_colouring_changed();
    }
}

void OrbitColouring::on_red_offset_changed(int value)
{
    if (!m_ignore_signals)
    {
        int r = m_config.getRedOffset();
        int g = m_config.getGreenOffset();
        int b = m_config.getBlueOffset();
        int change = value - r;
        if (change < 0)
        {
            change += 512;
        }
        m_config.setRedOffset(value);
        if (m_lockstep)
        {
            m_config.setGreenOffset(g + change);
            m_config.setBlueOffset(b + change);
        }
        display();
        emit orbit_colouring_changed();
    }
}

void OrbitColouring::on_blue_offset_changed(int value)
{
    if (!m_ignore_signals)
    {
        int r = m_config.getRedOffset();
        int g = m_config.getGreenOffset();
        int b = m_config.getBlueOffset();
        int change = value - b;
        if (change < 0)
        {
            change += 512;
        }
        m_config.setBlueOffset(value);
        if (m_lockstep)
        {
            m_config.setGreenOffset(g + change);
            m_config.setRedOffset(r + change);
        }
        display();
        emit orbit_colouring_changed();
    }
}

void OrbitColouring::on_green_offset_changed(int value)
{
    if (!m_ignore_signals)
    {
        int r = m_config.getRedOffset();
        int g = m_config.getGreenOffset();
        int b = m_config.getBlueOffset();
        int change = value - g;
        if (change < 0)
        {
            change += 512;
        }
        m_config.setGreenOffset(value);
        if (m_lockstep)
        {
            m_config.setBlueOffset(b + change);
            m_config.setRedOffset(r + change);
        }
        display();
        emit orbit_colouring_changed();
    }
}

void OrbitColouring::on_lockstep_changed(int state)
{
    m_lockstep = (state == Qt::Checked);
}

void OrbitColouring::on_scale_changed()
{
    if (!m_ignore_signals)
    {
        long double scale = stringToLongDouble(m_scale_entry->Get());
        m_config.setOrbitScale(scale);
        emit orbit_colouring_changed();
    }
}

void OrbitColouring::on_component_colour_order_changed(int)
{
    if (!m_ignore_signals)
    {
        m_config.setComponentOrder(m_component_order_combo->getValue());
        emit orbit_colouring_changed();
    }
}

void OrbitColouring::on_background_enabled(int state)
{
    if (!m_ignore_signals)
    {
        bool use_colour_map = (state == Qt::Checked);
        m_config.enableOrbitBackground(!use_colour_map);
        display();
        emit orbit_colouring_changed();
    }
}

void OrbitColouring::on_background_colour_changed(const QColor& colour)
{
    if (!m_ignore_signals)
    {
        m_config.setOrbitBackground(colour);
        emit orbit_colouring_changed();
    }
}

void OrbitColouring::setOffset()
{
    m_red_offset_spin->setValue(m_config.getRedOffset());
    m_green_offset_spin->setValue(m_config.getGreenOffset());
    m_blue_offset_spin->setValue(m_config.getBlueOffset());
}

