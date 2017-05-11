// 
// File:   Trap.h
// Author: M.R.Eggleston
//
// Created on 08 April 2013, 14:37
//
// Taken from ColouringTabContainers .cc and converted to Qt

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "Trap.h"
#include "Config.h"
#include "Utilities.h"


Trap::Trap(bool inner, QWidget* parent)
:
QWidget(parent),
m_inner(inner),
m_trap_combo(new OrbitTrapTypeCombobox()),
m_trap_size_entry(new NumberEntry(NumberEntry::FLOAT)),
m_trap_position_r_entry(new NumberEntry(NumberEntry::FLOAT)),
m_trap_position_i_entry(new NumberEntry(NumberEntry::FLOAT)),
m_trap_change_in_distance_check(new QCheckBox()),
m_trap_rotation_spin(new QSpinBox()),
m_trap_centre_point_check(new QCheckBox("Centre Point")),
m_trap_image_centre_button(new QPushButton("Use Image Centre")),
m_trap_scale_button(new QPushButton("Scale")),
m_steiner_chain_widget(new QWidget()),
m_invd_entry(new NumberEntry(NumberEntry::FLOAT)),
m_chain_length_spin(new QSpinBox()),
m_inner_circle_check(new QCheckBox("Inner Circle")),
m_outer_circle_check(new QCheckBox("Outer Circle")),
m_centres_check(new QCheckBox("Centre Points")),
m_triangles_check(new QCheckBox("Triangles")),
m_ignore_signals(false)
{
    m_config.SetFractalType(Config::instance()->getFractalType());
    m_config.setInner(m_inner);
    setup_ui_layout();

    m_trap_rotation_spin->setRange(-179, 180);
    m_trap_rotation_spin->setWrapping(true);
    m_chain_length_spin->setRange(static_cast<int>(SteinerChain::MIN_CHAIN_CIRCLES),
                                  static_cast<int>(SteinerChain::MAX_CHAIN_CIRCLES));
    connect_signals();
}

Trap::~Trap()
{
}

void Trap::setup_ui_layout()
{
    auto main_layout = new QVBoxLayout();
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    auto hlayout = new QHBoxLayout();
    hlayout->setSpacing(5);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->addWidget(m_trap_combo);
    hlayout->addStretch();
    main_layout->addLayout(hlayout);
    auto glayout = new QGridLayout();
    glayout->setHorizontalSpacing(5);
    glayout->setVerticalSpacing(0);
    glayout->setContentsMargins(0, 0, 0, 0);
    glayout->addWidget(new QLabel("Centre"), 0, 0);
    glayout->addWidget(new QLabel("r"), 0, 1);
    glayout->addWidget(m_trap_position_r_entry, 0, 2);
    glayout->addWidget(new QLabel("i"), 0, 3);
    glayout->addWidget(m_trap_position_i_entry, 0, 4);
    glayout->addWidget(new QLabel("Width"), 1, 0);
    glayout->addWidget(m_trap_size_entry, 1, 2);
    glayout->addWidget(m_trap_centre_point_check, 1, 3, 1, 4);
    glayout->addWidget(new QLabel("Rotation"), 2, 0);
    hlayout = new QHBoxLayout();
    hlayout->setSpacing(0);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->addWidget(m_trap_rotation_spin);
    hlayout->addStretch();
    hlayout->addWidget(m_trap_scale_button);
    hlayout->addWidget(m_trap_image_centre_button);
    glayout->addLayout(hlayout, 2, 2, 1, 3);
    main_layout->addLayout(glayout);
    auto vlayout = new QVBoxLayout();
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 0, 0, 0);
    auto line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    vlayout->addWidget(line);
    hlayout = new QHBoxLayout();
    hlayout->setSpacing(5);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->addWidget(new QLabel("Length"));
    hlayout->addWidget(m_chain_length_spin);
    hlayout->addWidget(new QLabel("Inversion distance"));
    hlayout->addWidget(m_invd_entry);
    vlayout->addLayout(hlayout);
    hlayout = new QHBoxLayout();
    hlayout->setSpacing(5);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->addWidget(m_outer_circle_check);
    hlayout->addWidget(m_inner_circle_check);
    hlayout->addWidget(m_centres_check);
    hlayout->addWidget(m_triangles_check);
    vlayout->addLayout(hlayout);
    m_steiner_chain_widget->setLayout(vlayout);
    m_steiner_chain_widget->hide();
    main_layout->addWidget(m_steiner_chain_widget);
    
    setLayout(main_layout);
}

void Trap::connect_signals()
{
    connect(m_trap_combo, SIGNAL(currentIndexChanged(int)),
            this,         SLOT(on_trap_changed(int)));
    connect(m_trap_size_entry, SIGNAL(changed()),
            this,         SLOT(on_trap_size_changed()));
    connect(m_trap_position_r_entry, SIGNAL(changed()),
            this,         SLOT(on_trap_position_changed()));
    connect(m_trap_position_i_entry, SIGNAL(changed()),
            this,         SLOT(on_trap_position_changed()));
    connect(m_trap_rotation_spin, SIGNAL(valueChanged(int)),
            this,         SLOT(on_trap_rotation_changed(int)));
    connect(m_invd_entry, SIGNAL(changed()),
            this,         SLOT(on_inversion_distance_changed()));
    connect(m_chain_length_spin, SIGNAL(valueChanged(int)),
            this,         SLOT(on_chain_length_changed(int)));
    connect(m_inner_circle_check, SIGNAL(stateChanged(int)),
            this,         SLOT(on_inner_check_changed(int)));
    connect(m_outer_circle_check, SIGNAL(stateChanged(int)),
            this,         SLOT(on_outer_check_changed(int)));
    connect(m_centres_check, SIGNAL(stateChanged(int)),
            this,         SLOT(on_centre_points_check_changed(int)));
    connect(m_triangles_check, SIGNAL(stateChanged(int)),
            this,         SLOT(on_triangles_check_changed(int)));
    connect(m_trap_image_centre_button, SIGNAL(clicked(bool)),
            this,         SLOT(on_image_centre_button(bool)));
    connect(m_trap_scale_button, SIGNAL(clicked(bool)),
            this,         SLOT(on_trap_scale_button(bool)));
    connect(m_trap_centre_point_check, SIGNAL(stateChanged(int)),
            this,         SLOT(on_trap_centre_point_changed(int)));
}

void Trap::SetFractalType(FRACTAL_TYPE ft)
{
    m_config.SetFractalType(ft);
}

void Trap::display()
{
    m_ignore_signals = true;
    m_method = m_config.getMethod();
    char type_ch = m_method[1];
    auto type = static_cast<OrbitTrap::TRAP_TYPE>(type_ch - 'a');
    m_trap_config.setTrapType(m_inner, type);
    m_trap_config.configFromString(m_method.substr(3));
    m_trap_combo->setValue(type);
    bool hasSize = m_trap_config.hasSize();
    m_trap_size_entry->Set(hasSize ? longdoubleToString(m_trap_config.getWidth()) : "0");
    m_trap_size_entry->setEnabled(hasSize);
    bool hasRotation = m_trap_config.hasRotation();
    m_trap_rotation_spin->setValue(hasRotation ? m_trap_config.getRotation() : 0);
    m_trap_rotation_spin->setEnabled(hasRotation);
    bool hasCentrePoint = m_trap_config.hasCentrePoint();
    if (hasCentrePoint)
    {
        m_trap_centre_point_check->setCheckState(m_trap_config.getCentrePoint() ? Qt::Checked : Qt::Unchecked);
        m_trap_centre_point_check->show();
    }
    else
    {
        m_trap_centre_point_check->hide();
    }
    LongComplex pos = m_trap_config.getPosition();
    m_trap_position_r_entry->Set(longdoubleToString(real(pos)));
    m_trap_position_i_entry->Set(longdoubleToString(imag(pos)));
    if (type == OrbitTrap::STEINER_CHAIN)
    {
        int chain_circles = m_trap_config.getChainCircles();
//        double max_angle = std::ceil(360.0/static_cast<double>(chain_circles));
        m_invd_entry->Set(longdoubleToString(m_trap_config.getInversionDistance()));
        m_chain_length_spin->setValue(chain_circles);
        m_inner_circle_check->setCheckState(m_trap_config.getInnerCircle() ? Qt::Checked : Qt::Unchecked);
        m_outer_circle_check->setCheckState(m_trap_config.getOuterCircle() ? Qt::Checked : Qt::Unchecked);
        m_centres_check->setCheckState(m_trap_config.getCentrePoints() ? Qt::Checked : Qt::Unchecked);
        m_triangles_check->setCheckState(m_trap_config.getTriangles() ? Qt::Checked : Qt::Unchecked);
        m_steiner_chain_widget->show();
    }
    else
    {
        m_steiner_chain_widget->hide();
    }
    m_ignore_signals = false;
}

std::string Trap::asString()
{
    return m_trap_config.configAsString();
}

void Trap::on_trap_changed(int)
{
    if (!m_ignore_signals)
    {
        char type = static_cast<char>(m_trap_combo->getValue() + 'a');
        std::string method = m_config.getMethod();
        std::string new_method = getTrapMethod(type);
        if (method != new_method)
        {
            m_config.setMethod(new_method);
            emit trap_changed();
        }
    }
}

void Trap::on_trap_size_changed()
{
    if (!m_ignore_signals)
    {
        long double size = m_trap_config.getWidth();
        long double new_size = stringToLongDouble(m_trap_size_entry->Get());
        if (size != new_size)
        {
            m_trap_config.setWidth(new_size);
            updateTrapMethod();
            emit trap_changed();
        }
    }
}

void Trap::on_trap_position_changed()
{
    if (!m_ignore_signals)
    {
        LongComplex position = m_trap_config.getPosition();
        LongComplex new_position = stdStringToLongComplex(m_trap_position_r_entry->Get(), m_trap_position_i_entry->Get());
        if (position != new_position)
        {
            m_trap_config.setPosition(new_position);
            updateTrapMethod();
            emit trap_changed();
        }
    }
}

void Trap::on_trap_rotation_changed(int value)
{
    if (!m_ignore_signals)
    {
        m_trap_config.setRotation(value);
        updateTrapMethod();
        emit trap_changed();
    }
}

void Trap::on_centre_point_changed()
{
    if (!m_ignore_signals)
    {
        std::string method = m_config.getMethod();
        bool old_enabled = m_trap_config.getCentrePoint();
        bool new_enabled = (m_trap_centre_point_check->checkState() == Qt::Checked);
        if (old_enabled != new_enabled)
        {
            m_trap_config.setCentrePoint(new_enabled);
            updateTrapMethod();
            emit trap_changed();
        }
    }
}

std::string Trap::getTrapMethod(char type)
{
    m_trap_config.setTrapType(m_inner, static_cast<OrbitTrap::TRAP_TYPE>(type - 'a'));
    std::string method = "t";
    method += type;
    method += m_config.getMethod()[2];
    method += ",";
    method += m_trap_config.configAsString();
    return method;
}

void Trap::updateTrapMethod()
{
    std::string method = m_config.getMethod().substr(0,4);
    method += m_trap_config.configAsString();
    m_config.setMethod(method);
}

void Trap::on_image_centre_button(bool)
{
    auto current_centre = m_trap_config.getPosition();
    auto c = m_config.getCentre();
    auto required_centre = stdStringToLongComplex(c.r, c.i);
    if (current_centre != required_centre)
    {
        m_trap_config.setPosition(required_centre);
        updateTrapMethod();
        emit trap_changed();
    }
}

void Trap::on_trap_scale_button(bool)
{
    auto current_width = m_trap_config.getWidth();
    auto required_width = stringToLongDouble(m_config.getCurrentWidth())*0.5L;
    if (current_width != required_width)
    {
        m_trap_config.setWidth(required_width);
        updateTrapMethod();
        emit trap_changed();
    }
}

void Trap::on_inversion_distance_changed()
{
    if (!m_ignore_signals)
    {
        long double old_invd = m_trap_config.getInversionDistance();
        long double new_invd = stringToLongDouble(m_invd_entry->Get());
        if (new_invd < -0.9L)
        {
            new_invd = -0.9L;
            m_invd_entry->Set(longdoubleToString(new_invd));
        }
        else if (new_invd > 0.9L)
        {
            new_invd = 0.9L;
            m_invd_entry->Set(longdoubleToString(new_invd));
        }
        if (old_invd != new_invd)
        {
            m_trap_config.setInversionDistance(new_invd);
            updateTrapMethod();
            emit trap_changed();
        }
    }
}

void Trap::on_chain_length_changed(int value)
{
    if (!m_ignore_signals)
    {
        m_trap_config.setChainCircles(value);
        updateTrapMethod();
        emit trap_changed();
    }
}

void Trap::on_outer_check_changed(int state)
{
    if (!m_ignore_signals)
    {
        auto old_outer = m_trap_config.getOuterCircle();
        auto new_outer = (state == Qt::Checked);
        if (old_outer != new_outer)
        {
            m_trap_config.setOuterCircle(new_outer);
            updateTrapMethod();
            emit trap_changed();
        }
    }
}

void Trap::on_inner_check_changed(int state)
{
    if (!m_ignore_signals)
    {
        auto old_inner = m_trap_config.getInnerCircle();
        auto new_inner = (state == Qt::Checked);
        if (old_inner != new_inner)
        {
            m_trap_config.setInnerCircle(new_inner);
            updateTrapMethod();
            emit trap_changed();
        }
    }
}

void Trap::on_centre_points_check_changed(int state)
{
    if (!m_ignore_signals)
    {
        auto old_centres = m_trap_config.getCentrePoints();
        auto new_centres = (state == Qt::Checked);
        if (old_centres != new_centres)
        {
            m_trap_config.setCentrePoints(new_centres);
            updateTrapMethod();
            emit trap_changed();
        }
    }
}

void Trap::on_triangles_check_changed(int state)
{
    if (!m_ignore_signals)
    {
        auto old_triangles = m_trap_config.getTriangles();
        auto new_triangles =(state == Qt::Checked);
        if (old_triangles != new_triangles)
        {
            m_trap_config.setTriangles(new_triangles);
            updateTrapMethod();
            emit trap_changed();
        }
    }
}

void  Trap::on_trap_centre_point_changed(int state)
{
    if (!m_ignore_signals)
    {
        auto old_enabled = m_trap_config.getCentrePoint();
        auto new_enabled = (state == Qt::Checked);
        if (old_enabled != new_enabled)
        {
            m_trap_config.setCentrePoint(new_enabled);
            updateTrapMethod();
            emit trap_changed();
        }
    }
}