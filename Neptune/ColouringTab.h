//
// File:   ColouringTab.h
// Author: M.R.Eggleston
//
// Created on 06 October 2010, 11:06
//

#pragma once

#include <QImage>
#include <QWidget>
#include "BailoutColouring.h"
#include "Config.h"
#include "FractalsConfig.h"
#include "OrbitColouring.h"
#include "TypesAndConstants.h"


class ColouringTab : public QWidget
{
    Q_OBJECT
signals:
    void colouring_changed(const QString& map_name);
    void colour_map_changed();
    
private slots:
    void on_colouring_changed(int index);
    void on_fixed_colour_changed(const QColor& colour);
    void on_orbit_colour_changed();
    void on_colour_maps_changed(const QString name);
    void on_inner_type_changed(int index);
    void on_compound_image_changed(int index);

public:
    ColouringTab(QWidget* parent = nullptr);
    virtual ~ColouringTab();

    void setFractalType(FRACTAL_TYPE ft);
    void display();
    void setColourMap(const QString& map_name);
    const QString& currentColourMap();
    const BailoutColouring* innerColouring();
    const BailoutColouring* outerColouring();

private:
    ColouringTab(const ColouringTab& orig);
    
    void setup_ui_layout();
    void connect_signals();

    void setColour(const std::string &value);
    void setColourMap();

    void display_for_bailout_fractals();
    void display_for_orbit_plotting();
    void expose_colour_map(bool inner);
    
    void drawColourMaps();
    void drawColourMap(QImage& image, const QString& map_name, bool inner);
    
    FractalsConfig m_config;
    
    InnerTypeComboBox* m_inner_type_combo;
    CompoundImageComboBox* m_compound_combo;
    QWidget* m_bailout_widget;
    ColouringCombobox* m_colouring_combo;
    BailoutColouring* m_inner_colouring;
    BailoutColouring* m_outer_colouring;
    OrbitColouring* m_orbit_colouring;
    QImage m_map1_image;
    QImage m_map2_image;
    QLabel* m_map1_label;
    QLabel* m_map2_label;

    bool m_ignore_signals;
    QString m_colour_map;
    int m_old_red;
    int m_old_green;
    int m_old_blue;
    boost::shared_ptr<ColourMaps> m_colour_maps;
    bool m_inner;
};

