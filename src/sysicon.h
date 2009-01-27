////////////////////////////////////////
//  File      : sysicon.h             //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

// app iconify added on system tray

////////////////////////////////////////

#ifndef SYSICON_H
#define SYSICON_H

#include "defs.h"
#include "frame.h"

class Frame;

////////////////////////////////////////

class Sysicon : public QWidget
{
    Q_OBJECT

public:
    Sysicon(Frame *frm, QWidget *parent=0);
    ~Sysicon();
    void read_settings();
    void update_style();

protected:
    void mousePressEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);

public slots:
    void sys_close();

signals:
    void show_sys(Sysicon *);
    void destroy_sys(Sysicon *);

private:
    Frame *frm;
    QString s_icon_pix;
    QString close_dock_pix;
    QPixmap pix;
    QSettings *style;
    QColor title_color;
    int bdr_width;

};

#endif
