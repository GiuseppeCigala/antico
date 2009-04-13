////////////////////////////////////////
//  File      : extension.h           //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#ifndef EXTENSION_H
#define EXTENSION_H

#include "defs.h"

///////////////////////////////////////

class Extension : public QWidget
{
    Q_OBJECT

public:
    Extension(int, int, oss_mixext, QWidget * parent = 0);
    ~Extension();
    int get_min_value();
    int get_max_value();
    int get_act_value();
    int get_mono_value();
    int get_left_value();
    int get_right_value();
    int get_ctrl();
    int get_parent();
    QStringList get_flags();
    QStringList get_entry_list(); 
    QString get_id();
    QString get_extname();
    QString get_type();
    QList <int> get_childs();
    void set_shift(int);
    void set_value_mask(int);
    void set_act_value(int);
    void set_mono_value(int);
    void set_left_value(int);
    void set_right_value(int);
    void set_type(QString);
    void set_child(int);
    void check_type();

private slots:

private:
    oss_mixext ext;
    int dev;
    int mix_fd;
    int shift;
    int value_mask;
    QString type;
    QList <int> childs;

};
#endif





