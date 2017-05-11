//
// File:   CalculatingUnitLD.h
// Author: M.R.Eggleston
//
// Wrapper for a template class required because Qt doesn't support templates.
//
// Created 29th January 2014, 15:39

#pragma once

#include "FractalConfig.h"

class CalculatingUnitGeneric : public QObject
{
    Q_OBJECT
signals:
    void signal_progress(int);
    void signal_so_progress(long, long);

public:
    CalculatingUnitGeneric() {};
    virtual ~CalculatingUnitGeneric() {};

    virtual void setFractal() = 0;

    virtual void setValues(int iteration, int chunk_size) = 0;
    virtual bool calculate() = 0;
    virtual void calculateOrbits() = 0;
    virtual void enableProgressSignals(bool value) = 0;
    virtual void sendProgressSignals(bool value) = 0;

    void send_signal_progress(int row)
    {
        emit signal_progress(row);
    }
    
    void send_signal_so_progress(long plot, long no_plot)
    {
        emit signal_so_progress(plot, no_plot);
    }

protected:
    CalculatingUnitGeneric(const CalculatingUnitGeneric& orig);

};
