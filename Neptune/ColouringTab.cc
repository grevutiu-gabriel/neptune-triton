//
// File:   ColouringTab.cc
// Author: M.R.Eggleston
//
// Created on 06 October 2010, 11:06
//

#include <iostream>
#include <QVBoxLayout>
#include "ColourMaps.h"
#include "ColouringTab.h"

ColouringTab::ColouringTab(QWidget* parent)
:
QWidget(parent),
m_inner_type_combo(new InnerTypeComboBox()),
m_compound_combo(new CompoundImageComboBox()),
m_bailout_widget(new QWidget()),
m_colouring_combo(new ColouringCombobox()),
m_inner_colouring(new BailoutColouring(true)),
m_outer_colouring(new BailoutColouring(false)),
m_orbit_colouring(new OrbitColouring()),
m_map1_label(new QLabel()),
m_map2_label(new QLabel()),
m_ignore_signals(false),
m_old_red(0),
m_old_green(0),
m_old_blue(0),
m_colour_maps(ColourMaps::instance()),
m_inner(false)
{
    m_config.SetFractalType(Config::instance()->getFractalType());
    m_config.setInner(false);

    m_map1_image = QImage(Waveform::NUMBER_OF_COLOURS, 35, QImage::Format_RGB32);
    m_map1_image.fill(0);
    m_map1_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_map1_label->setPixmap(QPixmap::fromImage(m_map1_image));
    m_map2_image = QImage(Waveform::NUMBER_OF_COLOURS, 35, QImage::Format_RGB32);
    m_map2_image.fill(0);
    m_map2_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_map2_label->setPixmap(QPixmap::fromImage(m_map1_image));

    setup_ui_layout();
    connect_signals();
}

ColouringTab::~ColouringTab()
{
}

void ColouringTab::setup_ui_layout()
{
    auto main_layout = new QVBoxLayout();
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    auto bailout_layout = new QVBoxLayout();
    bailout_layout->setSpacing(0);
    bailout_layout->setContentsMargins(0, 0, 0, 0);
    auto hlayout1 = new QHBoxLayout();
    hlayout1->setSpacing(0);
    hlayout1->setContentsMargins(0, 0, 0, 0);
    hlayout1->addWidget(m_inner_type_combo);
    hlayout1->addStretch();
    bailout_layout->addLayout(hlayout1);
    auto hlayout2 = new QHBoxLayout();
    hlayout2->setSpacing(0);
    hlayout2->setContentsMargins(0, 0, 0, 0);
    hlayout2->addWidget(m_compound_combo);
    hlayout2->addStretch();
    bailout_layout->addLayout(hlayout2);
    auto hlayout3 = new QHBoxLayout();
    hlayout3->setSpacing(0);
    hlayout3->setContentsMargins(0, 0, 0, 0);
    hlayout3->addWidget(m_colouring_combo);
    hlayout3->addStretch();
    bailout_layout->addLayout(hlayout3);
    m_outer_colouring->hide();
    bailout_layout->addWidget(m_outer_colouring);
    bailout_layout->addWidget(m_inner_colouring);
    m_bailout_widget->setLayout(bailout_layout);
    main_layout->addWidget(m_bailout_widget);
    main_layout->addWidget(m_orbit_colouring);
    
    main_layout->addStretch();
    main_layout->addWidget(m_map1_label);
    main_layout->addWidget(m_map2_label);
    
    setLayout(main_layout);
}

void ColouringTab::connect_signals()
{
    connect(m_colouring_combo, SIGNAL(currentIndexChanged(int)),
            this,              SLOT(on_colouring_changed(int)));
    connect(m_orbit_colouring, SIGNAL(orbit_colouring_changed()),
            this,              SLOT(on_orbit_colour_changed()));
    connect(m_inner_colouring->single_colour(), SIGNAL(changed(const QColor&)),
            this,              SLOT(on_fixed_colour_changed(const QColor&)));
    connect(m_outer_colouring->single_colour(), SIGNAL(changed(const QColor&)),
            this,              SLOT(on_fixed_colour_changed(const QColor&)));
    connect(ColourMaps::instance().get(), SIGNAL(changed(const QString)),
            this,              SLOT(on_colour_maps_changed(const QString)));
    connect(m_inner_type_combo, SIGNAL(currentIndexChanged(int)),
            this,              SLOT(on_inner_type_changed(int)));
    connect(m_compound_combo, SIGNAL(currentIndexChanged(int)),
            this,              SLOT(on_compound_image_changed(int)));
}

void ColouringTab::setColourMap(const QString &map_name)
{
    if (!m_ignore_signals)
    {
        m_config.setColourMap(map_name);
        display();
    }
}

void ColouringTab::display()
{
    m_ignore_signals = true;
    if (m_config.isOrbitPlotting())
    {
        m_bailout_widget->hide();
        m_orbit_colouring->show();
        m_orbit_colouring->display();
        m_map2_label->hide();
    }
    else
    {
        m_bailout_widget->show();
        m_orbit_colouring->hide();
        display_for_bailout_fractals();
        m_map2_label->show();
    }
    drawColourMaps();
    m_ignore_signals = false;
}

void ColouringTab::display_for_bailout_fractals()
{
    m_inner_type_combo->setValue(m_config.innerType());
    m_compound_combo->setValue(m_config.compoundImage());
    m_colouring_combo->setValue(m_inner ? INNER : OUTER);
    if (m_inner)
    {
        m_inner_colouring->display();
        m_inner_colouring->show();
        m_outer_colouring->hide();
    }
    else
    {
        m_outer_colouring->display();
        m_outer_colouring->show();
        m_inner_colouring->hide();
    }
}

void ColouringTab::on_colouring_changed(int index)
{
    bool inner = (index == INNER);
    if (inner != m_inner)
    {
        m_inner = inner;
        m_config.setInner(m_inner);
        auto method = m_config.getMethod();
        emit colouring_changed(method[0] != 'f' ? m_config.getColourMap() : "");
    }
}

void ColouringTab::setFractalType(FRACTAL_TYPE ft)
{
    m_config.SetFractalType(ft);
    m_orbit_colouring->setFractalType(ft);
    m_inner_colouring->setFractalType(ft);
    m_outer_colouring->setFractalType(ft);
    display();
}

void ColouringTab::on_fixed_colour_changed(const QColor& colour)
{
    if (!m_ignore_signals)
    {
        int red = colour.red();
        int green = colour.green();
        int blue = colour.blue();
        std::stringstream str;
        str << "f," << red << "," << green << "," << blue;
        std::string method(str.str());
        m_config.setMethod(method);
        if ((red != m_old_red) || (green != m_old_green) || (blue != m_old_blue))
        {
            emit colouring_changed("");
        }
        m_old_red = red;
        m_old_green = green;
        m_old_blue = blue;
        drawColourMaps();
    }
}

void ColouringTab::on_orbit_colour_changed()
{
    if (!m_ignore_signals)
    {
        emit colouring_changed(m_config.getColourMap());
    }
}

void ColouringTab::on_inner_type_changed(int)
{
    if (!m_ignore_signals)
    {
        auto new_value = m_inner_type_combo->getValue();
        auto old_value = m_config.innerType();
        if (old_value != new_value)
        {
            m_config.setInnerType(new_value);
            emit colouring_changed(m_config.getColourMap());
        }
    }
}

void ColouringTab::on_compound_image_changed(int)
{
    if (!m_ignore_signals)
    {
        auto new_value = m_compound_combo->getValue();
        auto old_value = m_config.compoundImage();
        if (old_value != new_value)
        {
            m_config.setCompoundImage(new_value);
            emit colouring_changed(m_config.getColourMap());
        }
    }
}

void ColouringTab::drawColourMaps()
{
    QColor fixed;
    if (m_config.isOrbitPlotting())
    {
        auto map_name = m_config.getColourMap();
        drawColourMap(m_map1_image, map_name, false);
        m_map1_label->setPixmap(QPixmap::fromImage(m_map1_image));
    }
    else
    {
        auto map_name = m_config.getColourMap(false);
        drawColourMap(m_map1_image, map_name, false);
        m_map1_label->setPixmap(QPixmap::fromImage(m_map1_image));
        map_name = m_config.getColourMap(true);
        drawColourMap(m_map2_image, map_name, true);
        m_map2_label->setPixmap(QPixmap::fromImage(m_map2_image));
    }
}

void ColouringTab::drawColourMap(QImage& image, const QString& map_name, bool inner)
{
    QColor fixed;
    auto method = m_config.getMethod(inner);
    if (method[0] == 'f')
    {
        fixed = QColor(stringToColour(method.substr(2)));
        image.fill(fixed);
    }
    else
    {
        COMPONENT_ORDER order = m_config.getComponentOrder(inner);
        int r_offset = m_config.getRedOffset(inner);
        int g_offset = m_config.getGreenOffset(inner);
        int b_offset = m_config.getBlueOffset(inner);
        ColourMap map = m_colour_maps->getColourMap(map_name);
        map.draw(image, r_offset, g_offset, b_offset, order);
    }
}

const QString& ColouringTab::currentColourMap()
{
    if (m_config.isOrbitPlotting())
    {
        m_colour_map = m_config.getColourMap();
        if (m_colour_map.isEmpty())
        {
            m_colour_map = "Default";
        }
    }
    else
    {
        std::string method = m_config.getMethod();
        if (method[0] == 'f')
        {
            m_colour_map = "";
        }
        else
        {
            m_colour_map = m_config.getColourMap();
        }
    }
    return m_colour_map;
}

const BailoutColouring* ColouringTab::innerColouring()
{
    return m_inner_colouring;
}

const BailoutColouring* ColouringTab::outerColouring()
{
    return m_outer_colouring;
}

void ColouringTab::on_colour_maps_changed(const QString name)
{
    bool deleted = !ColourMaps::instance()->nameExists(name);
    if (m_config.isOrbitPlotting())
    {
        QString map_name = m_config.getColourMap();
        if (map_name == name)
        {
            if (deleted)
            {
                m_config.setColourMap("Default");
                emit colouring_changed(m_config.getColourMap());
            }
        }
    }
    else
    {
        bool send_signal = false;
        for (int i = 0; i < 2; i++)
        {
            bool inner = (i == 1);
            auto method = m_config.getMethod(inner);
            if (method[0] != 'f')
            {
                QString map_name = m_config.getColourMap(inner);
                if (map_name == name)
                {
                    if (deleted)
                    {
                        m_config.setColourMap("Default", inner);
                    }
                    send_signal = true;
                }
            }
        }
        if (send_signal)
        {
            emit colouring_changed(m_config.getColourMap());
        }
    }
}
