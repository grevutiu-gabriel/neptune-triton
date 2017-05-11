//
// File:   Picture.h
// Author: M.R.Eggleston
//
// Created on 22 July 2010, 13:17
//

#pragma once

#ifdef NOT_MOC
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#endif
#include <vector>
#include <QObject>
#include "mpreal.h"
#include "CalculatingUnitLD.h"
#include "CalculatingUnitMP.h"
#include "Colours.h"
#include "FractalConfig.h"
#include "TypesAndConstants.h"
#include "OrbitTraps.h"

using boost::posix_time::microsec_clock;
using boost::posix_time::ptime;

class Picture : public QObject
{
    Q_OBJECT

public:
    enum STATE
    {
        PIC_IDLE,
        PIC_EXPANDING,
        PIC_EXPANDED
    };

signals:
    void signal_progress(double);
    void signal_state();
    void signal_final_status();

private slots:
    void on_progress_signal(int pos);

public:
    Picture();
    virtual ~Picture();


    enum OUTER_INNER
    {
        OUTER,
        INNER
    };

    void setConfig(const FractalConfig& f);

    void setWidthInPixels(double width);
    void setOutputFile(const QString& file);

    void abort();
    void expand();
    bool save();

    bool isIdle();
    bool isExpanding();
    bool isFinished();

    QString imageSize();
    QString newImageSize();
    QString precision();
    QString newPrecision();
    unsigned int width();
    unsigned int height();
    const QString &final_status();
    STATE state();

    //
    FractalConfig &getFractalConfig();
    
    boost::shared_ptr<OrbitTrap> getOrbitTrap(bool inner);

private:
    Picture(const Picture& orig);

    void escapeThread_control();
    void orbitThread(int thread_number);
    void multiPrecisionThread(int thread_number);
    void orbitThread_control();

    bool assignFormula();
    void bailoutThread(int thread_number);

    void set_orbit_formula_variables(int i, int j, LongComplex &z, LongComplex &c);
    
    void display_final_status();

    // Work set methods
    void allocateWorkSet();

    void setOrbitColoursThread();
    
    void waitForOrbitThreadsToFinish();
    void waitForThreadsToFinish();
    
    int saturnLevel();
    
    void loadFunctions();
    void loadComplexFunctions();
    
    FractalConfig m_config;

    unsigned int m_width;
    unsigned int m_height;
    int m_new_width;
    int m_new_height;
    int m_calc_height;
    bool m_is_orbit_plotting;
    int m_max_iterations;
    long double m_factor;
    QString m_outfile;
    //
    QRgb m_fixed_colour[2];
    //
    // Data for dynamically allocated working data.
    //
    int m_size;
    //
    unsigned int m_interval;
    unsigned int m_colour;

    //
    // Orbit Traps
    //
    boost::shared_ptr<OrbitTrap> m_trap[2];

    ptime m_start_time;
    ptime m_end_time;
    long double m_calc_rmin;
    long double m_calc_imax;
    ORBIT_PLOTS m_plot;
    long double m_orbit_scale;
    int m_progress;
    
    // Multi-threading
    unsigned int m_number_of_orbit_threads;
    unsigned int m_number_of_threads;
    boost::thread m_control_thread;
    
    boost::scoped_array<boost::thread> m_work_threads;
    boost::scoped_array<CalculatingUnitLD> m_unit;
    boost::scoped_array<CalculatingUnitMP> m_mp_unit;
    boost::mutex m_data_mutex;
    boost::mutex m_threads_mutex;
    boost::mutex m_progress_mutex;
    bool m_exit_threads;
    unsigned int m_omit;
    bool m_multi_precision;
    QString m_final_status;
    STATE m_state;
};

