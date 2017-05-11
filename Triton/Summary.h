//
// File:   Summary.h
// Author: M.R.Eggleston
//
// Created on 21 September 2011, 13:36
//

#pragma once

#include <QString>
#include "FractalConfig.h"
#include "Picture.h"

class Summary
{
public:
    static boost::shared_ptr<Summary> create();
    static boost::shared_ptr<Summary> instance();
    
    const QString &get(Picture* picture);

    virtual ~Summary();
private:
    Summary();
    Summary(const Summary& orig);

    const QString &bailoutSummary();
    void parameterSummary();
    void criticalPointSummary();

    void loadFormulae();
    void loadColourMethods();
    void loadBailoutMaps();
    void orbitPlot();

    QString getFormula(FRACTAL_TYPE ft);
    void parameter(const QString& name, int id);
    QString bailout(BAILOUT type, long double limit);
    QString colourSelection();
    QString outerAreas();
    QString overlap();
    QString colouringMethod(bool inner);
    QString methodName(char value);
    QString finalMagnitudeOptionName(char value);
    QString statistics(char value);
    QString gaussianType(char value);
    QString ratio(char q, char d);
    QString gaussianStatistic(char value);
    QString orbitTrap(bool inner);
    QString trapName(char value);

    typedef std::map<FRACTAL_TYPE, QString> FormulaMap;
    typedef std::map<unsigned int, QString> UintStringMap;
    typedef std::map<BAILOUT, QString> BailoutMap;
    typedef std::map<char, QString> CharStrMap;

    static boost::shared_ptr<Summary> s_instance;

    FractalConfig m_config;
    FormulaMap m_formulae;
    BailoutMap m_lt_bailout;
    BailoutMap m_gt_bailout;
    CharStrMap m_colour_method;
    CharStrMap m_gaussian_type;
    CharStrMap m_statistics_type;
    CharStrMap m_final_magnitude_option;
    CharStrMap m_ratio_part;
    CharStrMap m_gaussian_statistic;
    CharStrMap m_trap_name;
    Picture* m_picture;

    QString m_str;
};

