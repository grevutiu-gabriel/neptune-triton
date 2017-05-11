//
// File:   OptionComboboxes.cc
// Author: M.R.Eggleston
//
// Created on 24 May 2011, 14:56
//

#include <iostream>
#include "OptionComboboxes.h"

ColouringCombobox::ColouringCombobox(QWidget* parent)
: QComboBox(parent)
{
    addItem("Outer");
    addItem("Inner");
}

void ColouringCombobox::setValue(COLOURING value)
{
    setCurrentIndex(value == OUTER ? 0 : 1);
}

MethodCombobox::MethodCombobox(QWidget* parent)
: QComboBox(parent)
{
    addItem("Fixed Colour");
    addItem("Iteration");
    addItem("Final Magnitude");
    addItem("Magnitude");
    addItem("Change in Magnitude");
    addItem("Final Angle");
    addItem("Angle");
    addItem("Change in angle");
    addItem("Gaussian Integer");
    addItem("Trap Distance");
    addItem("Change in Trap Distance");
    addItem("Ratio");
    addItem("Change of Ratio");
}

METHOD MethodCombobox::getValue()
{
    return static_cast<METHOD>(currentIndex());
}

void MethodCombobox::setValue(METHOD value)
{
    setCurrentIndex(static_cast<int>(value));
}

GaussianTypeCombobox::GaussianTypeCombobox(QWidget* parent)
: QComboBox(parent)
{
    addItem("Round");
    addItem("Ceiling");
    addItem("Floor");
    addItem("Truncate");
}

GAUSSIAN_TYPE GaussianTypeCombobox::getValue()
{
    return static_cast<GAUSSIAN_TYPE>(currentIndex());
}

void GaussianTypeCombobox::setValue(GAUSSIAN_TYPE value)
{
    setCurrentIndex(static_cast<int>(value));
}

GaussianMethodCombobox::GaussianMethodCombobox(QWidget* parent)
: QComboBox(parent)
{
    addItem("Minimum Distance");
    addItem("Iteration @ Minimum Distance");
    addItem("Angle @ Minimum Distance");
    addItem("Maximum Distance");
    addItem("Iteration @ Maximum Distance");
    addItem("Angle @ Maximum Distance");
    addItem("Average Distance");
    addItem("Minimum Angle");
    addItem("Average Angle");
    addItem("Maximum Angle");
    addItem("Max. Distance/Min. Distance");
    addItem("Range");
    addItem("Variance");
    addItem("Standard Deviation");
    addItem("Coefficient of Variation");
}

GAUSSIAN_METHOD GaussianMethodCombobox::getValue()
{
    return static_cast<GAUSSIAN_METHOD>(currentIndex());
}

void GaussianMethodCombobox::setValue(GAUSSIAN_METHOD value)
{
    setCurrentIndex(static_cast<int>(value));
}

FinalMagnitudeTypeCombobox::FinalMagnitudeTypeCombobox(QWidget* parent)
: QComboBox(parent)
{
    addItem("norm(z)");
    addItem("smaller of real(z), imag(z)");
    addItem("greater of real(z), imag(z)");
    addItem("real(z)");
    addItem("abs(real(z)");
    addItem("imag(z)");
    addItem("abs(imag(z))");
    addItem("real(z)+imag(z)");
    addItem("abs(real(z)+imag(z))");
    addItem("real(z)*imag(z)");
    addItem("abs(real(z)*imag(z))");
}

FINAL_MAGNITUDE_TYPE FinalMagnitudeTypeCombobox::getValue()
{
    return static_cast<FINAL_MAGNITUDE_TYPE>(currentIndex());
}

void FinalMagnitudeTypeCombobox::setValue(FINAL_MAGNITUDE_TYPE value)
{
    setCurrentIndex(static_cast<int>(value));
}

StatisticsOptionTypeCombobox::StatisticsOptionTypeCombobox(QWidget* parent)
: QComboBox(parent)
{
    addItem("Minimum");
    addItem("Iteration @ Minimum");
    addItem("Maximum");
    addItem("Iteration @ Maximum");
    addItem("Range");
    addItem("Average");
    addItem("Variance");
    addItem("Standard Deviation");
    addItem("Coefficient of Variation");
    addItem("Fractal Dimension");
    addItem("Exponential Sum");
    addItem("Exponential Inverse Change Sum");
}

STATISTICS_OPTION_TYPE StatisticsOptionTypeCombobox::getValue()
{
    return static_cast<STATISTICS_OPTION_TYPE>(currentIndex());
}

void StatisticsOptionTypeCombobox::setValue(STATISTICS_OPTION_TYPE value)
{
    setCurrentIndex(static_cast<int>(value));
}

OrbitColourCombobox::OrbitColourCombobox(QWidget* parent)
: QComboBox(parent)
{
    addItem("Accumulation");
    addItem("Square Root");
    addItem("Average");
    addItem("Logarithm");
}

OrbitTrapTypeCombobox::OrbitTrapTypeCombobox(QWidget* parent)
: QComboBox(parent)
{
    addItem("Point");
    addItem("4 Points");
    addItem("Line");
    addItem("Cross");
    addItem("Square");
    addItem("Circle");
    addItem("Triangle");
    addItem("Triform");
    addItem("Asterisk");
    addItem("Circle Line");
    addItem("Circle Cross");
    addItem("Circle Triform");
    addItem("Two Quarter Circles");
    addItem("Circle Triangle");
    addItem("Triangle Circle");
    addItem("Circle Square");
    addItem("Square Circle");
    addItem("Octothorpe");
    addItem("Running Track");
    addItem("Pinch");
    addItem("Steiner Chain");
    m_values.push_back(OrbitTrap::POINT);
    m_values.push_back(OrbitTrap::FOUR_POINTS);
    m_values.push_back(OrbitTrap::LINE);
    m_values.push_back(OrbitTrap::CROSS);
    m_values.push_back(OrbitTrap::SQUARE);
    m_values.push_back(OrbitTrap::CIRCLE);
    m_values.push_back(OrbitTrap::TRIANGLE);
    m_values.push_back(OrbitTrap::TRIFORM);
    m_values.push_back(OrbitTrap::ASTERISK);
    m_values.push_back(OrbitTrap::CIRCLE_LINE);
    m_values.push_back(OrbitTrap::CIRCLE_CROSS);
    m_values.push_back(OrbitTrap::CIRCLE_TRIFORM);
    m_values.push_back(OrbitTrap::QUARTER_CIRCLES);
    m_values.push_back(OrbitTrap::CIRCLE_TRIANGLE);
    m_values.push_back(OrbitTrap::TRIANGLE_CIRCLE);
    m_values.push_back(OrbitTrap::CIRCLE_SQUARE);
    m_values.push_back(OrbitTrap::SQUARE_CIRCLE);
    m_values.push_back(OrbitTrap::OCTOTHORPE);
    m_values.push_back(OrbitTrap::RUNNING_TRACK);
    m_values.push_back(OrbitTrap::PINCH);
    m_values.push_back(OrbitTrap::STEINER_CHAIN);
}

OrbitTrap::TRAP_TYPE OrbitTrapTypeCombobox::getValue()
{
    return m_values[currentIndex()];
}

void OrbitTrapTypeCombobox::setValue(OrbitTrap::TRAP_TYPE value)
{
    bool found = false;
    int ivalue = 0;
    for (auto it = m_values.begin(); it != m_values.end(); it++)
    {
        if (*it == value)
        {
            found = true;
            break;
        }
        ivalue++;
    };
    setCurrentIndex(found ? ivalue : 0);
}

RatioPartTypeCombobox::RatioPartTypeCombobox(QWidget* parent)
: QComboBox(parent)
{
    addItem("Smaller of real(z), imag(z)");
    addItem("Larger of real(z), imag(z)");
    addItem("abs(real(z))");
    addItem("abs(imag(z))");
    addItem("abs(real(z) + imag(z))");
    addItem("abs(real(z) - imag(z))");
    addItem("abs(real(z)) + abs(imag(z))");
    addItem("abs(abs(real(z)) - abs(imag(z)))");
    addItem("abs(real(z)*imag(z))");
    addItem("abs(z)");
    addItem("norm(z)");
}

RATIO_PART_TYPE RatioPartTypeCombobox::getValue()
{
    return static_cast<RATIO_PART_TYPE>(currentIndex());
}

void RatioPartTypeCombobox::setValue(RATIO_PART_TYPE value)
{
    setCurrentIndex(static_cast<int>(value));
}


DivergeBailoutConditionCombobox::DivergeBailoutConditionCombobox(QWidget* parent)
: QComboBox(parent)
{
    addItem("abs(z)");
    addItem("norm(z)");
    addItem("imag(z)");
    addItem("imag(z) squared");
    addItem("real(z)");
    addItem("real(z) squared");
    addItem("abs(real(z)*imag(z))");
    addItem("abs(real(z)) + abs(imag(z))");
    addItem("real(z) + imag(z)");
    addItem("abs(real(z) + image(z))");
    addItem("max(real(z), imag(z)) squared");
    addItem("min(real(z), imag(z)) squared");
    addItem("abs(real(z) - imag(z))");
}

BAILOUT DivergeBailoutConditionCombobox::getValue()
{
    return static_cast<BAILOUT>(currentIndex()+1);
}

void DivergeBailoutConditionCombobox::setValue(BAILOUT value)
{
    setCurrentIndex(static_cast<int>(value)-1);
}

OrbitPlotCombobox::OrbitPlotCombobox(QWidget* parent)
: QComboBox(parent)
{
    addItem("All");
    addItem("Escaped");
    addItem("Captive");
}

ORBIT_PLOTS OrbitPlotCombobox::getValue()
{
    return static_cast<ORBIT_PLOTS>(currentIndex());
}

void OrbitPlotCombobox::setValue(ORBIT_PLOTS value)
{
    setCurrentIndex(static_cast<int>(value));
}

ExportCombobox::ExportCombobox(QWidget* parent)
: QComboBox(parent)
{
    addItem(".scm");
    addItem(".ugr");
}

EXPORT_FILES ExportCombobox::getValue()
{
    return static_cast<EXPORT_FILES>(currentIndex());
}

void ExportCombobox::setValue(EXPORT_FILES value)
{
    setCurrentIndex(static_cast<int>(value));
}

NavigationCombobox::NavigationCombobox(QWidget* parent)
: QComboBox(parent)
{
    addItem("Quick Zoom");
    addItem("Zoom In");
    addItem("Centre In");
    addItem("Centre Out");
    addItem("Zoom Off");
}
    
NAVIGATION NavigationCombobox::getValue()
{
    return static_cast<NAVIGATION>(currentIndex());
}

void NavigationCombobox::setValue(NAVIGATION value)
{
    setCurrentIndex(static_cast<int>(value));
}

ColourMapEditComboBox::ColourMapEditComboBox(QWidget* parent)
: QComboBox(parent)
{
    addItem("Auto");
    addItem("Manual");
    addItem("Component");
}

COLOUR_MAP_EDIT ColourMapEditComboBox::getValue()
{
    return static_cast<COLOUR_MAP_EDIT>(currentIndex());
}

void ColourMapEditComboBox::setValue(COLOUR_MAP_EDIT value)
{
    setCurrentIndex(static_cast<int>(value));
}

WaveComboBox::WaveComboBox(QWidget* parent)
: QComboBox(parent)
{
    addItem("Sine");
    addItem("Saw");
    addItem("Parabolic");
}

Waveform::WAVE WaveComboBox::getValue()
{
    return static_cast<Waveform::WAVE>(currentIndex());
}

void WaveComboBox::setValue(Waveform::WAVE value)
{
    setCurrentIndex(static_cast<int>(value));
}

ComponentOrderComboBox::ComponentOrderComboBox(QWidget* parent)
: QComboBox(parent)
{
    addItem("RGB");
    addItem("RBG");
    addItem("GRB");
    addItem("GBR");
    addItem("BRG");
    addItem("BGR");
}

COMPONENT_ORDER ComponentOrderComboBox::getValue()
{
    return static_cast<COMPONENT_ORDER>(currentIndex());
}

void ComponentOrderComboBox::setValue(COMPONENT_ORDER value)
{
    setCurrentIndex(static_cast<int>(value));
}

InnerTypeComboBox::InnerTypeComboBox(QWidget* parent)
: QComboBox(parent)
{
    addItem("Average inner/outer");
    addItem("Inner overlap only");
    addItem("Outer overlap only");
}

INNER_TYPE InnerTypeComboBox::getValue()
{
    return static_cast<INNER_TYPE>(currentIndex());
}

void InnerTypeComboBox::setValue(INNER_TYPE value)
{
    setCurrentIndex(static_cast<int>(value));
}

CompoundImageComboBox::CompoundImageComboBox(QWidget* parent)
: QComboBox(parent)
{
    addItem("Average outer values");
    addItem("Use highest iteration outer values");
}

COMPOUND_IMAGE CompoundImageComboBox::getValue()
{
    return static_cast<COMPOUND_IMAGE>(currentIndex());
}

void CompoundImageComboBox::setValue(COMPOUND_IMAGE value)
{
    setCurrentIndex(static_cast<int>(value));
}
