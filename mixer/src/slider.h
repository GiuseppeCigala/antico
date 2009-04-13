////////////////////////////////////////
//  File      : slider.h              //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#ifndef SLIDER_H
#define SLIDER_H

#include "defs.h"
#include "extension.h"
#include "cursor.h"

///////////////////////////////////////

class Slider : public QFrame
{
    Q_OBJECT

public:
    Slider(Extension *, QString type, QWidget * parent = 0);
    ~Slider();
    void init();


private slots:
    void set_mono_volume(int);
    void set_left_volume(int);
    void set_right_volume(int);

private:
    Extension *ext;
    QString type;

};
#endif





