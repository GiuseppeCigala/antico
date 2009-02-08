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

class Filedialog;

////////////////////////////////////////

class Deskfolder : public QWidget
{
    Q_OBJECT

public:
    Deskfolder(Filedialog *, Categorymenu *, const QString &, const QString &, const QRect &, QWidget *parent=0);
    ~Deskfolder();
    void read_settings();
    void update_style();
    void init();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);
    void contextMenuEvent(QContextMenuEvent *);

signals:
    void destroy_deskfolder(Deskfolder *);

public slots:
    void del_folder();

private:
    QMenu *main_menu;
    QMenu *open_menu;
    QString dir_name;
    QString dir_path;
    QRect geometry;
    QPoint mousepos;
    QString d_folder_pix;
    QString delete_link_pix;
    QString open_with_pix;
    QColor d_folder_col;
    QPixmap pix;
    QSettings *style;
    QSettings *antico;
    bool zoom;
    Filedialog *file_dialog;
    Categorymenu *cat_menu;
};

#endif
