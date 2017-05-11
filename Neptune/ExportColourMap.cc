// 
// File:   ExportColourMap.cc
// Author: M.R.Eggleston
// 
// Created on 21 February 2012, 11:03
//
// Converted to Qt April 2013
//

#include <iostream>
#include <QDir>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QVBoxLayout>
#include "Config.h"
#include "Utilities.h"
#include "ExportColourMap.h"

ExportColourMap::ExportColourMap(QWidget* parent)
:
QWidget(parent),
m_colour_map_drawing(new QLabel()),
m_exporting_list(new QListWidget()),
m_export_as_button(new QPushButton("Export as")),
m_move_button(new QToolButton()),
m_export_combo(new ExportCombobox()),
m_colour_map_list(new ColourMapList()),
m_context_menu(new QMenu()),
m_colour_maps(ColourMaps::instance())
{
    setWindowTitle("Export Colour Maps");
    hide();
    m_move_button->setArrowType(Qt::RightArrow);
    setup_ui_layout();
    connect_signals();
    
    m_colour_map_list->setCursorAtMap("Default");
    m_exporting_list->setSortingEnabled(true);
    
    m_export_as_button->setEnabled(false);
    m_colour_map_image = QImage(512, 35, QImage::Format_RGB32);
    auto map = m_colour_maps->getColourMap("Default");
    map.draw(m_colour_map_image);
    m_colour_map_drawing->setPixmap(QPixmap::fromImage(m_colour_map_image));
    setFixedWidth(512);
    setFixedHeight(480);
}

ExportColourMap::~ExportColourMap()
{
}

void ExportColourMap::setup_ui_layout()
{
    auto main_layout = new QVBoxLayout();
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    auto hlayout = new QHBoxLayout();
    hlayout->setSpacing(0);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->addWidget(m_colour_map_list);
    auto hlayout2 = new QHBoxLayout();
    hlayout2->setSpacing(0);
    hlayout2->setContentsMargins(10, 0, 10, 0);
    hlayout2->addWidget(m_move_button);
    hlayout->addLayout(hlayout2);
    hlayout->addWidget(m_exporting_list);
    main_layout->addLayout(hlayout);
    main_layout->addWidget(m_colour_map_drawing);
    hlayout = new QHBoxLayout();
    hlayout->setSpacing(0);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->addWidget(m_export_combo);
    hlayout->addStretch();
    hlayout->addWidget(m_export_as_button);
    main_layout->addLayout(hlayout);
    setLayout(main_layout);
}

void ExportColourMap::connect_signals()
{
    connect(m_export_as_button, SIGNAL(clicked(bool)),
            this,               SLOT(on_export_as_button(bool)));
    connect(m_move_button,      SIGNAL(clicked(bool)),
            this,               SLOT(on_move_button(bool)));
    connect(m_colour_map_list,  SIGNAL(currentRowChanged(int)),
            this,               SLOT(on_colour_map_cursor_changed(int)));
    connect(m_exporting_list,   SIGNAL(currentRowChanged(int)),
            this,               SLOT(on_exporting_cursor_changed(int)));
    
    auto action = m_context_menu->addAction("Remove");
    connect(action, SIGNAL(triggered()), this, SLOT(remove()));
    action = m_context_menu->addAction("Remove all");
    connect(action, SIGNAL(triggered()), this, SLOT(remove_all()));
    
    m_exporting_list->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_exporting_list,   SIGNAL(customContextMenuRequested(const QPoint&)),
            this,               SLOT(on_context_menu(const QPoint&)));
}


void ExportColourMap::on_export_as_button(bool)
{
    QString file_name = QFileDialog::getSaveFileName(this,
                                                     "Export Colour Maps ...",
                                                     QDir::homePath(),
                                                     "scm files (*.scm);;ugr files (*.ugr)");

    auto type = m_export_combo->getValue();
    QString extension = (type == EF_SCM ? "scm" : "ugr");
    file_name = addExtension(file_name, extension);
    QDir directory(QDir::homePath());
    if (directory.exists(file_name))
    {
        //file_exists(file_name); -> MainWindow has this move to utilities?
        auto file = name_only(file_name);
        QString msg("File " + file + " already exists");
        QMessageBox::information(this, "Neptune", msg); // discard return value
    }
    else if (type == EF_SCM)
    {
        saveScm(file_name);
    }
    else
    {
        saveUgr(file_name);
    }
}

void ExportColourMap::on_move_button(bool)
{
    QString name = m_colour_map_list->colourMapAtCursor();
    QString next_name = m_colour_map_list->colourMapAfterCursor();
    m_exporting_list->addItem(name);
    m_colour_map_list->setCursorAtMap(next_name.isEmpty() ? name : next_name);
    QString search_name = (next_name.isEmpty() ? name : next_name);
    bool exists = !m_exporting_list->findItems(search_name, Qt::MatchExactly).isEmpty();
    m_move_button->setEnabled(!exists);
    m_export_as_button->setEnabled(true);
}

void ExportColourMap::on_colour_map_cursor_changed(int)
{
    QString other_name;
    QString name = m_colour_map_list->colourMapAtCursor();
    QList<QListWidgetItem*> list =  m_exporting_list->findItems(name, Qt::MatchExactly);
    bool exists = !list.isEmpty();
    bool exporting_has_cursor = (m_exporting_list->currentRow() != -1);
    if (exists && exporting_has_cursor)
    {
        int row = m_exporting_list->currentRow();
        other_name = m_exporting_list->item(row)->text();
        if (name != other_name)
        {
            m_exporting_list->setCurrentItem(list.first());
        }
    }
    else
    {
        m_exporting_list->setCurrentRow(-1);
    }
    m_move_button->setEnabled(!exists);
    auto map = m_colour_maps->getColourMap(name);
    map.draw(m_colour_map_image);
    m_colour_map_drawing->setPixmap(QPixmap::fromImage(m_colour_map_image));
}

void ExportColourMap::on_exporting_cursor_changed(int row)
{
    if (row != -1)
    {
        QString other_name = m_colour_map_list->colourMapAtCursor();
        QString name = m_exporting_list->item(row)->text();
        if (other_name != name)
        {
            m_colour_map_list->setCursorAtMap(name);
        }
    }
}

void ExportColourMap::saveScm(const QString& name)
{
    std::vector<QString> maps;
    int size = m_exporting_list->count();
    for (int i = 0; i < size; i++)
    {
        maps.push_back(m_exporting_list->item(i)->text());
    }
    Config::instance()->save_scm_file(name, maps);
}

void ExportColourMap::saveUgr(const QString& path)
{
    QFile file(path);
    if (file.open(QIODevice::WriteOnly))
    {
        int size = m_exporting_list->count();
        for (int row = 0; row < size; row++)
        {
            auto name = m_exporting_list->item(row)->text();
            ColourMap map = m_colour_maps->getColourMap(name);
            QString str = name + " {\ngradient:\n";
            str += " title=\"" + name + "\" smooth=no\n";
            if (map.getType() != ColourMap::CM_MANUAL)
            {
                VectorColour colours = map.getColours();
                int size = colours.size();
                for (int i = 0; i < size; i++)
                {
                    str += " index=" + QString::number(i);
                    QRgb colour = colours[i];
                    unsigned int uintcolour = static_cast<unsigned int>(qBlue(colour))*256*256 +
                                              static_cast<unsigned int>(qGreen(colour))*256 +
                                              static_cast<unsigned int>(qRed(colour));
                    str += " color=" + QString::number(uintcolour) + "\n";
                }
            }
            else
            {
                ColoursMap colours = map.getColoursMap();
                for (auto it = colours.begin(); it != colours.end(); it++)
                {
                    str += " index=" + QString::number((*it).first);
                    QColor colour = it->second;
                    unsigned int uintcolour = static_cast<unsigned int>(colour.blue())*256*256 +
                                              static_cast<unsigned int>(colour.green())*256 +
                                              static_cast<unsigned int>(colour.red());
                    str += " color=" + QString::number(uintcolour) + "\n";
                }
            }
            str += "}\n\n";
            auto buffer = str.toUtf8();
            qint64 status = file.write(buffer.data(), buffer.length());
            if (status < static_cast<qint64>(0))
            {
                break;
            }
        }
    }
        file.close();
}

void ExportColourMap::on_context_menu(const QPoint& pos)
{
    if (m_exporting_list->count() > 0)
    {
        m_context_menu->popup(m_exporting_list->mapToGlobal(pos));
    }
}

void ExportColourMap::remove()
{
    int row = m_exporting_list->currentRow();
    if (row != -1)
    {
        auto item = m_exporting_list->takeItem(row);
        if (item != nullptr)
        {
            delete item;
        }
        int size = m_exporting_list->count();
        if (row >= size)
        {
            row--;
        }
        if (size != 0)
        {
            m_exporting_list->setCurrentRow(row);
        }
    }
    m_move_button->setEnabled(true);
    m_export_as_button->setEnabled(m_exporting_list->count() != 0);
}

void ExportColourMap::remove_all()
{
    m_exporting_list->clear();
    m_move_button->setEnabled(true);
    m_export_as_button->setEnabled(false);
}