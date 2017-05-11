//
// File:   FractalTab.cc
// Author: M.R.Eggleston
//
// Created on 29 January 2014, 14:07
//

#include <iostream>
#include <QVBoxLayout>
#include "Config.h"
#include "FractalTab.h"
#include "Utilities.h"

FractalTab::FractalTab(QWidget* parent)
:
QWidget(parent),
m_bailout_widget(new QWidget()),
m_parameter_defaults_button(new QPushButton("Defaults")),
m_formula_text(new QTextEdit()),
m_parameters_container(new ComplexParametersContainer()),
m_critical_points_container(new CriticalPointsContainer()),
m_orbit_check(new QCheckBox("plot")),
m_calculating_area_spin(new QSpinBox()),
m_orbit_plot_combo(new OrbitPlotCombobox()),
m_omit_spin(new QSpinBox()),
m_density_spin(new QSpinBox()),
m_settling_cycles_entry(new NumberEntry(NumberEntry::UINT)),
m_sequence_line(new QLineEdit),
m_bailout_condition(new BailoutCondition()),
m_fractal_type(Config::instance()->getFractalType()),
m_emit_colour_change(true),
m_enable_fractal_settings_changed_signal(false)
{
    m_density_spin->setRange(1, 400);
    m_config.SetFractalType(m_fractal_type);
    setup_ui_layout();
    connect_signals();
    
    display();
}

FractalTab::~FractalTab()
{
}

void FractalTab::setup_ui_layout()
{
    setup_bailout_layout();

    auto main_layout = new QVBoxLayout();
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->addWidget(m_bailout_widget);
    setLayout(main_layout);
}

void FractalTab::setup_bailout_layout()
{
    // Background for location QTexyEdit fields
    auto background = palette().window().color();
    QPalette palette;
    palette.setColor(QPalette::Base, background);

    auto bailout_layout = new QVBoxLayout();
    bailout_layout->setSpacing(0);
    m_formula_text->setReadOnly(true);
    m_formula_text->setFrameShape(QFrame::NoFrame);
    m_formula_text->setFrameShadow(QFrame::Plain);
    m_formula_text->setPalette(palette);
    m_formula_text->setFixedHeight(120);
    bailout_layout->addWidget(m_formula_text);
    m_bailout_condition->load(m_config.GetFractalType());
    bailout_layout->addWidget(m_bailout_condition);
    auto tabs = new QTabWidget();
    tabs->setFixedHeight(250);
    tabs->addTab(m_parameters_container, "Parameters");
    tabs->addTab(m_critical_points_container, "Critical Points");
    bailout_layout->addWidget(tabs);
    auto layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addStretch();
    layout->addWidget(m_parameter_defaults_button);
    bailout_layout->addLayout(layout);
    layout = new QHBoxLayout();
    layout->setSpacing(5);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_orbit_check);
    layout->addWidget(m_orbit_plot_combo);
    auto label = new QLabel("orbits omitting the first");
    layout->addWidget(label);
    layout->addWidget(m_omit_spin);
    m_omit_spin->setRange(0, (m_config.getMaxIterations() > 0 ? m_config.getMaxIterations() - 1 : 0));
    m_omit_spin->setValue(m_config.getStartOrbitIteration());
    label = new QLabel("iterations");
    layout->addWidget(label);
    layout->addStretch();
    bailout_layout->addLayout(layout);    
    layout = new QHBoxLayout();
    layout->setSpacing(5);
    layout->setContentsMargins(0, 0, 0, 0);
    label = new QLabel("Calculating area");
    layout->addWidget(label);
    m_calculating_area_spin->setRange(1, 1000);
    m_calculating_area_spin->setValue(9);
    layout->addWidget(m_calculating_area_spin);
    label = new QLabel("times display area");
    layout->addWidget(label);
    layout->addStretch();
    bailout_layout->addLayout(layout);
    layout = new QHBoxLayout();
    layout->setSpacing(5);
    layout->setContentsMargins(0, 0, 0, 0);
    label = new QLabel("Density");
    layout->addWidget(label);
    layout->addWidget(m_density_spin);
    layout->addStretch();
    bailout_layout->addLayout(layout);
    bailout_layout->addStretch();
    m_bailout_widget->setLayout(bailout_layout);
}

void FractalTab::connect_signals()
{
    connect(m_bailout_condition, SIGNAL(changed()),
            this,                SLOT(on_changed()));
    connect(m_calculating_area_spin, SIGNAL(valueChanged(int)),
            this,                SLOT(on_area_changed(int)));
    connect(m_omit_spin,         SIGNAL(valueChanged(int)),
            this,                SLOT(on_omit_changed(int)));
    connect(m_orbit_check,       SIGNAL(stateChanged(int)),
            this,                SLOT(on_orbit_check(int)));
    connect(m_orbit_plot_combo,  SIGNAL(currentIndexChanged(int)),
            this,                SLOT(on_orbit_plot_changed(int)));
    connect(m_parameter_defaults_button, SIGNAL(pressed()),
            this,                SLOT(on_bailout_defaults()));
    connect(m_density_spin,      SIGNAL(valueChanged(int)),
            this,                SLOT(on_density_changed(int)));
    connect(m_critical_points_container, SIGNAL(changed()),
            this,                SLOT(on_changed()));
    connect(m_parameters_container, SIGNAL(changed()),
            this,                SLOT(on_changed()));
}

void FractalTab::on_changed()
{
    emit config_changed();
}

void FractalTab::display()
{
    m_enable_fractal_settings_changed_signal = false;
    display_bailout_fractal();
    m_enable_fractal_settings_changed_signal = true;
}

void FractalTab::display_bailout_fractal()
{
    m_formula_text->setHtml(m_config.getFormula());
    m_bailout_condition->load(m_fractal_type);
    bool orbit_plotting = m_config.isOrbitPlotting();
    m_orbit_check->setCheckState(orbit_plotting ? Qt::Checked : Qt::Unchecked);
    m_calculating_area_spin->setValue(orbit_plotting ? m_config.getCalculatingArea() : 9);
    m_omit_spin->setRange(0, (m_config.getMaxIterations() > 0 ? m_config.getMaxIterations() - 1 : 0));
    m_omit_spin->setValue(orbit_plotting ? m_config.getStartOrbitIteration() : 0);
    m_orbit_plot_combo->setValue(orbit_plotting ? m_config.getOrbitPlots() : OP_ALL);
    m_density_spin->setValue(m_config.getDensity());
    m_calculating_area_spin->setEnabled(orbit_plotting);
    m_omit_spin->setEnabled(orbit_plotting);
    m_orbit_plot_combo->setEnabled(orbit_plotting);
    m_density_spin->setEnabled(orbit_plotting);
    
    m_parameters_container->load(m_fractal_type);
    m_critical_points_container->load(m_fractal_type);
}

void FractalTab::setFractalType(FRACTAL_TYPE ft)
{
    m_fractal_type =  ft;
    m_config.SetFractalType(m_fractal_type);
    display();
}

void FractalTab::on_bailout_defaults()
{
    m_config.resetFractalParameters();
    display();
    emit config_changed();
}

void FractalTab::on_orbit_check(int index)
{
    if (m_enable_fractal_settings_changed_signal)
    {
        bool orbit_plotting = (static_cast<Qt::CheckState>(index) == Qt::Checked);
        m_config.setOrbitPlotting(orbit_plotting);
        m_calculating_area_spin->setEnabled(orbit_plotting);
        m_omit_spin->setEnabled(orbit_plotting);
        m_orbit_plot_combo->setEnabled(orbit_plotting);
        m_density_spin->setEnabled(orbit_plotting);
        emit config_changed();
    }
}

void FractalTab::on_area_changed(int value)
{
    if (m_enable_fractal_settings_changed_signal)
    {
        m_config.setCalculatingArea(value);
        emit config_changed();
    }
}

void FractalTab::on_omit_changed(int value)
{
    if (m_enable_fractal_settings_changed_signal)
    {
        m_config.setStartOrbitIteration(value);
        emit config_changed();
    }
}

void FractalTab::on_density_changed(int value)
{
    if (m_enable_fractal_settings_changed_signal)
    {
        m_config.setDensity(value);
        emit config_changed();
    }
}

void FractalTab::on_orbit_plot_changed(int value)
{
    auto old_value = m_config.getOrbitPlots();
    auto new_value = static_cast<ORBIT_PLOTS>(value);
    if (old_value != new_value)
    {
        m_config.setOrbitPlots(new_value);
        if (m_enable_fractal_settings_changed_signal)
        {
            emit config_changed();
        }
    }
}

void FractalTab::startOrbitIterationRangeChanged(int value)
{
    m_enable_fractal_settings_changed_signal = false;
    m_omit_spin->setRange(0, value);
    m_enable_fractal_settings_changed_signal = true;
}
