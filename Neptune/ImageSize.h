// 
// File:   ImageSize.h
// Author: M.R.Eggleston
//
// Created on 02 May 2013, 10:34
//

#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>

class ImageSize : public QWidget
{
    Q_OBJECT
signals:
    void new_size(unsigned int w, unsigned int h);

private slots:
    void on_height_changed(int i);
    void on_width_changed(int i);
    void on_custom_changed(int state);
    void on_linked_changed(int state);
    void on_set(bool clicked);

public:
    ImageSize(QWidget* parent = nullptr);
    ~ImageSize();

    void setSize(unsigned int w, unsigned int h);
    unsigned int height();
    unsigned int width();

private:
    ImageSize(const ImageSize& other);
    
    void set_ui_layout();
    void connect_signals();
    
    QComboBox* m_height_combo;
    QSpinBox* m_height_spin;
    QComboBox* m_width_combo;
    QSpinBox* m_width_spin;
    QCheckBox* m_custom_check;
    QCheckBox* m_linked_check;
    QPushButton* m_set_button;
    
    unsigned int m_height;
    unsigned int m_width;
    unsigned int m_old_height;
    unsigned int m_old_width;
    double m_aspect_ratio;
    bool m_linked;
    bool m_ignore_signal;
};