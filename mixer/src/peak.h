////////////////////////////////////////
//  File      : peak.h                //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#ifndef PEAK_H
#define PEAK_H

#include "defs.h"
#include "extension.h"
#include "vumeter.h"

///////////////////////////////////////

class Peak : public QFrame
{
    Q_OBJECT

public:
    Peak(Extension *, QString type, QWidget * parent = 0);
    ~Peak();
    void init();

private slots:
    void mono_timeout();
    void stereo_timeout();
 
private:
    Extension *ext;
    QString type;
    QTimer *timer;
    VUMeter *mono_vu;
    VUMeter *left_vu;
    VUMeter *right_vu;

};
#endif


