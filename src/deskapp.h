////////////////////////////////////////
//  File      : deskapp.h             //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

// add application link on the desk

////////////////////////////////////////

#ifndef DESKAPP_H
#define DESKAPP_H

#include "defs.h"
#include "filedialog.h"

////////////////////////////////////////

class Deskapp : public QWidget
{
    Q_OBJECT

public:
    Deskapp(const QString &app_name, const QString &app_path, const QString &app_pix, QWidget *parent=0);
    ~Deskapp();
    void read_settings();
    void update_style();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);

signals:

public slots:
    void del_app();

private:
    QMenu *menu;
    QString app_name;
    QString app_path;
    QString delete_link_pix;
    QPoint mousepos;
    QColor d_app_col;
    QPixmap d_app_pix;
    QSettings *style;
    QSettings *antico;
    bool zoom;
};

#endif

