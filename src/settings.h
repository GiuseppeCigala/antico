////////////////////////////////////////
//  File      : settings.h            //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

// Show system settings

////////////////////////////////////////

#ifndef SETTINGS_H
#define SETTINGS_H

#include "defs.h"

////////////////////////////////////////

class Settings : public QDialog
{
    Q_OBJECT

public:
    Settings(QWidget *parent=0);
    ~Settings();
    void read_settings();
    void init();
    void display_tab();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

signals:

public slots:
    void ok_pressed();
    void close_pressed();

private:
    QString ok_button_pix;
    QString close_button_pix;
    QString stl_path;
    QString stl_name;
    QPoint mousepos;
    QTabWidget *tab;
    QSpinBox *hor_res_spin;
    QSpinBox *ver_res_spin;
    int scr_num;

};

#endif

