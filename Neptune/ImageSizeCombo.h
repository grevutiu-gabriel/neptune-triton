// 
// File:   ImageSizeCombo.h
// Author: M.R.Eggleston
//
// Created on 20 February 2012, 11:29
//

#pragma once

#include <vector>
#include <QComboBox>

class ImageSizeCombo : public QComboBox
{
    Q_OBJECT
signals:
    void changed(int w, int h);

private slots:
    void on_activated(int index);

public:
    ImageSizeCombo(QWidget* parent = nullptr);
    virtual ~ImageSizeCombo();
    
    void Set(int w, int h);
    void Get(int &w, int &h);
    bool isCustom();
    
private:
    ImageSizeCombo(const ImageSizeCombo& orig);
    
    std::vector<int> m_width;
    std::vector<int> m_height;
};


