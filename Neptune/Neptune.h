// 
// File:   Neptune.h
// Author: M.R.Eggleston
//
// Created on 29 January 2014, 14:02
//

#pragma once

#include <QApplication>
#include <QMessageBox>
#include "Config.h"

class Neptune : public QApplication
{
    Q_OBJECT
public:
    Neptune(int & argc, char ** argv)
    :
    QApplication(argc, argv)
    {
        connect(this, SIGNAL(aboutToQuit()), this, SLOT(on_quit()));
    }
    ~Neptune(){}

    bool notify(QObject* receiver, QEvent* event)
    {
        bool done = true;
        try
        {
            done = QApplication::notify(receiver, event);
        }
        catch (const std::bad_alloc&)
        {
            QMessageBox::warning(nullptr, "Neptune Warning", "Can not enough allocate memory");
        }
        catch (const std::exception& ex)
        {
            QString error("Unexpected exception:\n\n");
            error += QString::fromStdString(ex.what());
            QMessageBox::critical(nullptr, "Neptune Error", error);
        }
        catch (...)
        {
            QMessageBox::critical(nullptr, "Neptune Error", "Unknown exception");
        }
        return done;
    }

public slots:
    void on_quit() { Config::instance()->save(); }
};


