//
// File:   FractalDrawing.cc
// Author: M.R.Eggleston
// Created on 07 July 2010, 11:00
//

#include <iostream>
#include <vector>
#include <QPainter>
#include "FractalDrawing.h"
#include "StaticCalculatingUnit.h"
#include "Utilities.h"

FractalDrawing::FractalDrawing(QWidget* parent)
:
QLabel(parent),
m_zoom_selection(false),
m_press_x(0),
m_press_y(0),
m_release_x(0),
m_release_y(0),
m_motion_x(0),
m_motion_y(0),
m_left_button(false),
m_width(0),
m_height(0),
m_navigation(N_QUICK),
m_zoom_rectangle_h(0),
m_zoom_rectangle_w(0),
m_new_rectangle_h(0),
m_new_rectangle_w(0),
m_controlled_zoom(false),
m_cross_on(false),
m_can_grab(false),
m_resize_top(false),
m_resize_bottom(false),
m_resize_left(false),
m_resize_right(false),
m_grabbed(false)
{
    setMouseTracking(true);
}

FractalDrawing::~FractalDrawing()
{
}

void FractalDrawing::draw()
{
    update(0, 0, width(), height());
}

QPoint FractalDrawing::getPressButton()
{
    return QPoint(m_press_x, m_press_y);
}

QPoint FractalDrawing::getReleaseButton()
{
    return QPoint(m_release_x, m_release_y);
}

void FractalDrawing::mousePressEvent(QMouseEvent* event)
{
    checkPosition(event->pos());
    if (m_navigation != N_OFF)
    {
        m_left_button = false;
        if ((event->button() == Qt::LeftButton) || (event->button() == Qt::RightButton))
        {
            m_left_button = (event->button() == Qt::LeftButton);
            m_press_x = event->x();
            m_press_y = event->y();
            m_motion_x = m_press_x;
            m_motion_y = m_press_y;
        }
        if ((m_navigation == N_QUICK) && m_left_button)
        {
            m_grabbed = true;
        }
        else if ((m_navigation == N_ZOOM_IN) && m_left_button)
        {
            if (!m_controlled_zoom)
            {
                m_controlled_zoom = true;
                m_zoom_rectangle_h = static_cast<int>(static_cast<double>(m_height)/2.0 + 0.5);
                m_zoom_rectangle_w = static_cast<int>(static_cast<double>(m_width)/2.0 + 0.5);
                m_new_rectangle_h = m_zoom_rectangle_h;
                m_new_rectangle_w = m_zoom_rectangle_w;
                m_pos_x = m_press_x;
                m_pos_y = m_press_y;
                m_new_pos_x = m_pos_x;
                m_new_pos_y = m_pos_y;
            }
            m_grabbed = m_can_grab;
            if (m_resize_top || m_resize_bottom)
            {
                setCursor(QCursor(Qt::SizeVerCursor));
                m_grabbed = true;
            }
            else if (m_resize_left || m_resize_right)
            {
                setCursor(QCursor(Qt::SizeHorCursor));
                m_grabbed = true;
            }
            else if (m_can_grab)
            {
                setCursor(QCursor(Qt::ClosedHandCursor));
            }
            else
            {
                setCursor(QCursor(Qt::ArrowCursor));
                m_grabbed = false;
            }
            draw();
        }
        else if ((m_navigation != N_QUICK) && m_left_button)
        {
            if (!m_cross_on)
            {
                m_cross_on = true;
                m_pos_x = m_press_x;
                m_pos_y = m_press_y;
                m_new_pos_x = m_pos_x;
                m_new_pos_y = m_pos_y;
            }
            setCursor(QCursor(Qt::ClosedHandCursor));
            m_grabbed = true;
            draw();
        }
    }
}

void FractalDrawing::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_grabbed)
    {
        checkPosition(event->pos());
    }
    if (m_navigation != N_OFF)
    {
        if ((event->x() >= 0) && (event->x() < m_width) &&
            (event->y() >=0) && (event->y() < m_height))
        {
            // Check whether the bounds of the selection box go out of range
            if ((m_navigation == N_QUICK) && m_grabbed)
            {
                resizeZoomRectangle(event->x(), event->y());
            }
            else
            {
                m_motion_x = event->x();
                m_motion_y = event->y();
                if (m_resize_left || m_resize_right)
                {
                    setCursor(QCursor(Qt::SizeHorCursor));
                    if (m_grabbed)
                    {
                        resizeZoomRectangle();
                    }
                }
                else if (m_resize_top || m_resize_bottom)
                {
                    setCursor(QCursor(Qt::SizeVerCursor));
                    if (m_grabbed)
                    {
                        resizeZoomRectangle();
                    }
                }
                else if (m_grabbed)
                {
                    setCursor(QCursor(Qt::ClosedHandCursor));
                    moveRectangleOrCross();
                }
                else
                {
                    setCursor(m_can_grab ? QCursor(Qt::OpenHandCursor) : QCursor(Qt::ArrowCursor));
                }
            }
        }
    }
}

void FractalDrawing::resizeZoomRectangle(int x, int y)
{
    bool valid = true;
    int height = abs(m_press_y - y);
    if ((x - m_press_x) <= 0)
    {
        valid  = ((m_press_x - (height*m_width)/m_height) >= 0);
    }
    else
    {
        valid  = ((m_press_x + (height*m_width)/m_height) < m_width);
    }
    if (valid)
    {
        m_motion_x = x;
        m_motion_y = y;
        draw();
    }
}

void FractalDrawing::resizeZoomRectangle()
{
    double ratio = static_cast<double>(m_width)/static_cast<double>(m_height);
    int change = 0;
    if (m_resize_top)
    {
        change = m_new_pos_y - m_zoom_rectangle_h/2 - m_motion_y;
        m_new_rectangle_h = m_zoom_rectangle_h + 2*change;
        m_new_rectangle_w = static_cast<int>(static_cast<double>(m_new_rectangle_h)*ratio + 0.5);
    }
    else if (m_resize_bottom)
    {
        change = m_new_pos_y + m_zoom_rectangle_h/2 - m_motion_y;
        m_new_rectangle_h = m_zoom_rectangle_h - 2*change;
        m_new_rectangle_w = static_cast<int>(static_cast<double>(m_new_rectangle_h)*ratio + 0.5);
    }
    else if (m_resize_left)
    {
        change = m_new_pos_x - m_zoom_rectangle_w/2 - m_motion_x;
        m_new_rectangle_w = m_zoom_rectangle_w + 2*change;
        m_new_rectangle_h = static_cast<int>(static_cast<double>(m_new_rectangle_w)/ratio + 0.5);
    }
    else if (m_resize_right)
    {
        change = m_new_pos_x + m_zoom_rectangle_w/2 - m_motion_x;
        m_new_rectangle_w = m_zoom_rectangle_w - 2*change;
        m_new_rectangle_h = static_cast<int>(static_cast<double>(m_new_rectangle_w)/ratio + 0.5);
    }
    // The rectangle must not have sides smaller than 15 pixels
    if ((m_new_rectangle_h < 15) || (m_new_rectangle_w < 15))
    {
        if (ratio < 1.0)
        {
            m_new_rectangle_w = 15;
            m_new_rectangle_h = static_cast<int>(static_cast<double>(m_new_rectangle_w)/ratio + 0.5);
        }
        else
        {
            m_new_rectangle_h = 15;
            m_new_rectangle_w = static_cast<int>(static_cast<double>(m_new_rectangle_h)*ratio + 0.5);
        }
    }
    draw();
}


void FractalDrawing::moveRectangleOrCross()
{
    m_new_pos_x = m_pos_x + (m_motion_x - m_press_x);
    m_new_pos_y = m_pos_y + (m_motion_y - m_press_y);
    draw();
}

void FractalDrawing::mouseReleaseEvent(QMouseEvent* event)
{
    m_grabbed = false;
    if (m_navigation == N_QUICK)
    {
        if (event->button() == Qt::RightButton)
        {
            m_release_x = event->x();
            m_release_y = event->y();
            if ((m_press_x == m_release_x) || (m_press_y == m_release_y))
            {
                 emit signal_drawing_changed(ZOOM_OUT);
            }
        }
        else if (event->button() == Qt::LeftButton)
        {
            m_release_x = event->x();
            m_release_y = event->y();
            // Only zoom in if the 'box' has height and width of at least
            // 10 pixels
            // 
            int w = abs(m_press_x - m_release_x);
            int h = abs(m_press_y - m_release_y);
            if ((w >= 10) || (h >= 10))
            {
                 emit signal_drawing_changed(ZOOM_IN);
            }
            else
            {
                // Only centre the image if the button press and release
                // are at the same location and is on the image.
                int x = m_release_x;
                int y = m_release_y;
                bool in_view = (x >= 0) && ((x < m_width) && (y >= 0) && (y < m_height));
                if (in_view && (w == 0) && (h == 0))
                {
                    emit signal_drawing_changed(CENTRE);
                }
            }
            m_left_button = false;
        }
    }
    else if (event->button() == Qt::LeftButton)
    {
        if (m_resize_top || m_resize_bottom || m_resize_left || m_resize_right)
        {
            m_zoom_rectangle_w = m_new_rectangle_w;
            m_zoom_rectangle_h = m_new_rectangle_h;
        }
        m_pos_x = m_new_pos_x;
        m_pos_y = m_new_pos_y;
        m_left_button = false;
    }
    else if (event->button() == Qt::RightButton)
    {
        m_cross_on = false;
        m_controlled_zoom = false;
        setCursor(QCursor(Qt::ArrowCursor));
        draw();
    }
}

void FractalDrawing::paintEvent(QPaintEvent*)
{
    if (!StaticCalculatingUnit::s_image.isNull())
    {
        QPainter painter(this);
        StaticCalculatingUnit::paintImage(painter);
        m_width = StaticCalculatingUnit::s_width;
        m_height = StaticCalculatingUnit::s_height;
        if ((m_navigation == N_QUICK) && (m_left_button))
        {
            QuickRectangle(painter);
        }
        else if ((m_navigation == N_ZOOM_IN) && m_controlled_zoom)
        {
            ControlledRectangle(painter);
        }
        else if ((m_navigation == N_CENTRE_IN) || (m_navigation == N_CENTRE_OUT))
        {
            if (m_cross_on)
            {
                Cross(painter);
            }
        }
    }
}

void FractalDrawing::setNavigation(NAVIGATION value)
{
    m_navigation = value;
    selectionOff();
}

QPoint FractalDrawing::currentSelectionCentre()
{
    return QPoint(m_pos_x, m_pos_y);
}

int FractalDrawing::currentSelectionWidth()
{
    return m_zoom_rectangle_w;
}

bool FractalDrawing::isSelection()
{
    return (m_controlled_zoom || m_cross_on);
}
void FractalDrawing::selectionOff()
{
    m_controlled_zoom = false;
    m_cross_on = false;
    draw();
}

void FractalDrawing::QuickRectangle(QPainter& painter)
{
    int x = m_press_x;
    int y = m_press_y;
    int height = m_motion_y - m_press_y;
    if (height < 0)
    {
        y = m_motion_y;
        height = -height;
    }
    int width = (height*m_width)/m_height;
    if (m_motion_x < m_press_x)
    {
        x -= width;
    }
    if ((width >= 10) && (height >= 10))
    {
        painter.setPen(Qt::white);
        painter.drawRect(x, y, width, height);
        painter.setPen(Qt::black);
        painter.drawRect(x+1, y+1, width-2, height-2);
    }
}

void FractalDrawing::ControlledRectangle(QPainter& painter)
{
    int x;
    int y;
    int h;
    int w;
    if (m_resize_top || m_resize_bottom || m_resize_left || m_resize_right)
    {
        x = m_new_pos_x - m_new_rectangle_w/2;
        y = m_new_pos_y - m_new_rectangle_h/2;
        h = m_new_rectangle_h;
        w = m_new_rectangle_w;
    }
    else
    {
        x = m_new_pos_x - m_zoom_rectangle_w/2;
        y = m_new_pos_y - m_zoom_rectangle_h/2;
        h = m_zoom_rectangle_h;
        w = m_zoom_rectangle_w;
    }
    painter.setPen(Qt::white);
    painter.drawRect(x, y, w, h);
    painter.setPen(Qt::black);
    painter.drawRect(x+1, y+1, w-2, h-2);
}

void FractalDrawing::Cross(QPainter& painter)
{
    painter.setPen(Qt::black);
    painter.drawLine(m_new_pos_x, m_new_pos_y - 9, m_new_pos_x, m_new_pos_y + 10);
    painter.drawLine(m_new_pos_x - 9, m_new_pos_y, m_new_pos_x + 10, m_new_pos_y);
    painter.setPen(Qt::white);
    painter.drawLine(m_new_pos_x - 1, m_new_pos_y - 10, m_new_pos_x + 1, m_new_pos_y - 10);
    painter.drawLine(m_new_pos_x - 1, m_new_pos_y - 10, m_new_pos_x - 1, m_new_pos_y - 1);
    painter.drawLine(m_new_pos_x + 1, m_new_pos_y - 10, m_new_pos_x + 1, m_new_pos_y - 1);
    painter.drawLine(m_new_pos_x - 10, m_new_pos_y - 1, m_new_pos_x - 1, m_new_pos_y - 1);
    painter.drawLine(m_new_pos_x - 10, m_new_pos_y - 1, m_new_pos_x - 10, m_new_pos_y + 1);
    painter.drawLine(m_new_pos_x - 10, m_new_pos_y + 1, m_new_pos_x - 1, m_new_pos_y + 1);
    painter.drawLine(m_new_pos_x - 1, m_new_pos_y + 1, m_new_pos_x - 1, m_new_pos_y + 11);
    painter.drawLine(m_new_pos_x - 1, m_new_pos_y + 11, m_new_pos_x + 1, m_new_pos_y + 11);
    painter.drawLine(m_new_pos_x + 1, m_new_pos_y + 1, m_new_pos_x + 1, m_new_pos_y + 11);
    painter.drawLine(m_new_pos_x + 1, m_new_pos_y + 1, m_new_pos_x + 11, m_new_pos_y + 1);
    painter.drawLine(m_new_pos_x + 11, m_new_pos_y - 1, m_new_pos_x + 11, m_new_pos_y + 1);
    painter.drawLine(m_new_pos_x + 1, m_new_pos_y - 1, m_new_pos_x + 11, m_new_pos_y - 1);
}

void FractalDrawing::checkPosition(const QPoint& point)
{
    m_can_grab = false;
    m_resize_top = false;
    m_resize_bottom = false;
    m_resize_left = false;
    m_resize_right = false;
    if (m_cross_on || m_controlled_zoom)
    {
        int x = point.x();
        int y = point.y();

        if (m_navigation == N_ZOOM_IN)
        {
            int half_width = m_zoom_rectangle_w/2;
            int half_height = m_zoom_rectangle_h/2;
            int top_side = m_new_pos_y - half_height;
            int bottom_side = m_new_pos_y + half_height;
            int left_side = m_new_pos_x - half_width;
            int right_side = m_new_pos_x + half_width;
           
            if ((x > (left_side + 5)) && (x <= (right_side - 5)) &&
                (y > (top_side + 5)) && (y <= (bottom_side - 5)))
            {
                m_can_grab = true;
            }
            else if ((x > left_side) && (x <= right_side))
            {
                m_resize_top = false;
                m_resize_bottom = false;
                if ((y > (top_side - 5)) && (y <= (top_side + 5)))
                {
                    m_resize_top = ((m_new_rectangle_w > 15) && (m_new_rectangle_h > 15));
                }
                else if ((y > (bottom_side - 5)) && (y <= (bottom_side + 5)))
                {
                    m_resize_bottom = ((m_new_rectangle_w > 15) && (m_new_rectangle_h > 15));
                }
            }
            else if ((y > top_side) && (y <= bottom_side))
            {
                m_resize_left = false;
                m_resize_right = false;
                if ((x > (left_side - 5)) && (x <= (left_side + 5)))
                {
                    m_resize_left = ((m_new_rectangle_w > 15) && (m_new_rectangle_h > 15));
                }
                else if ((x > (right_side - 5)) && (x <= (right_side + 5)))
                {
                    m_resize_right = ((m_new_rectangle_w > 15) && (m_new_rectangle_h > 15));
                }
            }
        }
        else
        {
            if ((x > (m_new_pos_x - 9)) && (x < (m_new_pos_x + 10)) &&
                (y > (m_new_pos_y - 9)) && (y < (m_new_pos_y + 10)))
            {
                m_can_grab = true;
            }
        }
    }
}
