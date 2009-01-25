////////////////////////////////////////
//  File      : deskfolder.h          //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

// folder added on the desk

////////////////////////////////////////

#ifndef DESKFOLDER_H
#define DESKFOLDER_H

#include "defs.h"
#include "filedialog.h"

////////////////////////////////////////

class Deskfolder : public QLabel
{
    Q_OBJECT

public:
    Deskfolder(const QString &dir_name, const QString &dir_path, const QRect &geometry, QWidget *parent=0);
    ~Deskfolder();
    void read_settings();
    void update_style();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

signals:

public slots:
    void del_folder();

private:
    QMenu *menu;
    QString dir_name;
    QString dir_path;
    QRect geometry;
    QPoint mousepos;
    QString d_folder_pix;
    QString delete_link_pix;
    QColor d_folder_col;
    QPixmap pix;
    QSettings *style;
    QSettings *antico;
    
};

#endif
