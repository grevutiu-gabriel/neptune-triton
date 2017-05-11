// 
// File:   BailoutColouring.h
// Author: M.R.Eggleston
//
// Created on 04 April 2013, 11:21
//

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include "BailoutColouring.h"
#include "Config.h"

BailoutColouring::BailoutColouring(bool inner, QWidget* parent)
:
QWidget(parent),
m_method_combo(new MethodCombobox()),
m_statistics_options_combo(new StatisticsOptionTypeCombobox()),
m_statistics_widget(new QWidget()),
m_final_magnitude_combo(new FinalMagnitudeTypeCombobox()),
m_final_magnitude_widget(new QWidget()),
m_quotient_combo(new RatioPartTypeCombobox()),
m_divisor_combo(new RatioPartTypeCombobox()),
m_ratio_widget(new QWidget()),
m_gaussian_type_combo(new GaussianTypeCombobox()),
m_gaussian_method_combo(new GaussianMethodCombobox()),
m_gaussian_widget(new QWidget()),
m_single_colour(new SingleColour(CT_ALL)),
m_multi_colour(new QWidget()),
m_red_offset_spin(new QSpinBox()),
m_green_offset_spin(new QSpinBox()),
m_blue_offset_spin(new QSpinBox()),
m_lockstep_check(new QCheckBox("lock step")),
m_smooth_check(new QCheckBox("smooth")),
m_abs_log_check(new QCheckBox("absolute log of result value")),
m_scale_widget(new QWidget()),
m_scale_entry(new NumberEntry(NumberEntry::FLOAT)),
m_trap(new Trap(inner)),
m_component_order_combo(new ComponentOrderComboBox()),
m_component_order_widget(new QWidget()),
m_colour_maps(ColourMaps::instance()),
m_ignore_signals(false),
m_inner(inner),
m_lockstep(true),
m_map_cursor(false)
{
    m_config.SetFractalType(Config::instance()->getFractalType());
    m_config.setInner(m_inner);
    m_red_offset_spin->setRange(0, 511);
    m_red_offset_spin->setWrapping(true);
    m_green_offset_spin->setRange(0, 511);
    m_green_offset_spin->setWrapping(true);
    m_blue_offset_spin->setRange(0, 511);
    m_blue_offset_spin->setWrapping(true);
    m_lockstep_check->setCheckState(Qt::Checked);
    setup_ui_layout();
    connect_signals();
}

BailoutColouring::~BailoutColouring()
{
}

void BailoutColouring::setup_ui_layout()
{
    auto main_layout = new QVBoxLayout();
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    auto hlayout = new QHBoxLayout();
    hlayout->setSpacing(5);
    hlayout->setContentsMargins(0, 0, 0, 0);
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
    //
    m_single_colour->hide();
    main_layout->addWidget(m_single_colour);
    //
    auto vlayout = new QVBoxLayout();
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 0, 0, 0);
    hlayout = new QHBoxLayout();
    hlayout->setSpacing(5);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->addWidget(new QLabel("Offsets  red"));
    hlayout->addWidget(m_red_offset_spin);
    hlayout->addWidget(new QLabel("green"));
    hlayout->addWidget(m_green_offset_spin);
    hlayout->addWidget(new QLabel("blue"));
    hlayout->addWidget(m_blue_offset_spin);
    hlayout->addWidget(m_lockstep_check);
    hlayout->addStretch();
    vlayout->addLayout(hlayout);
    hlayout = new QHBoxLayout();
    hlayout->setSpacing(5);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->addWidget(new QLabel("Scale"));
    hlayout->addWidget(m_scale_entry);
    hlayout->addStretch();
    m_scale_widget->setLayout(hlayout);
    vlayout->addWidget(m_scale_widget);
    hlayout = new QHBoxLayout();
    hlayout->setSpacing(5);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->addWidget(m_smooth_check);
    hlayout->addWidget(m_abs_log_check);
    hlayout->addStretch();
    vlayout->addLayout(hlayout);
    hlayout = new QHBoxLayout();
    hlayout->setSpacing(5);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->addWidget(m_final_magnitude_combo);
    hlayout->addStretch();
    m_final_magnitude_widget->setLayout(hlayout);
    vlayout->addWidget(m_final_magnitude_widget);
    hlayout = new QHBoxLayout();
    hlayout->setSpacing(5);
    hlayout->setContentsMargins(0, 0, 0, 0);
    auto vlayout2 = new QVBoxLayout();
    vlayout2->setSpacing(0);
    vlayout2->setContentsMargins(0, 0, 0, 0);
    auto label = new QLabel("Quotient");
    label->setAlignment(Qt::AlignHCenter);
    vlayout2->addWidget(label);
    vlayout2->addWidget(m_quotient_combo);
    label = new QLabel("Divisor");
    label->setAlignment(Qt::AlignHCenter);
    vlayout2->addWidget(label);
    vlayout2->addWidget(m_divisor_combo);
    hlayout->addLayout(vlayout2);
    hlayout->addStretch();
    m_ratio_widget->setLayout(hlayout);
    vlayout->addWidget(m_ratio_widget);
    hlayout = new QHBoxLayout();
    hlayout->setSpacing(5);
    hlayout->setContentsMargins(0, 0, 0, 0);
    vlayout2 = new QVBoxLayout();
    vlayout2->setSpacing(0);
    vlayout2->setContentsMargins(0, 0, 0, 0);
    hlayout2 = new QHBoxLayout;
    hlayout->setSpacing(5);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout2->addWidget(m_gaussian_type_combo);
    hlayout2->addStretch();
    vlayout2->addLayout(hlayout2);
    hlayout2 = new QHBoxLayout;
    hlayout->setSpacing(5);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout2->addWidget(m_gaussian_method_combo);
    hlayout2->addStretch();
    vlayout2->addLayout(hlayout2);
    m_gaussian_widget->setLayout(vlayout2);
    vlayout->addWidget(m_gaussian_widget);
    hlayout = new QHBoxLayout();
    hlayout->setSpacing(5);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->addWidget(m_statistics_options_combo);
    hlayout->addStretch();
    m_statistics_widget->setLayout(hlayout);
    vlayout->addWidget(m_statistics_widget);
    vlayout->addWidget(m_trap);
    m_multi_colour->setLayout(vlayout);
    main_layout->addWidget(m_multi_colour);
    
    setLayout(main_layout);
}

void BailoutColouring::connect_signals()
{
    connect(m_method_combo, SIGNAL(currentIndexChanged(int)),
            this,           SLOT(on_method_changed(int)));
    connect(m_smooth_check, SIGNAL(stateChanged(int)),
            this,           SLOT(on_smooth_colouring_changed(int)));
    connect(m_abs_log_check, SIGNAL(stateChanged(int)),
            this,            SLOT(on_abs_log_changed(int)));
    connect(m_red_offset_spin, SIGNAL(valueChanged(int)),
            this,          SLOT(on_red_offset_changed(int)));
    connect(m_green_offset_spin, SIGNAL(valueChanged(int)),
            this,          SLOT(on_green_offset_changed(int)));
    connect(m_blue_offset_spin, SIGNAL(valueChanged(int)),
            this,          SLOT(on_blue_offset_changed(int)));
    connect(m_lockstep_check, SIGNAL(stateChanged(int)),
            this,          SLOT(on_lockstep_changed(int)));
    connect(m_scale_entry, SIGNAL(changed()),
            this,          SLOT(on_scale_changed()));
    connect(m_statistics_options_combo, SIGNAL(currentIndexChanged(int)),
            this,          SLOT(on_statistics_option_changed(int)));
    connect(m_final_magnitude_combo,  SIGNAL(currentIndexChanged(int)),
            this,          SLOT(on_final_magnitude_changed(int)));
    connect(m_quotient_combo, SIGNAL(currentIndexChanged(int)),
            this,          SLOT(on_quotient_changed(int)));
    connect(m_divisor_combo, SIGNAL(currentIndexChanged(int)),
            this,          SLOT(on_divisor_changed(int)));
    connect(m_gaussian_type_combo, SIGNAL(currentIndexChanged(int)),
            this,          SLOT(on_gaussian_type_changed(int)));
    connect(m_gaussian_method_combo, SIGNAL(currentIndexChanged(int)),
            this,          SLOT(on_gaussian_method_changed(int)));
    connect(m_trap,        SIGNAL(trap_changed()),
            this,          SLOT(on_trap_changed()));
    connect(m_component_order_combo, SIGNAL(currentIndexChanged(int)),    
            this,          SLOT(on_component_colour_order_changed(int)));
}

void BailoutColouring::setFractalType(FRACTAL_TYPE ft)
{
    m_config.SetFractalType(ft);
    m_trap->SetFractalType(ft);
    display();
}

void BailoutColouring::display()
{
    m_ignore_signals = true;
    auto method = colouringMethod();
    m_method_combo->setValue(method);
    
    if (method == FIXED_COLOUR)
    {
        m_map_cursor = false;
        m_multi_colour->hide();
        m_single_colour->show();
        QColor colour(stringToColour(m_config.getMethod().substr(2)));
        m_single_colour->setColour(colour);
        m_component_order_widget->hide();
    }
    else
    {
        bool show_statistics = false;
        bool iteration = (method == ITERATION);
        bool ratio = ((method == RATIO) || (method == RATIO_CHANGE));
        bool trap = ((method == ORBIT_TRAP) || (method == ORBIT_TRAP_CHANGE));
        m_map_cursor = true;
        m_single_colour->hide();
        m_multi_colour->show();
        setOffset();
        m_smooth_check->setCheckState(m_config.getSmooth() ? Qt::Checked : Qt::Unchecked);
        m_smooth_check->setEnabled(true);
        if (!iteration)
        {
            setScale();
            m_abs_log_check->setCheckState(m_config.getAbsLog() ? Qt::Checked : Qt::Unchecked);
            m_abs_log_check->setEnabled(true);
        }
        switch (method)
        {
        case FINAL_MAGNITUDE:
            setMagnitudeType();
            break;
        case MAGNITUDE:
            show_statistics = true;
            setStatisticsOption(1);
            break;
        case ANGLE:
        case ANGLE_CHANGE:
        case MAGNITUDE_CHANGE:
            show_statistics = true;
            setStatisticsOption(1);
            break;
        case GAUSSIAN:
            setGaussian();
            break;
        case ORBIT_TRAP:
        case ORBIT_TRAP_CHANGE:
            setTrap();
            show_statistics = true;
            break;
        case RATIO:
            setRatio();
            show_statistics = true;
            setStatisticsOption(3);
            break;
        case RATIO_CHANGE:
            setRatio();
            show_statistics = true;
            setStatisticsOption(3);
            break;
        default:
            break;
        }
        (iteration ? m_scale_widget->hide() : m_scale_widget->show());
        (method == FINAL_MAGNITUDE ? m_final_magnitude_widget->show() : m_final_magnitude_widget->hide());
        (method == GAUSSIAN ? m_gaussian_widget->show() : m_gaussian_widget->hide());
        (show_statistics ? m_statistics_widget->show() : m_statistics_widget->hide());
        (iteration ? m_abs_log_check->hide() : m_abs_log_check->show());
        (trap ? m_trap->show() : m_trap->hide());
        (ratio ? m_ratio_widget->show() : m_ratio_widget->hide());
        m_component_order_combo->setValue(m_config.getComponentOrder());
        m_component_order_widget->show();
    }
    m_ignore_signals = false;
}
    
METHOD BailoutColouring::colouringMethod()
{
    METHOD method;
    std::string smethod = m_config.getMethod();
    switch (smethod[0])
    {
    case 'f':
        method = FIXED_COLOUR;
        break;
    default:
    case 'i':
        method = ITERATION;
        break;
    case 'm':
        method = FINAL_MAGNITUDE;
        break;
    case 'M':
        method = (smethod[1] < 'a' ? MAGNITUDE_CHANGE : MAGNITUDE);
        break;
    case 'a':
        method = FINAL_ANGLE;
        break;
    case 'A':
        method = (smethod[1] < 'a' ? ANGLE_CHANGE : ANGLE);
        break;
    case 'g':
        method = GAUSSIAN;
        break;
    case 't':
        method = (smethod[2] < 'a' ? ORBIT_TRAP_CHANGE : ORBIT_TRAP);
        break;
    case'r':
        method = (smethod[3] < 'a' ? RATIO_CHANGE : RATIO);
        break;
    }
    return method;
}

void BailoutColouring::setGaussian()
{
    std::string method = m_config.getMethod();
    GAUSSIAN_TYPE gtype;
    switch (method[1])
    {
    default:
    case 'r':
        gtype = ROUND;
        break;
    case 'c':
        gtype = CEILING;
        break;
    case 'f':
        gtype = FLOOR;
        break;
    case 't':
        gtype = TRUNCATE;
        break;
    }
    m_gaussian_type_combo->setValue(gtype);
    auto value = static_cast<GAUSSIAN_METHOD>(method[2] - 'a');
    m_gaussian_method_combo->setValue(value);
    bool enable = (value != MIN_DISTANCE_ITERATION) && (value != MAX_DISTANCE_ITERATION);
    m_smooth_check->setCheckState(enable && m_config.getSmooth() ? Qt::Checked : Qt::Unchecked);
    m_abs_log_check->setCheckState(enable && m_config.getAbsLog() ? Qt::Checked : Qt::Unchecked);
    m_smooth_check->setEnabled(enable);
    m_abs_log_check->setEnabled(enable);
}

void BailoutColouring::setMagnitudeType()
{
    std::string method = m_config.getMethod();
    FINAL_MAGNITUDE_TYPE type;
    switch (method[1])
    {
    default:
    case 'n':
        type = MAG_NORM;
        break;
    case 's':
        type = MAG_SMALLER;
        break;
    case 'g':
        type = MAG_GREATER;
        break;
    case 'r':
        type = MAG_REAL;
        break;
    case 'R':
        type = MAG_ABS_REAL;
        break;
    case 'i':
        type = MAG_IMAG;
        break;
    case 'I':
        type = MAG_ABS_IMAG;
        break;
    case 'c':
        type = MAG_COMBINED;
        break;
    case 'C':
        type = MAG_ABS_COMBINED;
        break;
    case 'p':
        type = MAG_PRODUCT;
        break;
    case 'P':
        type = MAG_ABS_PRODUCT;
        break;
    }
    m_final_magnitude_combo->setValue(type);
}

void BailoutColouring::setRatio()
{
    std::string method = m_config.getMethod();
    RATIO_PART_TYPE q = static_cast<RATIO_PART_TYPE>(method[1] - 'a');
    RATIO_PART_TYPE d = static_cast<RATIO_PART_TYPE>(method[2] - 'a');
    m_quotient_combo->setValue(q);
    m_divisor_combo->setValue(d);
}

void BailoutColouring::setStatisticsOption(int pos)
{
    char option = m_config.getMethod()[pos];
    option = (option < 'a' ? option - 'A' : option - 'a');
    auto value = static_cast<STATISTICS_OPTION_TYPE>(option);
    m_statistics_options_combo->setValue(value);
    bool enable = (value != SO_ITER_AT_MINIMUM) && (value != SO_ITER_AT_MAXIMUM);
    m_smooth_check->setEnabled(enable && m_config.getSmooth() ? Qt::Checked : Qt::Unchecked);
    m_abs_log_check->setCheckState(enable && m_config.getAbsLog() ? Qt::Checked : Qt::Unchecked);
    m_scale_entry->setEnabled(enable);
    m_smooth_check->setEnabled(enable);
    m_abs_log_check->setEnabled(enable);
}

void BailoutColouring::setTrap()
{
    setStatisticsOption(2);
    m_trap->display();
}

void BailoutColouring::on_method_changed(int)
{
    if (!m_ignore_signals)
    {
        m_map_cursor = true;
        int scale = 1000;
        auto method = m_method_combo->getValue();
        std::string method_string;
        switch (method)
        {
        default:
        case ITERATION:
            method_string = "i";
            m_config.setSmooth(false);
            scale = 1;
            break;
        case FIXED_COLOUR:
            method_string = "f,0,0,0";
            scale = 1;
            m_map_cursor = false;
            break;
        case FINAL_MAGNITUDE:
            method_string = "mn";
            scale = 100;
            break;
        case FINAL_ANGLE:
            method_string = "a";
            break;
        case GAUSSIAN:
            method_string = "gra";
            break;
        case MAGNITUDE:
            method_string = "Ma";
            break;
        case ANGLE:
            method_string = "Aa";
            break;
        case ANGLE_CHANGE:
            method_string = "AA";
            break;
        case MAGNITUDE_CHANGE:
            method_string = "MA";
            break;
        case ORBIT_TRAP:
            method_string = "taa," + m_trap->asString();
            break;
        case ORBIT_TRAP_CHANGE:
            method_string = "taA," + m_trap->asString();
            break;
        case RATIO:
            method_string = "raba";
            break;
        case RATIO_CHANGE:
            method_string = "rabA";
            break;
        }
        m_config.setMethod(method_string);
        m_config.setColourScale(scale);
        m_config.setAbsLog(false);
        display();
        emit colouring_changed(m_map_cursor ? m_config.getColourMap() : "");
    }
}

void BailoutColouring::setOffset()
{
    m_red_offset_spin->setValue(m_config.getRedOffset());
    m_green_offset_spin->setValue(m_config.getGreenOffset());
    m_blue_offset_spin->setValue(m_config.getBlueOffset());
}

void BailoutColouring::setScale()
{
    m_scale_entry->setEnabled(true);
    m_scale_entry->Set(longdoubleToString(m_config.getColourScale()));
}

void BailoutColouring::on_smooth_colouring_changed(int state)
{
    if (!m_ignore_signals)
    {
        bool smooth = (state == Qt::Checked);
        m_config.setSmooth(smooth);
        emit colouring_changed(m_map_cursor ? m_config.getColourMap() : "");
    }
}

void BailoutColouring::on_abs_log_changed(int state)
{
    if (!m_ignore_signals)
    {
        bool abs_log = (state == Qt::Checked);
        m_config.setAbsLog(abs_log);
        emit colouring_changed(m_map_cursor ? m_config.getColourMap() : "");
    }
}

void BailoutColouring::on_red_offset_changed(int value)
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
        emit colouring_changed(m_map_cursor ? m_config.getColourMap() : "");
    }
}

void BailoutColouring::on_blue_offset_changed(int value)
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
        emit colouring_changed(m_map_cursor ? m_config.getColourMap() : "");
    }
}

void BailoutColouring::on_green_offset_changed(int value)
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
        emit colouring_changed(m_map_cursor ? m_config.getColourMap() : "");
    }
}

void BailoutColouring::on_lockstep_changed(int state)
{
    m_lockstep = (state == Qt::Checked);
}

void BailoutColouring::on_scale_changed()
{
    if (!m_ignore_signals)
    {
        long double scale = stringToLongDouble(m_scale_entry->Get());
        (m_config.isOrbitPlotting() ? m_config.setOrbitScale(scale) : m_config.setColourScale(scale));
        emit colouring_changed(m_map_cursor ? m_config.getColourMap() : "");
    }
}

void BailoutColouring::on_statistics_option_changed(int)
{
    if (!m_ignore_signals)
    {
        auto method = m_config.getMethod();
        char ch = static_cast<char>(m_statistics_options_combo->getValue());
        int position = 1;
        if (method[0] == 't') position = 2;
        if (method[0] == 'r') position = 3;
        ch = (method[position] < 'a' ? ch + 'A' : ch + 'a');
        method.replace(position, 1, 1, ch);
        m_config.setMethod(method);
        emit colouring_changed(m_map_cursor ? m_config.getColourMap() : "");
    }
}

void BailoutColouring::on_final_magnitude_changed(int)
{
    if (!m_ignore_signals)
    {
        std::string type;
        auto mag_type = m_final_magnitude_combo->getValue();
        switch (mag_type)
        {
        default:
        case MAG_NORM:
            type = "n";
            break;
        case MAG_SMALLER:
            type = "s";
            break;
        case MAG_GREATER:
            type = "g";
            break;
        case MAG_REAL:
            type = "r";
            break;
        case MAG_ABS_REAL:
            type = "R";
            break;
        case MAG_IMAG:
            type = "i";
            break;
        case MAG_ABS_IMAG:
            type = "I";
            break;
        case MAG_COMBINED:
            type = "c";
            break;
        case MAG_ABS_COMBINED:
            type = "C";
            break;
        case MAG_PRODUCT:
            type = "p";
            break;
        case MAG_ABS_PRODUCT:
            type = "P";
            break;
        }
        std::string method = m_config.getMethod();
        method.replace(1, 1, type);
        m_config.setMethod(method);
        emit colouring_changed(m_map_cursor ? m_config.getColourMap() : "");
    }
}

void BailoutColouring::on_quotient_changed(int)
{
    if (!m_ignore_signals)
    {
        std::string method = m_config.getMethod();
        char ch = static_cast<char>(m_quotient_combo->getValue());
        ch = (method[1] < 'a' ? ch + 'A' : ch + 'a');
        method.replace(1, 1, 1, ch);
        m_config.setMethod(method);
        emit colouring_changed(m_map_cursor ? m_config.getColourMap() : "");
    }
}

void BailoutColouring::on_divisor_changed(int)
{
    if (!m_ignore_signals)
    {
        std::string method = m_config.getMethod();
        char ch = static_cast<char>(m_divisor_combo->getValue());
        ch = (method[2] < 'a' ? ch + 'A' : ch + 'a');
        method.replace(2, 1, 1, ch);
        m_config.setMethod(method);
        emit colouring_changed(m_map_cursor ? m_config.getColourMap() : "");
    }
}

void BailoutColouring::on_gaussian_type_changed(int)
{
    if (!m_ignore_signals)
    {
        char ch;
        auto type = m_gaussian_type_combo->getValue();
        switch (type)
        {
        case ROUND:
        default:
            ch = 'r';
            break;
        case CEILING:
            ch = 'c';
            break;
        case FLOOR:
            ch = 'f';
            break;
        case TRUNCATE:
            ch = 't';
            break;
        }
        std::string method = m_config.getMethod();
        method.replace(1, 1, 1, ch);
        m_config.setMethod(method);
        emit colouring_changed(m_map_cursor ? m_config.getColourMap() : "");
    }
}

void BailoutColouring::on_gaussian_method_changed(int)
{
    if (!m_ignore_signals)
    {
        char ch = static_cast<char>(m_gaussian_method_combo->getValue() + static_cast<int>('a'));
        std::string method = m_config.getMethod();
        method.replace(2, 1, 1, ch);
        m_config.setMethod(method);
        emit colouring_changed(m_map_cursor ? m_config.getColourMap() : "");
    }
}

void BailoutColouring::on_trap_changed()
{
    if (!m_ignore_signals)
    {
        emit colouring_changed(m_map_cursor ? m_config.getColourMap() : "");
    }
}

void BailoutColouring::on_component_colour_order_changed(int)
{
    if (!m_ignore_signals)
    {
        m_config.setComponentOrder(m_component_order_combo->getValue());
        emit colouring_changed(m_map_cursor ? m_config.getColourMap() : "");
    }
}

const SingleColour* BailoutColouring::single_colour()
{
    return m_single_colour;
}
