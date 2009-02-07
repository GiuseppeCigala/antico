////////////////////////////////////////
//  File      : systray.h             //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

// in the bottom right of dockbar

////////////////////////////////////////

#ifndef SYSTRAY_H
#define SYSTRAY_H

#include "defs.h"
#include "sysicon.h"

class Frame;
class Sysicon;

////////////////////////////////////////

class Systray : public QLabel
{
    Q_OBJECT

public:
    Systray(QWidget *parent=0);
    ~Systray();
    void read_settings();
    void add(Frame *);
    void add(Window);
    void remove(Window);
    void update_style();
 
public slots:
    void remove(Sysicon *);
    
protected:
    virtual bool x11Event (XEvent *);
    
private:
    QHBoxLayout *layout;
    QHash<int, Sysicon *> sys_icons; // mapping frame id with Sysicon (key=frame_win_id value=Sysicon)
    Sysicon *s_icon;
    QX11EmbedContainer *emb_cont;
    QString sys_pix;
    // System tray properties
    Atom kde_systray_prop;
    Atom net_opcode_atom;
    Atom net_selection_atom;
    Atom net_manager_atom;
    Atom net_message_data_atom;
};

#endif
