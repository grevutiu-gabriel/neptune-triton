//
// File:   ColourMapTree.h
// Author: M.R.Eggleston
//
// Created on 06 July 2010, 10:20
//
// Converted to Qt March 2013 renamed from ColourMapTree.cc
//

#pragma once

#include <QListWidget>
#include "ColourMaps.h"

class ColourMapList : public QListWidget
{
    Q_OBJECT
private slots:
    void on_colour_map_changed(const QString name); // to handle signal from colour_maps

public:
    ColourMapList(QWidget* parent = nullptr);
    virtual ~ColourMapList();

    QString& colourMapAtCursor();
    void setCursorAtMap(const QString& map); // defaults to first entry if not found.
    bool hasCursor();
    QString& colourMapAfterCursor();
    void load();
    //
    // Colour maps "Default" & "Default 1" to "Default 4" should not be edited.
    // If the cursor is on the a default map this routine should return true.
    //
    bool isDefault();

private:
    ColourMapList(const ColourMapList& orig);

    QString m_map_name;
    boost::shared_ptr<ColourMaps> m_colour_maps;
};
