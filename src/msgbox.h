////////////////////////////////////////
//  File      : msgbox.h              //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

// information box

////////////////////////////////////////

#ifndef MSGBOX_H
#define MSGBOX_H

#include "defs.h"

////////////////////////////////////////

class Msgbox : public QMessageBox
{
    Q_OBJECT

public:
    Msgbox(QWidget *parent=0);
    ~Msgbox();

protected:
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

signals:

public slots:

private:
    QPoint mousepos;

};


#endif
