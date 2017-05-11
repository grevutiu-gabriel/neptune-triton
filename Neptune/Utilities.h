// 
// File:   Utilities.h
// Author: M.R.Eggleston
//
// Created on 29 January 2014, 14:10
//

#pragma once

#include "ldcomplex.h"
#include "mpcomplex.h"
#include "ColourMap.h"
#include "TypesAndConstants.h"

std::string intToString(int value);
std::string uintToString(unsigned int value);
std::string ulongToString(unsigned long value);
std::string longdoubleToString(long double value);
QString longdoubleToQString(long double value);
std::string longdoubleToString(long double value, bool fixed, int precision);
StringComplex longComplexToStringComplex(const LongComplex& value);
QString longComplexToString(const LongComplex& value);
QString complexNumberToString(const StringComplex& z);
QString fractalTypeToString(FRACTAL_TYPE ft);

QColor stringToColour(const std::string& str);
unsigned int stringToUint(const std::string& str);
int stringToInt(const std::string& str);
long double stringToLongDouble(const std::string& str);
LongComplex stdStringToLongComplex(const std::string& s1, const std::string& s2);
LongComplex stringComplexToLongComplex(const StringComplex& z);
MpComplex stringComplexToMpComplex(const StringComplex& z, int precision);

long double trunc(long double value); // C++11 std::trunc not available in MS C++ yet...

std::string abbreviate(const std::string& s);

int determinePrecision(const StringComplex& x, const std::string& width, int hpixels, int vpixels);

QString addExtension(const QString& path, const QString& extension);
QString name_only(const QString& name);

// Qt doesn't successfully load PNG metadata on OS X and Windows, the following
// utilities are required to extract the data.

// Conversion from network format 4 byte integer into host format used
// instead of ntohl to avoid header location problems
int convertFromNetworkFormat32(unsigned char *data);

void quadratic(const LongComplex& a,
               const LongComplex& b,
               const LongComplex& c,
               VectorLongComplex& roots);
void roots(const LongComplex& z,
           int power,
           VectorLongComplex& roots);
void polyroots(const VectorLongComplex& coefficients,
               VectorLongComplex& roots);
bool isnan(long double);