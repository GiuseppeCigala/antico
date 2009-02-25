////////////////////////////////////////
//  File      : desk.h                //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

// the desktop on the screen

////////////////////////////////////////

#ifndef DESK_H
#define DESK_H

#include "defs.h"
#include "deskfolder.h"
#include "deskfile.h"
#include "deskapp.h"
#include "deskdev.h"
#include "msgbox.h"
#include "antico.h"
#include "filedialog.h"
#include "trash.h"
#include "utils.h"

class Antico;
class Deskapp;
class Deskdev;
class Deskfile;
class Deskfolder;
class Filedialog;

////////////////////////////////////////

class Desk : public QLabel
{
    Q_OBJECT

public:
    Desk(Antico *, QWidget *parent=0);
    ~Desk();
    void init();
    void read_settings();
    void set_geometry();
    void set_desk_icons();
    void create_desk_app(const QString &, const QString &, const QPoint &, QWidget *);
    void create_desk_file(const QString &, const QString &, const QString &, const QPoint &, QWidget *);
    void create_desk_folder(const QString &, const QString &, const QRect &, const QPoint &, QWidget *);
    void update_style();
    void mount_device(const QString &, const QString &, const QString &, const QString &, const QString &);
    void unmount_device(const QString &);
  
public slots:
    void run_menu(QAction *);
    void remove_deskfolder(Deskfolder *);
    void remove_deskfile(Deskfile *);
    void remove_deskapp(Deskapp *);
    void device_added( const QString &);
    void device_removed(const QString &);

protected:
    void mousePressEvent(QMouseEvent *);
    void dragEnterEvent(QDragEnterEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);

private:
    QMenu *menu;
    QSettings *style;
    QSettings *antico;
    int dock_height;
    QList<Deskfolder *> desk_folders;
    QList<Deskfile *> desk_files;
    QList<Deskapp *> desk_apps;
    QHash<QString, Deskdev *> desk_dev;
    QDBusInterface *dbus_interface;
    QString wall_pix;
    QString folder_link_pix;
    QString file_link_pix;
    QString app_link_pix;
    Antico *app;
    Filedialog *file_dialog;
    Categorymenu *cat_menu;
    Deskfolder *d_folder;
    Deskfile *d_file;
    Deskapp *d_app;
    Appicon *app_icon;
    Trash *trsh;
    
};


#endif
