//
// File:   FractalDrawing.h
// Author: M.R.Eggleston
//
// Created on 07 July 2010, 11:00
//

// March 2013 converted to Qt

#pragma once

#include <QLabel>
#include <QMouseEvent>
#include "ColourMaps.h"
#include "OptionComboboxes.h"

class FractalDrawing : public QLabel
{
    Q_OBJECT
public:
    enum EVENT
    {
        ZOOM_IN,
        ZOOM_OUT,
        CENTRE,
    };
    
signals:
    void signal_drawing_changed(FractalDrawing::EVENT event);

public:
    FractalDrawing(QWidget* parent = nullptr);
    virtual ~FractalDrawing();
    
    void setNavigation(NAVIGATION);
    QPoint currentSelectionCentre();
    int currentSelectionWidth();
    bool isSelection();
    void selectionOff();

    void draw();

    QPoint getPressButton();
    QPoint getReleaseButton();

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);

private:
    FractalDrawing(const FractalDrawing& orig);
    
    void QuickRectangle(QPainter& painter);
    void ControlledRectangle(QPainter& painter);
    void Cross(QPainter& painter);
    void resizeZoomRectangle(int x, int y);
    void moveRectangleOrCross();
    void checkPosition(const QPoint& point);
    void resizeZoomRectangle();
    
    bool m_zoom_selection;

    int m_press_x;
    int m_press_y;
    int m_release_x;
    int m_release_y;
    int m_motion_x;
    int m_motion_y;
    bool m_left_button;
    int m_width;
    int m_height;
    NAVIGATION m_navigation;
    int m_zoom_rectangle_h;
    int m_zoom_rectangle_w;
    int m_new_rectangle_h;
    int m_new_rectangle_w;
    int m_controlled_zoom;
    int m_pos_x; // Initial position of centre of rectangle/cross
    int m_pos_y;
    int m_new_pos_x; // Moved position of centre of rectangle/cross
    int m_new_pos_y;
    bool m_cross_on;
    bool m_can_grab;
    bool m_resize_top;
    bool m_resize_bottom;
    bool m_resize_left;
    bool m_resize_right;
    bool m_grabbed;
};
