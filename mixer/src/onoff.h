////////////////////////////////////////
//  File      : onoff.h               //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#ifndef ONOFF_H
#define ONOFF_H

#include "defs.h"
#include "extension.h"
#include "push.h"

///////////////////////////////////////

class Onoff : public QFrame
{
    Q_OBJECT

public:
    Onoff(Extension *, QWidget * parent = 0);
    ~Onoff();
    void init();

private slots:
    void set_state();

private:
    Extension *ext;
    Push *onoff;
    int state;
};
#endif





