////////////////////////////////////////
//  File      : mute.h                //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#ifndef MUTE_H
#define MUTE_H

#include "defs.h"
#include "extension.h"
#include "push.h"

///////////////////////////////////////

class Mute : public QFrame
{
    Q_OBJECT

public:
    Mute(Extension *, QWidget * parent = 0);
    ~Mute();
    void init();

private slots:
    void set_state();

private:
    Extension *ext;
    Push *mute;
    int state;
    int act_volume;

};
#endif





