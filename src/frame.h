////////////////////////////////////////
//  File      : frame.h               //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

// create window frame around X11 apps
// manage all the move/resize event

////////////////////////////////////////

#ifndef FRAME_H
#define FRAME_H

#include "defs.h"
#include "border.h"
#include "header.h"
#include "dockbar.h"

class Dockbar;
class Header;

////////////////////////////////////////

class Frame : public QFrame
{
    Q_OBJECT

public:
    Frame(Window w, const QString &type, Dockbar *dock, QWidget *parent=0);
    ~Frame();
    void init();
    void update_style();
    void create_border();
    void get_client_geometry();
    void set_frame_geometry();
    void get_wm_hints();
    void get_wm_normal_hints();
    void withdraw();
    void unmap();
    void map();
    void raise();
    void get_wm_name();
    void get_wm_protocols();
    void send_wm_protocols(long, long);
    void resize_request(int, int);
    void set_active();
    void set_inactive();
    void set_client_state(int);
    void set_focus(long);
    void get_icon();
    void get_colormaps();
    void set_colormaps(Colormap);
    void read_settings(); // from "antico.cfg"
    WId cl_win()
    {
        return(c_win);
    }
    int cl_x()
    {
        return(x()+lateral_bdr_width);
    }
    int cl_y()
    {
        return(y()+top_bdr_height);
    }
    int cl_h()
    {
        return(height()-diff_border_h);
    }
    int cl_w()
    {
        return(width()-diff_border_w);
    }
    QString cl_name()
    {
        return(wm_name);
    }
    QPixmap cl_icon()
    {
        return(wicon);
    }
    bool is_iconize()
    {
        return iconize;
    }

private:
    QDesktopWidget *desk;       // root window
    WId c_win;                  // client window
    Dockbar *dock;              // dockbar
    QString frame_type;         // frame type (Normal, Splash, Dialog, Desktop..)
    QString app_icon;           // default header icon used if no icon is find
    QPoint mousepos;            // mouse position at button press
    int m_px, m_py, m_pw, m_ph; // normal parent geometry in maximize state
    int n_px, n_py, n_pw, n_ph; // normal parent geometry in normal mode
    int frame_w, frame_h;       // frame geometry
    int frame_x, frame_y;       // frame geometry
    int client_x, client_y;     // client geometry
    int client_w, client_h;     // client geometry
    int lateral_bdr_width;	    // parent lateral border width
    int top_bdr_height;         // parent top border height
    int bottom_bdr_height;	    // parent top border height
    QString header_active_pix;  // frame header pixmap
    QString header_inactive_pix;// frame header pixmap
    QString minmax_pix;         // frame minmax pixmap
    QString close_pix;          // frame close pixmap
    QString arrow_pix;          // frame cursor
    QString move_pix;           // frame cursor
    QString left_pix;           // frame cursor
    QString right_pix;          // frame cursor
    QString bottom_pix;         // frame cursor
    QString bottom_left_pix;    // frame cursor
    QString bottom_right_pix;   // frame cursor
    QColor title_color;         // frame title color
    int diff_border_h;          // height space between parent frame (qt) and client frame
    int diff_border_w;          // width space between parent frame (qt) and client frame
    bool maximized;             // maximize window
    bool iconize;               // iconize window
    bool prot_delete;           // client has delete WM protocol
    bool prot_take_focus;       // client has take focus WM protocol
    QPixmap wicon;              // window icon
    int dock_height;            // dockbar height
    QString wm_name;            // WM_NAME property or res_name
    QString res_name;           // ClassHint
    QString res_class;          // ClassHint
    bool inputfield;            // WMHints
    Colormap cmap;              // colormap
    Header *tm_bdr;             // top mid window border (for window move)
    Border *tl_bdr;             // top left window border (icon)
    Border *tr_bdr;             // top right window border (icon)
    Border *bm_bdr;             // bottom mid window border
    Border *bl_bdr;             // bottom left window border
    Border *br_bdr;             // bottom right window border
    Border *l_bdr;              // left window border
    Border *r_bdr;              // right window border
    Border *c_bdr;              // center window border (client apps)
    QGridLayout *layout;
    QSettings *style;

public slots:
    void press_top_mid(QMouseEvent *);          // top mid border press
    void move_top_mid(QMouseEvent *);           // top mid border move
    void press_bottom_left(QMouseEvent *);      // bottom left border press
    void move_bottom_left(QMouseEvent *);       // bottom left border move
    void press_bottom_right(QMouseEvent *);     // bottom right border press
    void move_bottom_right(QMouseEvent *);      // bottom right border move
    void press_bottom_mid(QMouseEvent *);       // bottom mid border press
    void move_bottom_mid(QMouseEvent *);        // bottom mid border move
    void press_right(QMouseEvent *);            // right border press
    void move_right(QMouseEvent *);             // right border move
    void press_left(QMouseEvent *);             // left border press
    void move_left(QMouseEvent *);              // left border move
    void destroy();                             // destroy client
    void maximize();                            // maximize client
    void iconify();                             // iconify client

};
#endif