// 
// File:   ImportColourMap.cc
// Author: M.R.Eggleston
// 
// Created on 21 February 2012, 10:56
//

#include <iostream>
#include <QDomDocument>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <boost/lexical_cast.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/tokenizer.hpp>
#include "Utilities.h"
#include "ImportColourMap.h"

ImportColourMap::ImportColourMap(QWidget* parent)
:
QTabWidget(parent),
m_browse_button(new QPushButton("Browse")),
m_move_button(new QToolButton()),
m_import_button(new QPushButton("Import")),
m_importing_tree(new ImportingTree()),
m_source_tree(new ImportSourceTree()),
m_messages_text(new QTextEdit()),
m_colour_map_drawing(new QLabel()),
m_source_context_menu(new QMenu()),
m_import_context_menu(new QMenu()),
m_colour_maps(ColourMaps::instance())
{
    setWindowTitle("Import Colour Maps");
    hide();
    m_move_button->setArrowType(Qt::RightArrow);
    m_source_tree->setHeaderHidden(true);
    setup_ui_layout();
    connect_signals();
    
    m_move_button->setEnabled(false);
    m_import_button->setEnabled(false);

    m_colour_map_image = QImage(512, 35, QImage::Format_RGB32);
    m_colour_map_image.fill(0);
    m_colour_map_drawing->setPixmap(QPixmap::fromImage(m_colour_map_image));
    setFixedWidth(512);
    setFixedHeight(480);
}

ImportColourMap::~ImportColourMap()
{
}

void ImportColourMap::setup_ui_layout()
{
    auto select_layout = new QVBoxLayout();
    select_layout->setSpacing(0);
    select_layout->setContentsMargins(0, 0, 0, 0);
    auto hlayout = new QHBoxLayout();
    hlayout->setSpacing(0);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->addWidget(m_browse_button);
    hlayout->addStretch();
    select_layout->addLayout(hlayout);
    hlayout = new QHBoxLayout();
     hlayout->setSpacing(0);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->addWidget(m_source_tree);
    auto hlayout2 = new QHBoxLayout();
    hlayout2->setSpacing(0);
    hlayout2->setContentsMargins(10, 0, 10, 0);
    hlayout2->addWidget(m_move_button);
    hlayout->addLayout(hlayout2);
    hlayout->addWidget(m_importing_tree);
    select_layout->addLayout(hlayout);
    select_layout->addWidget(m_colour_map_drawing);
    hlayout = new QHBoxLayout();
    hlayout->addStretch();
    hlayout->addWidget(m_import_button);
    select_layout->addLayout(hlayout);
    auto widget = new QWidget();
    widget->setLayout(select_layout);
    addTab(widget, "Select");
    
    addTab(m_messages_text, "Messages");
}

void ImportColourMap::connect_signals()
{
    connect(m_browse_button,  SIGNAL(clicked(bool)),
            this,             SLOT(on_browse_button(bool)));
    connect(m_import_button,  SIGNAL(clicked(bool)),
            this,             SLOT(on_import_button(bool)));
    connect(m_move_button,    SIGNAL(clicked(bool)),
            this,             SLOT(on_move_button(bool)));
    connect(m_source_tree,    SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
            this,             SLOT(on_source_cursor(QTreeWidgetItem*, QTreeWidgetItem*)));
    connect(m_importing_tree, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
            this,             SLOT(on_import_cursor(QTreeWidgetItem*, QTreeWidgetItem*)));
    
    auto action = m_source_context_menu->addAction("Remove");
    connect(action, SIGNAL(triggered()), this, SLOT(on_source_remove()));
    action = m_source_context_menu->addAction("Remove all");
    connect(action, SIGNAL(triggered()), this, SLOT(on_source_remove_all()));
    
    m_source_tree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_source_tree,      SIGNAL(customContextMenuRequested(const QPoint&)),
            this,               SLOT(on_source_context_menu(const QPoint&)));

    action = m_import_context_menu->addAction("Remove");
    connect(action, SIGNAL(triggered()), this, SLOT(on_import_remove()));
    action = m_import_context_menu->addAction("Remove all");
    connect(action, SIGNAL(triggered()), this, SLOT(on_import_remove_all()));
    
    m_importing_tree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_importing_tree,   SIGNAL(customContextMenuRequested(const QPoint&)),
            this,               SLOT(on_import_context_menu(const QPoint&)));
}

void ImportColourMap::on_browse_button(bool)
{
    auto file_names = QFileDialog::getOpenFileNames(this,
                                                    "Import Colour Maps ...",
                                                    QDir::homePath(),
                                                    "scm files (*.scm);;ugr files (*.ugr);;map files (*.map)");
    load(file_names);
}

void ImportColourMap::on_move_button(bool)
{
    QString collection;
    QStringList maps = m_source_tree->getMapsAtCursor(collection);
    for (auto it = maps.begin(); it != maps.end(); it++)
    {
        QString name = *it;
        QString import_name = name;
        if (!m_importing_tree->mapExists(collection, name))
        {
            // Only add map from collection if it isn't already in
            // the importing tree
            if (m_importing_tree->importingMapExists(name))
            {
                import_name = assignTemporaryName(name);
            }
            m_importing_tree->addMap(import_name, collection, name);
        }
    }
    map_identity id  = m_source_tree->getMapAtNext();
    m_source_tree->setCursorAtMap(id.collection, id.name);
    m_import_button->setEnabled(true);
    on_source_cursor(nullptr, nullptr);
}

void ImportColourMap::on_import_button(bool)
{
    std::vector<map_full_identity> ids = m_importing_tree->getMaps();
    int size = ids.size();
    for (int i = 0; i < size; i++)
    {
        map_full_identity& id = ids[i];
        m_colour_maps->updateMaps(id.import_name,
                                  id.import_name,
                                  m_import_maps[id.collection][id.name],
                                  i == (size - 1));
        m_source_tree->removeMap(id.collection, id.name);
        deleteFromImportMap(id.collection, id.name);
    }
    m_importing_tree->clear();
    m_import_button->setEnabled(false);
    drawMap(); // redraw colour map, there should be no cursor so black should be displayed.
}

void ImportColourMap::load(QStringList& names)
{
    startMessages();
    for (auto it = names.begin(); it != names.end(); it++)
    {
        QString filename = *it;
        if (filename.length() > 4)
        {
            auto extension = filename.right(4).toLower();
            if (extension == ".map")
            {
                importMapFile(filename);
            }
            else if (extension == ".ugr")
            {
                importUgrFile(filename);
            }
            else if (extension == ".scm")
            {
                importScmFile(filename);
            }
        }
    }
    writeMessages();
    loadSourceTree();
}

void ImportColourMap::loadSourceTree()
{
    m_source_tree->clear();
    for (auto it1 = m_import_maps.begin(); it1 != m_import_maps.end(); it1++)
    {
        Map &maps = it1->second;
        for (auto it2 = maps.begin(); it2 != maps.end(); it2++)
        {
            m_source_tree->addMap(it1->first, it2->first);
        }
    }
}

void ImportColourMap::importMapFile(const QString& path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly))
    {
        QString collection_name = name_only(path);
        QString name = collection_name;
        std::string s;
        while (!file.atEnd())
        {
            QByteArray data = file.readLine();
            int length = data.length();
            if (length > 12)
            {
                length = 12;
            }
            if (length != 0)
            {
                s += std::string(data.data(), length);
            }
        }
        if (s.empty())
        {
            // no data read, empty file or unknown error occurred
            mapImportError(name);
        }
        else
        {
            boost::tokenizer<> tokens(s);
            int i = 0;
            name.truncate(name.length() - 4);

            mapFileMessage(name);
            Map maps;

            try
            {
                ColourMap colour_map;
                int r = 0;
                int g = 0;
                int b = 0;
                for (auto it = tokens.begin(); it != tokens.end(); it++)
                {
                    std::string value = *it;
                    int ivalue = boost::lexical_cast<int>(value);
                    switch (i % 3)
                    {
                    case 0:
                        r = ivalue;
                        break;
                    case 1:
                        g = ivalue;
                        break;
                    case 2:
                        b = ivalue;
                        colour_map.setColour(i/3, QColor(r, g, b));
                        break;
                    }
                    i++;
                }
                addImportMap(name, maps, colour_map);
                if (!maps.empty())
                {
                    m_import_maps[collection_name] = maps;
                }
            }
            catch (boost::bad_lexical_cast&)
            {
                mapImportError(name);
            }
        }
    }
    else
    {
        // error add error message to messages
    }
}

void ImportColourMap::importUgrFile(const QString& path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();
        if (data.isEmpty())
        {
            // no data read, emppty file or unknown error occurred
        }
        else
        {
            std::string s(data.data(), data.length());
            data.clear();

            QString collection_name = name_only(path);
            mapFileMessage(collection_name);

            boost::char_separator<char> sep(" =\"\n\r");
            boost::tokenizer<boost::char_separator<char>> tokens(s, sep);
            int temp_import_number = 0;
            QString base_name = "~~~IMPORT";
            Map maps;
            ColourMap colour_map;
            auto it = tokens.begin();
            while (it != tokens.end())
            {
                try
                {
                    // Look for "gradient:"
                    QString token = QString::fromStdString(*it);
                    QString title;
                    bool start_found = false;
                    while (it != tokens.end() && !start_found)
                    {
                        start_found = (*it ==  "gradient:");
                        it++;
                    };
                    if (start_found)
                    {
                        bool end_found = false;
                        int index = 0;
                        int colour = 0;
                        colour_map.clear();
                        while (it != tokens.end() && !end_found)
                        {
                            token = QString::fromStdString(*it);
                            if (token == "title")
                            {
                                it++;
                                if (it != tokens.end())
                                {
                                    QString name = QString::fromStdString(*it);
                                    if ((name != "Default") &&
                                        (name != "Default 1") &&
                                        (name != "Default 2") &&
                                        (name != "Default 3") &&
                                        (name != "Default 4"))
                                    {
                                        title = name;
                                    }
                                }
                            }
                            else if (token == "opacity:" || token == "}")
                            {
                                // no more gradient find end of definition
                                while (token != "}")
                                {
                                    it++;
                                    token = QString::fromStdString(*it);
                                    if (it == tokens.end()) token = "}";
                                };
                                if (title.isEmpty())
                                {
                                    std::stringstream str;
                                    str << base_name.toStdString() << temp_import_number;
                                    title = QString::fromStdString(str.str());
                                    temp_import_number++;
                                }
                                addImportMap(title, maps, colour_map);
                                end_found = true;
                            }
                            else if (token == "index")
                            {
                                it++;
                                index = boost::lexical_cast<int>(*it);
                            }
                            else if (token == "color")
                            {
                                it++;
                                colour = boost::lexical_cast<int>(*it);
                                int r = colour % 256;
                                int g = (colour / 256) % 256;
                                int b = colour / (256*256);
                                colour_map.setColour(index, QColor(r, g, b));
                            }
                            it++;
                        }
                    }
                }
                catch (boost::bad_lexical_cast &ex)
                {
                    exceptionMessage(collection_name, ex);
                }
            }
            if (!maps.empty())
            {
                m_import_maps[collection_name] = maps;
            }
        }
    }
    else
    {
        // error opening file
    }
}

void ImportColourMap::importScmFile(const QString& path)
{
    QString collection_name = name_only(path);
    Map maps;
    boost::scoped_ptr<QDomDocument> dom(new QDomDocument());
    QFile file(path);
    if (file.open(QIODevice::ReadOnly))
    {
        bool ok = dom->setContent(&file);
        file.close();
        if (ok)
        {
            auto root = dom->documentElement();
            if (!root.isNull())
            {
                if (root.tagName() == "saturn_colour_maps")
                {
                    auto child = root.firstChildElement();
                    while (!child.isNull())
                    {
                        if (child.tagName() == "colourmap")
                        {
                            QString map_name;
                            ColourMap map;
                            load_colour_map_child(child, map, map_name);
                            addImportMap(map_name, maps, map);
                        }
                        child = child.nextSiblingElement();
                    }
                }
                else
                {
                    badScmFileMessage(path);
                }
            }
        }
        m_import_maps[collection_name] = maps;
    }
    else
    {
        // failed to open ...
    }
}

void ImportColourMap::addImportMap(const QString& name, Map& maps, ColourMap& colour_map)
{
    QString found_name;
    bool map_exists = m_colour_maps->mapExists(colour_map, found_name);
    if (map_exists)
    {
        mapExistsMessage(name, found_name);
    }
    else
    {
        bool found = false;
        for (auto it = m_import_maps.begin(); it != m_import_maps.end(); it++)
        {
            Map colour_maps = it->second;
            for (auto it2 = colour_maps.begin(); it2 != colour_maps.end(); it2++)
            {
                ColourMap &map = it2->second;
                found = map.equal(colour_map);
                if (found)
                {
                    mapAlreadyLoadedMessage(name);
                    break;
                }
            }
            if (found)
            {
                break;
            }
        }
        if (!found)
        {
            maps[name] = colour_map;
        }
    }
}

void ImportColourMap::mapExistsMessage(const QString& name, const QString& found_name)
{
    QString name_prefix = (name.length() > 3 ? name.left(3) : "");
    QString found_name_prefix = (found_name.length() > 3 ? found_name.left(3) : "");
    
    if ((found_name_prefix == "~~~") && (name_prefix != "~~~"))
    {
        m_colour_maps->rename(found_name, name);
        m_messages += "    " + found_name + " renamed " + name;
    }
    else
    {
        m_messages += "    " + name + " already exists";
        if (name != found_name)
        {
            m_messages += " as " + found_name;
        }
    }
    m_messages += "\n";
}

void ImportColourMap::mapImportError(const QString& name)
{
    m_messages += "    failed to import " + name + "\n";
}

void ImportColourMap::mapAlreadyLoadedMessage(const QString& name)
{
    m_messages += "    map " + name + " already loaded\n";
}

void ImportColourMap::startMessages()
{
    m_messages = "Loading ...\n";
}

void ImportColourMap::mapFileMessage(const QString& name)
{
    m_messages += "  importing " + name + " ...\n";
}

void ImportColourMap::exceptionMessage(const QString &name, std::exception& ex)
{
    m_messages += "    failed to import " + name + "\n";
    m_messages += "\nexception: " + QString::fromStdString(ex.what()) + "\n\n";
}

void ImportColourMap::badScmFileMessage(const QString& name)
{
    m_messages += "    " + name + " not recognised as a Saturn Colour Maps file\n";
}

void ImportColourMap::writeMessages()
{
    m_messages_text->setText(m_messages);
}

void ImportColourMap::on_source_cursor(QTreeWidgetItem*, QTreeWidgetItem*)
{
    bool enable_move = false;
    map_identity id = m_source_tree->colourMapAtCursor();
    // If id.collection is empty there isn't a cursor ...
    if (!id.collection.isEmpty())
    {
        if (id.name.isEmpty())
        {
            // At root check whether any of the maps in the branch
            // are not in the importing tree.
            auto maps = m_source_tree->getMapsAtCursor(id.collection);
            for (auto it = maps.begin(); it != maps.end(); it++)
            {
                enable_move = !m_importing_tree->mapExists(id.collection, *it);
                if (enable_move)
                {
                    break;
                }
            }
        }
        else
        {
            enable_move = !m_importing_tree->mapExists(id.collection, id.name);
        }
        m_importing_tree->setCursorAtMap(id.collection, id.name);
        m_move_button->setEnabled(enable_move);
        drawMap();
    }
}

void ImportColourMap::on_import_cursor(QTreeWidgetItem*, QTreeWidgetItem*)
{
    map_identity id = m_importing_tree->colourMapAtCursor();
    m_source_tree->setCursorAtMap(id.collection, id.name);
    m_move_button->setEnabled(false);
    drawMap();
}

void ImportColourMap::deleteFromImportMap(const QString& collection, const QString& name)
{
    auto collection_it = m_import_maps.find(collection);
    if (collection_it != m_import_maps.end())
   {
        if (name.isEmpty())
        {
            m_import_maps.erase(collection_it);
        }
        else
        {
            auto item_it = (collection_it->second).find(name);
            if (item_it != (collection_it->second).end())
            {
                (collection_it->second).erase(item_it);
            }
        }
    }
}

void ImportColourMap::on_source_remove()
{
    auto id = m_source_tree->colourMapAtCursor();
    if (id.name.isEmpty())
    {
        m_source_tree->deleteBranchAtCursor();
        m_importing_tree->deleteCollection(id.collection);
        deleteFromImportMap(id.collection, "");
    }
    else
    {
        m_source_tree->deleteItemAtCursor();
        m_importing_tree->deleteImportingMap(id.collection, id.name);
        deleteFromImportMap(id.collection, id.name);
    }
    m_import_button->setEnabled(m_importing_tree->topLevelItemCount() != 0);
    m_move_button->setEnabled(false);
}

void ImportColourMap::on_source_remove_all()
{
    m_source_tree->clear();
    m_importing_tree->clear();
    m_import_maps.clear();
    m_move_button->setEnabled(false);
    m_import_button->setEnabled(false);
}

void ImportColourMap::on_import_remove()
{
    auto id = m_importing_tree->colourMapAtCursor();
    if (!id.name.isEmpty())
    {
        m_importing_tree->deleteImportingMap(id.collection, id.name);
    }
    m_move_button->setEnabled(true);
    m_import_button->setEnabled(m_importing_tree->topLevelItemCount() != 0);
}

void ImportColourMap::on_import_remove_all()
{
    m_importing_tree->clear();
    m_move_button->setEnabled(true);
    m_import_button->setEnabled(false);
}

void ImportColourMap::on_source_context_menu(const QPoint& pos)
{
    if (m_source_tree->topLevelItemCount() > 0)
    {
        m_source_context_menu->popup(m_source_tree->mapToGlobal(pos));
    }
}

void ImportColourMap::on_import_context_menu(const QPoint& pos)
{
    if (m_importing_tree->topLevelItemCount() > 0)
    {
        m_import_context_menu->popup(m_importing_tree->mapToGlobal(pos));
    }
}

void ImportColourMap::load_colour_map_child(const QDomElement& root, ColourMap &map, QString& name)
{
    QDomElement child = root.firstChildElement();
    name = root.attribute("name");
    ColourMap::MAP_TYPE type = ColourMap::CM_MANUAL;
    QString type_attribute = root.attribute("auto");
    if (type_attribute.isEmpty())
    {
        type_attribute = root.attribute("type");
        type = static_cast<ColourMap::MAP_TYPE>(type_attribute.toInt());
    }
    else
    {
        type = (type_attribute == "1" ? ColourMap::CM_AUTO : ColourMap::CM_MANUAL);
    }
    map.setType(type);
    QString tag_name;
    while (!child.isNull())
    {
        tag_name = child.tagName();
        if (tag_name == "rgb")
        {
            unsigned int index = child.attribute("index").toUInt();
            QRgb colour = qRgb(child.attribute("red").toInt(),
                               child.attribute("green").toInt(),
                               child.attribute("blue").toInt());
            map.setColour(index, colour);
        }
        else if ((tag_name == "red") && (type == ColourMap::CM_AUTO)) 
        {
            load_auto_colour(child, RGB_RED, map);
        }
        else if ((tag_name == "green") && (type == ColourMap::CM_AUTO)) 
        {
            load_auto_colour(child, RGB_GREEN, map);
        }
        else if ((tag_name == "blue") && (type == ColourMap::CM_AUTO)) 
        {
            load_auto_colour(child, RGB_BLUE, map);
        }
        else if (tag_name == "red")
        {
            unsigned int index = child.attribute("index").toUInt();
            unsigned int value = child.attribute("value").toUInt();
            map.setRed(index, value);
        }
        else if (tag_name == "green")
        {
            unsigned int index = child.attribute("index").toUInt();
            unsigned int value = child.attribute("value").toUInt();
            map.setGreen(index, value);
        }
        else if (tag_name == "blue")
        {
            unsigned int index = child.attribute("index").toUInt();
            unsigned int value = child.attribute("value").toUInt();
            map.setBlue(index, value);
        }
        child = child.nextSiblingElement();
    }
}

void ImportColourMap::load_auto_colour(const QDomElement& child, RGB colour, ColourMap &map)
{
    Waveform::AutoColour auto_colour;
    auto_colour.hoffset = child.attribute("hoffset").toInt();
    auto_colour.voffset = child.attribute("voffset").toInt();
    auto_colour.maximum = child.attribute("maximum").toInt();
    auto_colour.wave = static_cast<Waveform::WAVE>(child.attribute("wave").toUInt());
    auto_colour.cycles = child.attribute("cycles").toUInt();
    map.setAutoColour(colour, auto_colour);
}

void ImportColourMap::drawMap()
{
    map_identity id = m_source_tree->colourMapAtCursor();
    if (id.name.isEmpty())
    {
        m_colour_map_image.fill(0);
        m_colour_map_drawing->setPixmap(QPixmap::fromImage(m_colour_map_image));
    }
    else
    {
        ColourMap map = m_import_maps[id.collection][id.name];
        map.draw(m_colour_map_image);
        m_colour_map_drawing->setPixmap(QPixmap::fromImage(m_colour_map_image));
    }
}

QString ImportColourMap::assignTemporaryName(const QString &base)
{
    int i = 0;
    QString name = base + "~" + QString::number(i);
    while (m_importing_tree->importingMapExists(name))
    {
        i++;
        name = base + "~" + QString::number(i);
    }
    while (m_colour_maps->nameExists(name))
    {
        i++;
        name = base + "~" + QString::number(i);
    }
    return name;
}
