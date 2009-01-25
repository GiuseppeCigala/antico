////////////////////////////////////////
//  File      : colordialog.h         //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

// dialog for color selection

////////////////////////////////////////

#ifndef COLORDIALOG_H
#define COLORDIALOG_H

#include "defs.h"
#include "colorsel.h"

////////////////////////////////////////

class Colordialog : public QDialog
{
    Q_OBJECT

public:
    Colordialog(const QColor &color, QWidget *parent=0);
    ~Colordialog();
    QColor get_color();
    void init();

protected:
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

signals:

public slots:

private:
    QPoint mousepos;
    QDialogButtonBox *buttonBox;
    QColor actual_col;
    Colorsel *col_sel;

};


#endif
