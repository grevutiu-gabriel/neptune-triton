// 
// File:   ComplexParameterContainer.h
// Author: M.R.Eggleston
//
// Created on 18 March 2014, 08:33
//

#include <QVBoxLayout>
#include "CriticalPointsContainer.h"
#include "Utilities.h"

CriticalPointsContainer::CriticalPointsContainer(QWidget* parent)
:
QWidget(parent),
m_list(new QListWidget()),
m_ignore_check(new QCheckBox("ignore")),
m_main_widget(new QWidget),
m_emit(false),
m_row(0)
{
    setup_ui_layout();
    connect_signals();
}

CriticalPointsContainer::~CriticalPointsContainer()
{
}

void CriticalPointsContainer::setup_ui_layout()
{
    auto main_layout = new QVBoxLayout();
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->addWidget(m_list);
    auto layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_ignore_check);
    main_layout->addLayout(layout);
    m_main_widget->setLayout(main_layout);
    auto vlayout = new QVBoxLayout();
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->addWidget(m_main_widget);
    setLayout(vlayout);
}

void CriticalPointsContainer::connect_signals()
{
    connect(m_list, SIGNAL(currentRowChanged(int)),
            this,   SLOT(on_row_changed(int)));
    connect(m_ignore_check, SIGNAL(toggled(bool)),
            this,           SLOT(on_ignore_check_changed(bool)));
}


void CriticalPointsContainer::load(FRACTAL_TYPE ft)
{
    m_load_in_progress = true;
    m_row = m_list->currentRow();
    if (m_row < 0)
    {
        m_row = 0;
    }
    m_list->clear();
    m_config.SetFractalType(ft);
    CriticalPointFinder<LongComplex> cp_finder(m_config.getFractalConfig(false));
    m_cp_finder = cp_finder;
    m_cp_finder.find();
    unsigned int number_of_critical_points = m_config.getNumberOfCriticalPoints();
    for (unsigned int i = 0; i < number_of_critical_points; i++)
    {
        QString str = " ";
        str += longComplexToString(m_cp_finder.value(i));
        if (m_config.ignoreCriticalPoint(i))
        {
            str += " ignore";
        }
        m_list->addItem(str);
    }
    m_emit = false;
    m_list->setCurrentRow(m_row);
    m_emit = true;
    displayValueAtCursor();
    m_load_in_progress = false;
}

void CriticalPointsContainer::displayValueAtCursor()
{
    // display checkbox -> i.e. is critical value to be ignored?
    m_emit = false;
    int row = m_list->currentRow();
    if (row >= 0)
    {
        m_row = row;
        bool ignore = m_config.ignoreCriticalPoint(m_row);
        m_ignore_check->setCheckState(ignore ? Qt::Checked : Qt::Unchecked);
    }
    m_emit = true;
}

void CriticalPointsContainer::on_row_changed(int row)
{
    if (row != -1)
    {
        displayValueAtCursor();
    }
}


void CriticalPointsContainer::on_ignore_check_changed(bool checked)
{
    if (m_emit)
    {
        // At least one critical point must be used
        auto ignore = m_config.ignoreCriticalPoint(m_row);
        if (ignore != checked)
        {
            m_config.setIgnoreCriticalPoint(m_row, checked);
            unsigned int points = m_config.getNumberOfCriticalPoints();
            unsigned int in_use = 0;
            for (unsigned int i = 0; i < points; i++)
            {
                in_use += (m_config.ignoreCriticalPoint(i) ? 0 : 1);
            }
            if (in_use == 0)
            {
                // Use the next critical point
                int point = (m_row + 1) % points;
                m_config.setIgnoreCriticalPoint(point, false);
                displayParameterInList(point);
            }
            displayParameterInList(m_row);
            emit changed();
        }
    }
}

void CriticalPointsContainer::displayParameterInList(int row)
{
    auto item = m_list->item(row);
    if (item != nullptr)
    {
        QString str = " ";
        str += longComplexToString(m_cp_finder.value(row));
        if (m_config.ignoreCriticalPoint(row))
        {
            str += " ignore";
        }
        item->setText(str);
    }
}

