// 
// File:   Splash.h
// Author: M.R.Egglesston
//
// Created on 19 March 2013, 08:16
//

#pragma once

#include <QEventLoop>
#include <QSplashScreen>

class Splash : QSplashScreen
{
    Q_OBJECT
public:
    Splash();
    virtual ~Splash();
    
    void display();
    
private slots:
    void on_timer();

private:
    Splash(const Splash& orig);
    
    void thread();

    QEventLoop* m_events;
    bool m_finished;
};


