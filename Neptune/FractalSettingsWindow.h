//
// File:   FractalSettingsWindow.h
// Author: M.R.Eggleston
//
// Created on 03 May 2010, 15:39
//
// Converted to Qt March 2013

#pragma once

#ifdef NOT_MOC
#include <boost/shared_ptr.hpp>
#endif
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QTabWidget>
#include "ColourMapList.h"
#include "ColourTab.h"
#include "ColouringTab.h"
#include "FractalList.h"
#include "FractalTab.h"
#include "ImageTab.h"

class FractalSettingsWindow : public QWidget
{
    Q_OBJECT
signals:
    void fractal_type(FRACTAL_TYPE ft);
    void colouring_changed();
    
private slots:
    void on_fractal_type(FRACTAL_TYPE ft);
    void on_tab_changed(int page);
    void on_colour_map_changed(int row);
    void on_colouring_changed(const QString&map_name);
    void on_selection_changed();
    void on_new_map();
    void on_map(const QString& map_name);

public:
    FractalSettingsWindow(QWidget *parent = nullptr);

    void setFractalType(FRACTAL_TYPE ft);
    
    void redisplayLocation();
    ImageTab* image_tab();
    ImageSize* image_size();
    FractalTab* fractal_tab();

    static const int FRACTAL_PAGE;
    static const int IMAGE_PAGE;
    static const int COLOUR_PAGE;
    static const int COLOUR_MAP_PAGE; 

private:
    FractalSettingsWindow(const FractalSettingsWindow& orig);

    void setup_ui_layout();
    void connect_signals();

    FractalList* m_fractal_list;
    ColourMapList* m_colour_map_list;
    QTabWidget* m_tabs;
    FractalTab* m_fractal_tab;
    ImageTab* m_image_tab;
    ColouringTab* m_colouring_tab;
    ColourTab* m_colour_tab;

    FractalsConfig m_config;
    int m_page;
};
