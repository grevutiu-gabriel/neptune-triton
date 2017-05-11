//
// File:   Picture.cc
// Author: M.R.Eggleston
//
// Created on 22 July 2010, 13:17
//


#include <iomanip>
#include <iostream>
#include <boost/thread.hpp>
#include <QMessageBox>
#include "ColourMap.h"
#include "KeyString.h"
#include "Picture.h"
#include "Utilities.h"

Picture::Picture()
:
QObject(),
m_width(0),
m_height(0),
m_new_width(0),
m_new_height(0),
m_max_iterations(0),
m_factor(1.0L),
m_size(0),
m_interval(0),
m_colour(0),
m_calc_rmin(0.0L),
m_calc_imax(0.0L),
#if DEBUG
m_number_of_threads(1),
#else
m_number_of_threads(boost::thread::hardware_concurrency() == 0 ? 1 : boost::thread::hardware_concurrency()),
#endif
m_work_threads(new boost::thread[m_number_of_threads]),
m_unit(new CalculatingUnitLD[m_number_of_threads]),
m_mp_unit(new CalculatingUnitMP[m_number_of_threads]),
m_exit_threads(false),
m_omit(0),
m_multi_precision(false),
m_state(PIC_IDLE)
{
    m_unit[0].enableProgressSignals(true);
    connect(&m_unit[0], SIGNAL(signal_progress(int)), this, SLOT(on_progress_signal(int)));
    m_mp_unit[0].enableProgressSignals(true);
    connect(&m_mp_unit[0], SIGNAL(signal_progress(int)), this, SLOT(on_progress_signal(int)));
}

Picture::~Picture()
{
}

unsigned int Picture::width()
{
    return m_width;
}

unsigned int Picture::height()
{
    return m_height;
}

void Picture::expand()
{
    m_exit_threads = false;
    StaticCalculatingUnit::s_stop = false;
    StaticCalculatingUnit::s_max_iteration = 0;
    m_config.setWidth(m_new_width);
    m_config.setHeight(m_new_height);
    StaticCalculatingUnit::setFractal(m_config);
    if (StaticCalculatingUnit::s_image.isNull())
    {
        QMessageBox::warning(nullptr, "Triton Warning", "Can not allocate enough memory");
    }
    else
    {
        int bits = m_config.getPrecision();
        if (bits == 0)
        {
            bits = determinePrecision(m_config.getImageCentre(), m_config.getImageWidth(), m_new_width, m_new_height);
        }
        m_multi_precision = (bits > BASE_PRECISION);
        m_max_iterations = m_config.getMaxIterations();
        m_state = PIC_EXPANDING;
        emit signal_state();
        emit signal_progress(0.0);
        m_is_orbit_plotting = m_config.isOrbitPlotting();
        if (m_is_orbit_plotting)
        {
            m_control_thread = boost::thread(boost::bind(&Picture::orbitThread_control, this));
        }
        else
        {
            m_control_thread = boost::thread(boost::bind(&Picture::escapeThread_control, this));
        }
    }
}

void Picture::escapeThread_control()
{
    // It is possible for the thread to start and access the thread control
    // the thread id has been placed in the array resulting in an incorrect
    // thread number. FindThreadNumber now looks for an empty slot, sets the
    // id and returns the slot number. So the thread id returned by create
    // can be thrown away.
//    m_config.setWidth(m_new_width);
//    m_config.setHeight(m_new_height);
//    StaticCalculatingUnit::setFractal(m_config, true);
    m_new_width = static_cast<unsigned int>(static_cast<long double>((m_width)*m_factor + 0.5));
    m_new_height = static_cast<unsigned int>(static_cast<long double>((m_height)*m_factor + 0.5));
    allocateWorkSet();
    m_start_time = microsec_clock::local_time();
    m_progress = 0;
    if (m_multi_precision)
    {
        for (unsigned int i = 0; i < m_number_of_threads; i++)
        {
            m_mp_unit[i].setFractal();
        }
        for (unsigned int j = 0; j < m_number_of_threads; j++)
        {
            m_work_threads[j] = boost::thread(&Picture::multiPrecisionThread, this, j);
        }
        waitForThreadsToFinish();
    }
    else
    {
        for (unsigned int i = 0; i < m_number_of_threads; i++)
        {
            m_unit[i].setFractal();
        }
        for (unsigned int j = 0; j < m_number_of_threads; j++)
        {
            m_work_threads[j] = boost::thread(&Picture::bailoutThread, this, j);
        }
        waitForThreadsToFinish();
    }
    if (!m_exit_threads)
    {
        m_end_time = microsec_clock::local_time();
        display_final_status();
        m_state = PIC_EXPANDED;
    }
    else
    {
        m_state = PIC_IDLE;
    }
    emit signal_final_status();
}

void Picture::orbitThread_control()
{
    m_start_time = microsec_clock::local_time();
    m_config.setWidth(m_new_width);
    m_config.setHeight(m_new_height);
    StaticCalculatingUnit::setFractal(m_config);
    m_number_of_orbit_threads = (StaticCalculatingUnit::s_colouring_type == OC_AVERAGE ? 1 : m_number_of_threads);
    for (unsigned int i = 0; i < m_number_of_orbit_threads; i++)
    {
        m_unit[i].setFractal();
    }
    m_progress = 0;
    long double area = m_config.getArea();
    long double density = static_cast<long double>(m_config.getDensity());
    long double adjustment = std::sqrt(area)*std::sqrt(density);
    
    m_omit = m_config.getStartOrbitIteration();

    m_colour = 0;
    //
    // To produce decent zoomed images the calculation area is larger than
    // the display area this is specified by the m_area value which is
    // ratio of the calculation area to the display area, the ratio of
    // the dimensions is the square root. The size of the calculating area
    // by Saturn and is in the range 1 to 100.
    //
    long double base_interval = static_cast<long double>(m_config.getInterval());
    int interval = static_cast<unsigned int>(base_interval*m_factor*m_factor*area);
    StaticCalculatingUnit::s_colour_change_interval = interval;
    m_calc_height = static_cast<int>(static_cast<long double>(m_new_height)*adjustment);
    allocateWorkSet();

    // It is possible for the thread to start and access the thread control
    // the thread id has been placed in the array resulting in an incorrect
    // thread number. FindThreadNumber now looks for an empty slot, sets the
    // id and returns the slot number. So the thread id returned by create
    // can be thrown away.

    for (unsigned int i = 0; i < m_number_of_orbit_threads; i++)
    {
        m_work_threads[i] = boost::thread(&Picture::orbitThread, this, i);
    }
    waitForOrbitThreadsToFinish();
    if (!m_exit_threads)
    {
        StaticCalculatingUnit::updateOrbitImage();
    }
    if (!m_exit_threads)
    {
        m_end_time = microsec_clock::local_time();
        display_final_status();
        m_state = PIC_EXPANDED;
    }
    else
    {
        m_state = PIC_IDLE;
    }
    emit signal_final_status();
}

void Picture::waitForOrbitThreadsToFinish()
{
    for (unsigned int i = 0; i < m_number_of_orbit_threads; i++)
    {
        m_work_threads[i].join();
    }
}

void Picture::orbitThread(int thread_number)
{
    m_unit[thread_number].setValues(0, m_max_iterations);
    m_unit[thread_number].calculateOrbits();
}

bool Picture::save()
{
    return StaticCalculatingUnit::s_image.save(m_outfile, "PNG");
}

void Picture::bailoutThread(int thread_number)
{
    m_unit[thread_number].setValues(0, m_max_iterations);
    m_unit[thread_number].calculate();
}

void Picture::multiPrecisionThread(int thread_number)
{
    m_mp_unit[thread_number].setValues(0, m_max_iterations);
    m_mp_unit[thread_number].calculate();
}

void Picture::setWidthInPixels(double width)
{
    m_factor = (m_width > 0 ? width/static_cast<double>(m_width) : 1.0);
    m_new_width = static_cast<unsigned int>(static_cast<double>((m_width)*m_factor + 0.5));
    m_new_height = static_cast<unsigned int>(static_cast<double>((m_height)*m_factor + 0.5));
}

void Picture::setOutputFile(const QString& file)
{
    m_outfile = file;
}

void Picture::abort()
{
    m_exit_threads = true;
    StaticCalculatingUnit::stop();
    if (m_control_thread.joinable())
    {
        m_control_thread.join();
    }
    m_state = PIC_IDLE;
    emit signal_final_status();
}


void Picture::display_final_status()
{
    m_final_status = "100.00% [" + QString::fromStdString(to_simple_string(m_end_time - m_start_time)) + "] ";
    bool orbit_plotting = m_config.isOrbitPlotting();
    if (!orbit_plotting)
    {
        m_final_status += QString::number(StaticCalculatingUnit::s_max_iteration) + " iterations";
    }
    emit signal_final_status();
}

QString Picture::imageSize()
{
    std::string str = uintToString(m_width) + "x" + uintToString(m_height);
    return QString(str.c_str());
}

QString Picture::newImageSize()
{
    std::string str = uintToString(m_new_width) + "x" + uintToString(m_new_height);
    return QString(str.c_str());
}

QString Picture::precision()
{
    int precision = m_config.getPrecision();
    if (precision == 0)
    {
        auto c = m_config.getImageCentre();
        std::string w = m_config.getImageWidth();
        precision = determinePrecision(c, w, m_width, m_height);
    }
    std::string str = intToString(precision) + " bits";
    return QString::fromStdString(str);
}

QString Picture::newPrecision()
{
    int precision = m_config.getPrecision();
    if (precision == 0)
    {
        auto c = m_config.getImageCentre();
        std::string w = m_config.getImageWidth();
        precision = determinePrecision(c, w, m_new_width, m_new_height);
    }
    std::string str = intToString(precision) + " bits";
    return QString::fromStdString(str);
}

void Picture::allocateWorkSet()
{
    // is this really required ?
    m_size = m_new_width*m_new_height;
}

FractalConfig &Picture::getFractalConfig()
{
    return m_config;
}

boost::shared_ptr<OrbitTrap> Picture::getOrbitTrap(bool inner)
{
    return m_trap[static_cast<bool>(inner)];
}

void Picture::setOrbitColoursThread()
{
    StaticCalculatingUnit::updateOrbitImage();
}

void Picture::waitForThreadsToFinish()
{
    for (unsigned int i = 0; i < m_number_of_threads; i++)
    {
        m_work_threads[i].join();
    }
}

void Picture::on_progress_signal(int row)
{
    double fraction;
    if (m_is_orbit_plotting)
    {
        fraction = static_cast<double>(row + 1)/static_cast<double>(m_calc_height);
    }
    else
    {
        fraction = static_cast<double>(row + 1)/static_cast<double>(m_new_height);
    }
    emit signal_progress(fraction);
    if (m_multi_precision)
    {
        m_mp_unit[0].sendProgressSignals(true);
    }
    else
    {
        m_unit[0].sendProgressSignals(true);
    }
}

void Picture::setConfig(const FractalConfig& c)
{
    m_config = c;
    m_width = m_config.widthInPixels();
    m_height = m_config.heightInPixels();
}

const QString &Picture::final_status()
{
    return m_final_status;
}

Picture::STATE Picture::state()
{
    return m_state;
}
