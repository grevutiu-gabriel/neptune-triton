//
// File:   FractalSettingsWindow.h
// Author: M.R.Eggleston
//
// Created on 03 May 2010, 15:39
//

#include "FractalSettingsWindow.h"

const int FractalSettingsWindow::FRACTAL_PAGE = 0;
const int FractalSettingsWindow::IMAGE_PAGE = 1;
const int FractalSettingsWindow::COLOUR_PAGE = 2;
const int FractalSettingsWindow::COLOUR_MAP_PAGE = 3;

FractalSettingsWindow::FractalSettingsWindow(QWidget* parent)
:
QWidget(parent),
m_fractal_list(new FractalList()),
m_colour_map_list(new ColourMapList()), 
m_tabs(new QTabWidget()),
m_fractal_tab(new FractalTab()),
m_image_tab(new ImageTab()),
m_colouring_tab(new ColouringTab()),
m_colour_tab(new ColourTab()),
m_page(FRACTAL_PAGE )
{
    hide();
    setWindowTitle("Neptune - Settings");
    m_config.SetFractalType(Config::instance()->getFractalType());
    setup_ui_layout();
    connect_signals();
}

void FractalSettingsWindow::setup_ui_layout()
{
    m_fractal_list->setFixedWidth(200);
    m_colour_map_list->setFixedWidth(200);
    m_colour_map_list->setVisible(false);
    auto main_layout = new QHBoxLayout();
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->addWidget(m_fractal_list);
    main_layout->addWidget(m_colour_map_list);
    m_tabs->setFixedWidth(512);
    m_tabs->addTab(m_fractal_tab, "Fractal");
    m_tabs->addTab(m_image_tab, "Image");
    m_tabs->addTab(m_colouring_tab, "Colour");
    m_tabs->addTab(m_colour_tab, "Colour Maps");
    main_layout->addWidget(m_tabs);
    setLayout(main_layout);
    setFixedWidth(712);
    setFixedHeight(580);
}

void FractalSettingsWindow::connect_signals()
{
    connect(m_fractal_list, SIGNAL(fractal_type(FRACTAL_TYPE)),
            this,           SLOT(on_fractal_type(FRACTAL_TYPE)));
    connect(m_tabs,         SIGNAL(currentChanged(int)),
            this,           SLOT(on_tab_changed(int)));
    connect(m_colour_map_list, SIGNAL(currentRowChanged(int)),
            this,           SLOT(on_colour_map_changed(int)));
    connect(m_colouring_tab->innerColouring(), SIGNAL(colouring_changed(const QString&)),
            this,           SLOT(on_colouring_changed(const QString&)));
    connect(m_colouring_tab->outerColouring(), SIGNAL(colouring_changed(const QString&)),
            this,           SLOT(on_colouring_changed(const QString&)));
    connect(m_colouring_tab, SIGNAL(colouring_changed(const QString&)),
            this,           SLOT(on_colouring_changed(const QString&)));
    connect(m_colour_tab,   SIGNAL(new_map()),
            this,           SLOT(on_new_map()));
    connect(m_colour_tab,   SIGNAL(map(const QString&)),
            this,           SLOT(on_map(const QString&)));
}

void FractalSettingsWindow::setFractalType(FRACTAL_TYPE ft)
{
    m_config.SetFractalType(ft);
    m_fractal_list->setCursorToFractalType(ft);
    m_fractal_tab->setFractalType(ft);
    m_image_tab->setFractalType(ft);
    m_colouring_tab->setFractalType(ft);
    if (m_page == COLOUR_PAGE)
    {
        m_colour_map_list->setCursorAtMap(m_colouring_tab->currentColourMap());
    }
}

void FractalSettingsWindow::on_fractal_type(FRACTAL_TYPE ft)
{
    Config::instance()->setFractalType(ft);
    setFractalType(ft);
    emit fractal_type(ft);
}

void FractalSettingsWindow::on_tab_changed(int page)
{
    m_page = page;
    if (page == COLOUR_PAGE)
    {
        QString map_name = m_colouring_tab->currentColourMap();
        m_colour_map_list->show();
        m_fractal_list->hide();
        m_colour_map_list->setCursorAtMap(map_name);
        m_colouring_tab->display();
    }
    else if (page == COLOUR_MAP_PAGE)
    {
        m_colour_map_list->show();
        m_fractal_list->hide();
        m_colour_map_list->setCursorAtMap(m_colour_tab->currentColourMap());
        m_colour_tab->setColourMap(m_colour_tab->currentColourMap());
    }
    else
    {
        m_colour_map_list->hide();
        m_fractal_list->show();
    }
}

void FractalSettingsWindow::on_colour_map_changed(int row)
{
    if (row >= 0)
    {
        if (m_page == COLOUR_PAGE)
        {
            m_colouring_tab->setColourMap(m_colour_map_list->colourMapAtCursor());
            emit colouring_changed();
        }
        else
        {
            m_colour_tab->setColourMap(m_colour_map_list->colourMapAtCursor());
        }
    }
}

void FractalSettingsWindow::on_colouring_changed(const QString& map_name)
{
    m_colour_map_list->setCursorAtMap(map_name);
    m_colouring_tab->display();
    emit colouring_changed();
}

void FractalSettingsWindow::on_selection_changed()
{
    emit colouring_changed();
}

void FractalSettingsWindow::on_new_map()
{
    m_colour_map_list->setCursorAtMap(""); // removes cursor
}

void FractalSettingsWindow::on_map(const QString& map_name)
{
    m_colour_map_list->load();
    m_colour_map_list->setCursorAtMap(map_name);
}

void FractalSettingsWindow::redisplayLocation()
{
    m_image_tab->display();
}

ImageTab* FractalSettingsWindow::image_tab()
{
    return m_image_tab;
}

ImageSize* FractalSettingsWindow::image_size()
{
    return m_image_tab->image_size();
}

FractalTab* FractalSettingsWindow::fractal_tab()
{
    return m_fractal_tab;
}

