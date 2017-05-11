//
// File:   Fractal.h
// Author: M.R.Eggleston
//
// Created on 29 January 2014
//

#pragma once

#ifdef NOT_MOC
#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#endif
#include <QStatusBar>
#include "mpreal.h"
#include "CalculatingUnitLD.h"
#include "CalculatingUnitMP.h"
#include "ColourMaps.h"
#include "Colours.h"
#include "FractalsConfig.h"
#include "FractalDrawing.h"
#include "TypesAndConstants.h"

class Fractal : public QObject
{
    Q_OBJECT
   
signals:
    void signal_row_by_row_progress();
    
private slots:
    void on_progress_signal(int row);

public:
    Fractal(QLabel* status);
    virtual ~Fractal();
    void Centre(const QPoint& press);
    FRACTAL_TYPE GetFractalType();
    void Reset();
    void SetFractalType(FRACTAL_TYPE ft);
    void Start();
    void Stop();
    // px, p_y -> pres co-ordinates, r_x, r_y -> release co-ordinates
    void ZoomIn(const QPoint& press, const QPoint& release);
    void ZoomIn(const QPoint& centre);
    void ZoomIn(const QPoint& centre, int width);
    void ZoomOut(const QPoint& press);
    
    void changeColours();
    void changeIterations(int iterations);
    void status();
    void update_progress();
    
private:    
    Fractal();
    void bailoutStatus();
   
    void waitForThreadsToFinish();
    void waitForOrbitThreadsToFinish();
    void signalStatus();
    
    void QuickThread_control();
    void QuickThread(int thread_number);
    void OrbitThread_control();
    void OrbitThread(int thread_number);
  
    bool restartRequired();

    QLabel* m_status;
    FractalsConfig m_config;

    boost::thread m_thread;
    int m_iteration;
    int m_max_iterations;
    int m_iterate_step_size;
    bool m_exitThread;

    bool m_finished;

    QString m_duration;

    // Multi-threading
    int m_number_of_threads;
    int m_number_of_orbit_threads;
    boost::scoped_array<boost::thread> m_work_threads;
    boost::mutex m_threads_mutex;
    boost::condition m_threads_condition;
    bool m_multi_precision;
    int m_precision;
    bool m_ignore_progress;
    std::string m_colouring_method[2];
    unsigned int m_row;
    unsigned int m_rows;
    boost::scoped_array<CalculatingUnitLD> m_unit;
    boost::scoped_array<CalculatingUnitMP> m_mp_unit;
    bool m_settled;
    unsigned int m_settling_cycle;
    unsigned int m_calculating_cycle;
    unsigned int m_settling_cycles;
    unsigned int m_calculating_cycles;
    bool m_orbit_plotting;
    int m_number_of_active_threads;
    std::string m_colour_signature;
    INNER_TYPE m_inner_type;
    COMPOUND_IMAGE m_compound_image;
};

inline void Fractal::signalStatus()
{
    if (!StaticCalculatingUnit::s_stop)
    {
        emit signal_row_by_row_progress();
    }
}
