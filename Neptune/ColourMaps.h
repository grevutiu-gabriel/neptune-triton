//
// File:   ColourMaps.h
// Author: M.R.Eggleston
//
// Created on 08 June 2010, 12:46
//

#pragma once

#ifdef NOT_MOC
#include <boost/shared_ptr.hpp>
#endif
#include <map>
#include <vector>
#include <QObject>
#include <QString>
#include "ColourMap.h"

//
// The available colour maps must always contain:
//
//      "Default",
//      "Default 1",
//      "Default 2",
//      "Default 3" and
//      "Default 4"

typedef std::map<QString, ColourMap> Map;
typedef std::map<QString, unsigned int> OffsetMap;
typedef std::vector<QString> QStrings;

class ColourMaps : public QObject
{
    Q_OBJECT
signals:
    void changed(const QString map_name);

public:
    static const unsigned int MAX_COLOURS;
    static const QString TEMP_MAP;

    static boost::shared_ptr<ColourMaps> create();
    static boost::shared_ptr<ColourMaps> instance();

    // Add or replace a map.
    QString setColours(const QString& name, ColourMap& colours, bool emit = true);
    void rename(const QString& old_name, const QString& new_name);
    void updateMaps(const QString& old_name, const QString& new_name, ColourMap& colours, bool emit);
    void RemoveMap(const QString& map_name);
    unsigned int size();
    QStrings& getNames();
    //
    // Colours can be obtained from the map in bulk or individually, if
    // the individual colour is required without the offset applied then get
    // the colour vector and index that.
    //
    ColourMap &getColourMap(const QString& map_name);
    //
    bool nameExists(const QString& name);
    bool mapExists(ColourMap& map, QString& found_name);
    bool isDefault(const QString& name);
    QString assignTemporaryName(const QString& base);

     // the string is the current map
//    sigc::signal<void, QString> signal_maps_changed();

private:
    ColourMaps();
    ColourMaps(const ColourMaps &other);

    void AddDefaultMap(const QString& name);
    void LoadDefaults();

//    sigc::signal<void, Glib::ustring> m_changed;

    Map m_map;
    QStrings m_names;

    QString m_map_name;
    ColourMap m_current_map;

    static boost::shared_ptr<ColourMaps> s_instance;
};

