////////////////////////////////////////
//  File      : device.h              //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#ifndef DEVICE_H
#define DEVICE_H

#include "defs.h"
#include "extension.h"

////////////////////////////////////////

class Device : public QWidget
{
    Q_OBJECT
    
public:
    Device(QWidget * parent = 0);
    ~Device();
    void init();
    int get_mixer_fd();
    int get_mixer_dev(int);
    void create_extensions(int, int);
    QHash <int, Extension *> get_extensions();
    QMap <QString, QString> get_system_info();
   
private slots:

private:
    QHash <int, Extension *> ext_list;
    QMap <QString, QString> system_info;

};
#endif




