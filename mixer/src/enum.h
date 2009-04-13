////////////////////////////////////////
//  File      : enum.h                //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#ifndef ENUM_H
#define ENUM_H

#include "defs.h"
#include "extension.h"

///////////////////////////////////////

class Enum : public QFrame
{
    Q_OBJECT

public:
    Enum(Extension *, QWidget * parent = 0);
    ~Enum();
    void init();
    void read_entry();

private slots:
    void change_entry(int);

private:
    Extension *ext;
    QComboBox *combo;

};
#endif


