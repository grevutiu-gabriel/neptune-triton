// 
// File:   Splash.cc
// Author: M.R.Eggleston
// 
// Created on 19 March 2013, 08:16
///

#include <boost/thread.hpp>
#include <QTimer>
#include "ColourMaps.h"
#include "Config.h"
#include "Splash.h"

Splash::Splash()
:
QSplashScreen(),
m_events(new QEventLoop()),
m_finished(false)
{
}

Splash::~Splash()
{
}

void Splash::display()
{
    // Find splash picture, exit with false if not found
    bool found = true;
    if (found)
    {
        QPixmap pixmap(":/images/splash.png");
        setPixmap(pixmap);
    }
    else
    {
        setVisible(false);
    }
    show();
    auto id = boost::thread(boost::bind(&Splash::thread, this));
    QTimer::singleShot(1000, this, SLOT(on_timer()));
    m_events->exec();
}

void Splash::thread()
{
    boost::shared_ptr<ColourMaps> maps = ColourMaps::create();
    boost::shared_ptr<Config> config = Config::create();
    m_finished = true;
}

void Splash::on_timer()
{
    if (m_finished)
    {
        m_events->quit();
    }
    else
    {
        QTimer::singleShot(1000, this, SLOT(on_timer()));
    }
}