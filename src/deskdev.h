////////////////////////////////////////
//  File      : deskdev.h             //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

// device added on the desk

////////////////////////////////////////

#ifndef DESKDEV_H
#define DESKDEV_H

#include "defs.h"
#include "filedialog.h"

////////////////////////////////////////

class Deskdev : public QLabel
{
    Q_OBJECT

public:
    Deskdev(const QString &device, const QString &mnt_path, const QString &label, const QString &type, QWidget *parent=0);
    ~Deskdev();
    void read_settings();
    void init();
    void update_style();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

signals:

public slots:
 
private:
    QString device_name;
    QString mount_path;
    QString device_label;
    QString device_type;
    QPoint mousepos;
    QString d_disk_pix;
    QString d_cdrom_pix;
    QColor d_dev_col;
    QPixmap dev_pix;
    QSettings *style;
    QSettings *antico;
    
};

#endif
