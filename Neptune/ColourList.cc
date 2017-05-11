// 
// File:   ColourList.h
// Author: M.R.Eggleston
//
// Created on 15 April 2013, 13:29
//

#include <iostream>
#include <QHBoxLayout>
#include "ColourList.h"

ColourList::ColourList(COLOUR_TYPE colour, QWidget* parent)
:
QWidget(parent),
m_list_type(colour),
m_colour(new SingleColour(m_list_type)),
m_index_spin(new QSpinBox()),
m_add_button(new QPushButton("Add")),
m_set_button(new QPushButton("Set")),
m_delete_button(new QPushButton("Delete")),
m_cancel_button(new QPushButton("Cancel")),
m_swap_above_button(new QPushButton("Above")),
m_swap_below_button(new QPushButton("Below")),
m_display_list(new QTreeWidget())
{
    m_index_spin->setRange(0, 511);
    m_index_spin->setWrapping(true);
    m_display_list->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
    m_display_list->setColumnCount(2);
    QStringList column_headers;
    column_headers.append("Index");
    column_headers.append("Colour");
    m_display_list->setHeaderHidden(true);
    m_display_list->setHeaderLabels(column_headers);
    setup_ui_layout();
    connect_signals();
}

ColourList::~ColourList()
{
}

void ColourList::setup_ui_layout()
{
    auto main_layout = new QHBoxLayout();
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    auto vlayout = new QVBoxLayout();
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->addWidget(m_colour);
    vlayout->addStretch();
    main_layout->addLayout(vlayout);
    vlayout = new QVBoxLayout();
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 0, 0, 0);
    auto label = new QLabel("Index");
    label->setAlignment(Qt::AlignHCenter);
    vlayout->addWidget(label);
    vlayout->addWidget(m_index_spin);
    vlayout->addWidget(m_add_button);
    vlayout->addWidget(m_set_button);
    vlayout->addWidget(m_delete_button);
    vlayout->addWidget(m_cancel_button);
    label = new QLabel("Swap");
    label->setAlignment(Qt::AlignHCenter);
    vlayout->addWidget(label);
    vlayout->addWidget(m_swap_above_button);
    vlayout->addWidget(m_swap_below_button);
    vlayout->addStretch();
    main_layout->addLayout(vlayout);
    main_layout->addWidget(m_display_list);
    main_layout->addStretch();
    setLayout(main_layout);
}

void ColourList::connect_signals()
{
    connect(m_display_list, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
            this,           SLOT(on_cursor_changed(QTreeWidgetItem*, QTreeWidgetItem*)));
    connect(m_index_spin,   SIGNAL(valueChanged(int)),
            this,           SLOT(on_index_spin(int)));
    connect(m_set_button,   SIGNAL(clicked(bool)),
            this,           SLOT(on_set(bool)));
    connect(m_add_button,   SIGNAL(clicked(bool)),
            this,           SLOT(on_add(bool)));
    connect(m_delete_button, SIGNAL(clicked(bool)),
            this,           SLOT(on_delete(bool)));
    connect(m_cancel_button, SIGNAL(clicked(bool)),
            this,           SLOT(on_cancel(bool)));
    connect(m_swap_above_button, SIGNAL(clicked(bool)),
            this,           SLOT(on_above(bool)));
    connect(m_swap_below_button, SIGNAL(clicked(bool)),
            this,           SLOT(on_below(bool)));
}

void ColourList::clear()
{
    m_display_list->clear();
    m_list.clear();
    m_colour->setColour(QColor(0, 0, 0));
    m_index_spin->setValue(0);
    m_display_list->setColumnCount(2);
}

void ColourList::addRow(int index, const QColor& colour)
{
    auto item = new QTreeWidgetItem();
    item->setText(0, QString::number(index));
    item->setText(1, colour.name());
    QImage image = QImage(10, 10, QImage::Format_RGB32);
    image.fill(colour);
    item->setIcon(1, QIcon(QPixmap::fromImage(image)));
    m_display_list->addTopLevelItem(item);
    m_list.push_back(colourEntry(index, colour));
}

void ColourList::setColumns()
{
    m_display_list->resizeColumnToContents(0);
    m_display_list->resizeColumnToContents(1);
    set_buttons(-1);
}

void ColourList::on_cursor_changed(QTreeWidgetItem* item, QTreeWidgetItem*)
{
    // second parameter not used
    if (item != nullptr)
    {
        int index = item->text(0).toInt();
        int row = rowWithIndex(index);
        m_index_spin->setValue(index);
        set_buttons(row);
    }
}

void ColourList::on_index_spin(int)
{
    auto item = m_display_list->currentItem();
    int row = m_display_list->indexOfTopLevelItem(item);
    set_buttons(row);
}

void ColourList::on_set(bool)
{
    int index = m_index_spin->value();
    QColor colour = m_colour->value();
    auto item = m_display_list->currentItem();
    if (item != nullptr)
    {
        m_display_list->setCurrentItem(nullptr); // remove cursor
        int old_index = item->text(0).toInt();
        if (old_index == index)
        {
            item->setText(1, colour.name());
            QImage image = QImage(10, 10, QImage::Format_RGB32);
            image.fill(colour);
            item->setIcon(1, QIcon(QPixmap::fromImage(image)));
            int row = rowWithIndex(index);
            m_list[row].colour = colour;
        }
        else
        {
            deleteFromList(old_index);
            deleteFromDisplayList(old_index);
            addToList(index, colour);
            addToDisplayList(index, colour);
        }
    }
    set_buttons(-1);
    emit changed();
}

void ColourList::on_add(bool)
{
    int index = m_index_spin->value();
    QColor colour = m_colour->value();
    addToList(index, colour);
    addToDisplayList(index, colour);
    m_display_list->resizeColumnToContents(0);
    m_display_list->resizeColumnToContents(1);
    m_display_list->setCurrentItem(nullptr);
    set_buttons(-1);
    emit changed();
}

void ColourList::on_delete(bool)
{
    auto item = m_display_list->currentItem();
    int index = item->text(0).toInt();
    deleteFromList(index);
    int row = m_display_list->indexOfTopLevelItem(item);
    m_display_list->takeTopLevelItem(row);
    int size = m_list.size();
    if (row == size)
    {
        row--;
    }
    item = m_display_list->topLevelItem(row);
    if (item != nullptr)
    {
        m_display_list->setCurrentItem(item);
    }
    set_buttons(item != nullptr ? row : -1);
    emit changed();
}

void ColourList::on_cancel(bool)
{
    m_display_list->setCurrentItem(nullptr); // remove cursor
    set_buttons(-1);
}

void ColourList::on_above(bool)
{
    auto item = m_display_list->currentItem();
    int row = m_display_list->indexOfTopLevelItem(item);
    if (row > 0)
    {
        auto entry = m_list[row];
        m_list[row].colour = m_list[row - 1].colour;
        m_list[row - 1].colour = entry.colour;
        item->setText(1, m_list[row].colour.name());
        QImage image = QImage(10, 10, QImage::Format_RGB32);
        image.fill( m_list[row].colour);
        m_colour->setColour(m_list[row].colour);
        item->setIcon(1, QIcon(QPixmap::fromImage(image)));
        auto above = m_display_list->itemAbove(item);
        if (item != nullptr)
        {
            QImage image = QImage(10, 10, QImage::Format_RGB32);
            image.fill(m_list[row - 1].colour);
            above->setIcon(1, QIcon(QPixmap::fromImage(image)));
            above->setText(1, m_list[row - 1].colour.name());
            m_display_list->setCurrentItem(above);
        }
        emit changed();
    }
}

void ColourList::on_below(bool)
{
    auto item = m_display_list->currentItem();
    int row = m_display_list->indexOfTopLevelItem(item);
    int last_row = m_list.size() - 1;
    if (row < last_row)
    {
        auto entry = m_list[row];
        m_list[row].colour = m_list[row + 1].colour;
        m_list[row + 1].colour = entry.colour;
        item->setText(1, m_list[row].colour.name());
        QImage image = QImage(10, 10, QImage::Format_RGB32);
        image.fill(m_list[row].colour);
        m_colour->setColour(m_list[row].colour);
        item->setIcon(1, QIcon(QPixmap::fromImage(image)));
        auto below = m_display_list->itemBelow(item);
        if (item != nullptr)
        {
            QImage image = QImage(10, 10, QImage::Format_RGB32);
            image.fill( m_list[row + 1].colour);
            below->setIcon(1, QIcon(QPixmap::fromImage(image)));
            below->setText(1, m_list[row + 1].colour.name());
            m_display_list->setCurrentItem(below);
        }
        emit changed();
    }
}

void ColourList::set_buttons(int row)
{
    bool cursor = (row != -1);
    bool index_exists = cursor; // if cursor then definitely in list
    if (!cursor)
    {
        // index is possibly in list
        int index = m_index_spin->value();
        index_exists = (rowWithIndex(index) != -1);
    }
    m_add_button->setEnabled(!index_exists);
    m_set_button->setEnabled(cursor);
    m_delete_button->setEnabled(cursor);
    m_cancel_button->setEnabled(cursor);
    m_swap_above_button->setEnabled((row != 0) && cursor);
    int size = m_list.size();
    m_swap_below_button->setEnabled(((row + 1) != size) && cursor);
}

int ColourList::rowWithIndex(int index)
{
    int row = -1;
    int size = m_list.size();
    for (int i = 0; i < size; i++)
    {
        if (m_list[i].index == index)
        {
            row = i;
            break;
        }
    }
    return row;
}

ColourEntryVector ColourList::getColours()
{
    return m_list;
}

void ColourList::deleteFromList(int index)
{
    for (auto it = m_list.begin(); it != m_list.end(); it++)
    {
        if (it->index == index)
        {
            m_list.erase(it);
            break;
        }
    }
}

void ColourList::addToList(int index, const QColor& colour)
{
    colourEntry entry(index, colour);
    auto it = m_list.begin();
    bool inserted = false;
    while (it != m_list.end())
    {
        if (it->index > index)
        {
            // insert here
            m_list.insert(it, 1, entry);
            inserted = true;
            break;
        }
        it++;
    }
    if (!inserted)
    {
        m_list.push_back(entry);
    }
}

void ColourList::deleteFromDisplayList(int index)
{
    int size = m_display_list->topLevelItemCount();
    for (int i = 0; i < size; i++)
    {
        auto item = m_display_list->topLevelItem(i);
        int item_index = item->text(0).toInt();
        if (item_index == index)
        {
            m_display_list->takeTopLevelItem(index);
            break;
        }
    }
}

void ColourList::addToDisplayList(int index, const QColor& colour)
{
    auto new_item = new QTreeWidgetItem();
    new_item->setText(0, QString::number(index));
    new_item->setText(1, colour.name());
    QImage image = QImage(10, 10, QImage::Format_RGB32);
    image.fill(colour);
    new_item->setIcon(1, QIcon(QPixmap::fromImage(image)));
    int size = m_display_list->topLevelItemCount();
    bool inserted = false;
    for (int i = 0; i < size; i++)
    {
        auto item = m_display_list->topLevelItem(i);
        int item_index = item->text(0).toInt();
        if (item_index > index)
        {
            m_display_list->insertTopLevelItem(i, new_item);
            inserted = true;
            break;
        }
    }
    if (!inserted)
    {
        m_display_list->addTopLevelItem(new_item);
    }
    m_display_list->scrollToItem(new_item);
}
