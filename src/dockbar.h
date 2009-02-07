////////////////////////////////////////
//  File      : dockbar.h             //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

// create the dockbar on bottom of the screen

////////////////////////////////////////

#ifndef DOCKBAR_H
#define DOCKBAR_H

#include "defs.h"
#include "launcher.h"
#include "systray.h"
#include "dockicon.h"
#include "dateclock.h"

class Frame;
class Dockicon;
class Systray;
class Antico;
class Launcher;

////////////////////////////////////////

class Dockbar : public QLabel
{
    Q_OBJECT

public:
    Dockbar(Antico *, QWidget *parent=0);
    ~Dockbar();
    void add(Frame *);
    void update_size();
    void read_settings();
    void set_geometry();
    void update_style();
    void remove(Window);
    void update_dockicon_name(const QString &, Frame *);
    Systray *system_tray() const
    {
        return sys;
    }

public slots:
    void remove(Dockicon *);

private:
    QHBoxLayout *dock_layout;
    QHBoxLayout *icon_layout;
    QFrame *d_frame;
    QHash<int, Dockicon *> dock_icons; // mapping Frame and Dockicon (key=frame win_id value=Dockicon)
    int d_length; 	//dockicon lenght
    int dock_height;
    QString dock_pix;
    QString arrow_pix;
    QSettings *style;
    QSettings *antico;
    Launcher *lchr;
    Dockicon *d_icon;
    Systray *sys;
    Dateclock *clk;
    Antico *app;

};
#endif
