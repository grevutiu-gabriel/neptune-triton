// 
// File:   Triton.h
// Author: M.R.Eggleston
//
// Created on 11 April 2014, 09:19
//

#pragma once

#include <iostream>
#include <QApplication>
#include <QMessageBox>


class Triton : public QApplication
{
    Q_OBJECT
public:
    Triton(int& argc, char** argv)
    :
    QApplication(argc, argv) {}
    ~Triton() {}
    
    bool notify(QObject* receiver, QEvent* event)
    {
        bool done = true;
        try
        {
            done = QApplication::notify(receiver, event);
        }
        catch (const std::bad_alloc&)
        {
            QMessageBox::warning(nullptr, "Triton Warning", "Can not enough allocate memory");
        }
        catch (const std::exception& ex)
        {
            QString error("Unexpected exception:\n\n");
            error += QString::fromStdString(ex.what());
            QMessageBox::critical(nullptr, "Triton Error", error);
        }
        catch (...)
        {
            QMessageBox::critical(nullptr, "Triton Error", "Unknown exception");
        }
        return done;
    }
};

