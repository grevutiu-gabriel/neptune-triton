//
// File:   FractalList.h
// Author: M.R.Eggleston
//
// Created on 06 July 2010, 16:10
//
// Converted to Qt March 2013 renamed from FractalTree.h

#pragma once

#include <QListView>
#include <QStandardItemModel>

#include "FractalsConfig.h"
#include "TypesAndConstants.h"

class FractalList : public QListView
{
    Q_OBJECT
signals:
    void fractal_type(FRACTAL_TYPE ft);

public:
    FractalList(QWidget* parent = nullptr);
    virtual ~FractalList();

    FRACTAL_TYPE getFractalTypeAtCursor();
    void setCursorToFractalType(FRACTAL_TYPE ft);

private:
    FractalList(const FractalList& orig);
    
    void currentChanged(const QModelIndex& current, const QModelIndex& previous);

    QStandardItemModel* m_model;
    FractalsConfig m_config;
    int m_size;
};

