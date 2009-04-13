////////////////////////////////////////
//  File      : info.h                //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#ifndef INFO_H
#define INFO_H

#include "defs.h"
#include "device.h"

///////////////////////////////////////

class Info : public QGroupBox
{
    Q_OBJECT

public:
    Info(Device *, QWidget * parent = 0);
    ~Info();
    void init();

private slots:

private:
    Device *dev;
};
#endif


