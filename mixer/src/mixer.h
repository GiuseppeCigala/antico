////////////////////////////////////////
//  File      : mixer.cpp             //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#ifndef MIXER_H
#define MIXER_H

#include "defs.h"
#include "device.h"
#include "slider.h"
#include "group.h"
#include "peak.h"
#include "mute.h"
#include "onoff.h"
#include "enum.h"

////////////////////////////////////////

class Mixer : public QDialog
{
    Q_OBJECT
    
public:
    Mixer(QWidget * parent = 0);
    ~Mixer();
    void init();
    
private slots:
    
private:
    Device *dev;
    QGridLayout *main_layout;
    QGroupBox *master_group;
    QGroupBox *sections_group;
    QGroupBox *info_group;
    QHBoxLayout *master_group_layout;
    QVBoxLayout *sections_group_layout;
    QVBoxLayout *info_group_layout;
    QHash <int, QWidget *> control_list;
    QHash <int, Extension *> extension_list;
};


#endif
