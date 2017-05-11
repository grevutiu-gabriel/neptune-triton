//
// File:   FractalTree.cc
// Author: M.R.Eggleston
//
// Created on 06 July 2010, 16:10
//
// Converted to Qt March 2013 and renamed FractalList.cc

#include <iostream>
#include "FractalList.h"
#include "Config.h"

FractalList::FractalList(QWidget* parent)
:
QListView(parent),
m_model(nullptr),
m_size(0)
{
    auto fractal_id = static_cast<int>(Config::instance()->getFractalType());
    m_model = new QStandardItemModel(this);
    FractalsConfig::VectorFractalId fractal_index = m_config.getAllFractals();
    m_size = fractal_index.size();
    int cursor_row = 0;
    for (int i=0; i< m_size; i++)
    {
        QList<QStandardItem*> row;
        auto name = new QStandardItem(fractal_index[i].name);
        auto id = new QStandardItem(QString::number(fractal_index[i].type));
        row.append(name);
        row.append(id);
        m_model->appendRow(row);
    }
    m_model->sort(0);
    for (int i = 0; i < m_size; i++)
    {
        auto item = m_model->item(i, 1);
        if (item != nullptr)
        {
            int id = item->text().toInt();
            if (id == fractal_id)
            {
                cursor_row = i;
                break;
            }
        }
    }
    auto item = m_model->item(cursor_row);
    setModel(m_model);
    if (item != nullptr)
    {
        auto index = m_model->indexFromItem(item);
        setCurrentIndex(index);
        scrollTo(index);
    }
    else
    {
        scrollToTop();
    }
}

FractalList::~FractalList()
{
}

FRACTAL_TYPE FractalList::getFractalTypeAtCursor()
{
    auto row = currentIndex().row();
    auto item = m_model->item(row, 1);
    return static_cast<FRACTAL_TYPE>(item->text().toUInt());
}

void FractalList::setCursorToFractalType(FRACTAL_TYPE ft)
{
    bool changed = true;
    int cursor_row = 0;
    auto current_index = currentIndex();
    auto current_item = m_model->item(current_index.row(), 1);
    if (current_item != nullptr)
    {
        changed = (static_cast<int>(ft) != current_item->text().toInt());
    }
    if (changed)
    {
        for (int i = 0; i < m_size; i++)
        {
            auto item = m_model->item(i, 1);
            if (item != nullptr)
            {
                int id = static_cast<FRACTAL_TYPE>(item->text().toInt());
                if (id == ft)
                {
                    cursor_row = i;
                    break;
                }
            }
        }
        auto item = m_model->item(cursor_row);
        if (item != nullptr)
        {
            auto index = m_model->indexFromItem(item);
            setCurrentIndex(index);
            scrollTo(index);
        }
        else
        {
            scrollToTop();
        }
    }
}

void FractalList::currentChanged(const QModelIndex& current, const QModelIndex&)
{
    int row = current.row();
    auto item = m_model->item(row);
    auto index = m_model->indexFromItem(item);
    scrollTo(index);
    item = m_model->item(row, 1);
    auto ft = static_cast<FRACTAL_TYPE>(item->text().toUInt());
    emit fractal_type(ft);
}
