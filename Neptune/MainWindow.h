// 
// File:   MainWindow.h
// Author: M.R.Eggleston
//
// Created on 19 March 2013, 10:42
//

#pragma once

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenuBar>
#include <QPixmap>
#include <QScrollArea>
#include "ExportColourMap.h"
#include "ImportColourMap.h"
#include "Fractal.h"
#include "FractalSettingsWindow.h"
#include "NumberEntry.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow();
    
    void start();
   
private slots:
    void on_row_by_row_progress();
    void on_iterations_changed();
    void on_drawing_changed(FractalDrawing::EVENT event);
    void on_config_changed();
    void on_new_size(unsigned int w, unsigned int h);
    void on_fractal_type(FRACTAL_TYPE ft);
    void on_colouring_changed();
    void on_navigation(int value);
    void on_zoom(bool clicked);
    void on_centre(bool clicked);
    void on_transforms_changed();
    //Menu
    void open();
    void save_image();
    void save_parameters();
    void import_maps();
    void export_maps();
    void quit();
    void settings();
    void revert();
    void aboutQt();
    void about();

private:
    MainWindow(const MainWindow& orig);
    
    void closeEvent(QCloseEvent* event);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dropEvent(QDropEvent* event);

    void setup_menu();
    void setup_ui_layout();
    void connect_signals();
    
    void display_iterations();
    bool loadParameters(const QString &filename);
    void process_image(FractalConfig& seed);
    void file_not_valid(const QString& file_name);
    void file_exists(const QString& file_name);
    void save_failed(const QString& file_name);
    void black_image(int w, int h);
    void black_image();

    FractalsConfig m_config;
    
    QLabel* m_iterations_label; // And orbit length or calculating cycles
    NumberEntry* m_iterations_line;
    QScrollArea* m_scroll;
    FractalDrawing* m_fractal_image;
    NavigationCombobox* m_navigation_combo;
    QPushButton* m_zoom_button;
    QPushButton* m_centre_button;
    QLabel* m_status_label;
    FractalSettingsWindow* m_fractal_settings;
    ExportColourMap* m_export_colour;
    ImportColourMap* m_import_colour;
    
    boost::scoped_ptr<Fractal> m_fractal;
    bool m_opening_image;
    bool m_drop_ok;
};


