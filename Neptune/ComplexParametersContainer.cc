// 
// File:   ComplexParameterContainers.cc
// Author: M.R.Eggleston
// 
// Created on 24 April 2012, 16:16
//

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include "ComplexParametersContainer.h"
#include "Utilities.h"

ComplexParametersContainer::ComplexParametersContainer(QWidget* parent)
:
QWidget(parent),
m_list(new QListWidget()),
m_real_entry(new NumberEntry(NumberEntry::FLOAT)),
m_imag_entry(new NumberEntry(NumberEntry::FLOAT)),
m_main_widget(new QWidget),
m_no_complex_parameters_label(new QLabel("no complex parameters defined")),
m_emit(false),
m_row(0)
{
    setup_ui_layout();
    connect_signals();
}

ComplexParametersContainer::~ComplexParametersContainer()
{
}

void ComplexParametersContainer::setup_ui_layout()
{
    auto main_layout = new QVBoxLayout();
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->addWidget(m_list);
    auto layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(new QLabel("r"));
    layout->addWidget(m_real_entry);
    layout->addWidget(new QLabel("i"));
    layout->addWidget(m_imag_entry);
    main_layout->addLayout(layout);
    m_main_widget->setLayout(main_layout);
    auto vlayout = new QVBoxLayout();
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->addWidget(m_main_widget);
    m_no_complex_parameters_label->hide();
    vlayout->addWidget(m_no_complex_parameters_label);
    setLayout(vlayout);
}

void ComplexParametersContainer::connect_signals()
{
    connect(m_list, SIGNAL(currentRowChanged(int)),
            this,   SLOT(on_row_changed(int)));
    connect(m_real_entry, SIGNAL(changed()),
            this,   SLOT(on_parameter_changed()));
    connect(m_imag_entry, SIGNAL(changed()),
            this,   SLOT(on_parameter_changed()));
}


void ComplexParametersContainer::load(FRACTAL_TYPE ft)
{
    m_load_in_progress = true;
    m_row = m_list->currentRow();
    if (m_row < 0)
    {
        m_row = 0;
    }
    m_list->clear();
    m_config.SetFractalType(ft);
    int number_of_complex_parameters = m_config.getNumberOfComplexParameters();
    if (number_of_complex_parameters > 0)
    {
        m_main_widget->show();
        m_no_complex_parameters_label->hide();
    }
    else
    {
        m_main_widget->hide();
        m_no_complex_parameters_label->show();
    }
    uint complex_base = 0x03b1; // 'α'
    for (int i = 0; i < number_of_complex_parameters; i++, complex_base++)
    {
        QString str = " ";
        str += QChar(complex_base);
        str += " = ";
        str += complexNumberToString(m_config.getComplexParameter(i));
        m_list->addItem(str);
    }
    if (number_of_complex_parameters > 0)
    {
        m_emit = false;
        m_list->setCurrentRow(m_row);
        m_emit = true;
        displayValueAtCursor();
    }
    m_load_in_progress = false;
}

void ComplexParametersContainer::displayValueAtCursor()
{
    m_emit = false;
    int row = m_list->currentRow();
    if (row >= 0)
    {
        m_row = row;
        auto p = m_config.getComplexParameter(m_row);
        m_real_entry->Set(p.r);
        m_imag_entry->Set(p.i);
    }
    m_emit = true;
}

void ComplexParametersContainer::on_row_changed(int row)
{
    if (row != -1)
    {
        displayValueAtCursor();
    }
}


void ComplexParametersContainer::on_parameter_changed()
{
    if (m_emit)
    {
        if (updateParameter())
        {
            emit changed();
        }
    }
}

void ComplexParametersContainer::on_parameter_state_changed(int)
{
    on_parameter_changed();
}

bool ComplexParametersContainer::updateParameter()
{
    bool changed = false;
    auto p = m_config.getComplexParameter(m_row);
    std::string real = m_real_entry->Get();
    std::string imag = m_imag_entry->Get();
    changed = ((real != p.r) || (imag != p.i));
    if (changed)
    {
        m_config.setComplexParameter(m_row, StringComplex(real, imag));
        displayParameterInList(m_row);
    }
    displayValueAtCursor();
    return changed;
}

void ComplexParametersContainer::displayParameterInList(int row)
{
    auto item = m_list->item(row);
    if (item != nullptr)
    {
        uint complex_base = 0x03b1; // 'α'
        QString str = " ";
        str += QChar(complex_base + m_row);
        str += " = ";
        str += complexNumberToString(m_config.getComplexParameter(m_row));
        item->setText(str);
    }
}