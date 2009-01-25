////////////////////////////////////////
//  File      : frame.cpp             //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "frame.h"

////////////////////////////////////////

Frame::Frame(Window w, const QString &type, Dockbar *dkbr, QWidget *parent) : QFrame(parent)
{
    dock = dkbr;
    frame_type = type;
    c_win = w;
    read_settings();
    init();
    setFrameStyle(QFrame::Panel|QFrame::Raised);
}

Frame::~Frame()
{}

void Frame::read_settings()
{
    // get style path
    QSettings *antico = new QSettings(QCoreApplication::applicationDirPath() + "/antico.cfg", QSettings::IniFormat, this);
    antico->beginGroup("Style");
    QString stl_name = antico->value("name").toString();
    QString stl_path = antico->value("path").toString();
    antico->endGroup(); //Style
    // get style values
    style = new QSettings(stl_path + stl_name, QSettings::IniFormat);
    ////// Frame //////
    style->beginGroup("Frame");
    style->beginGroup("Border");
    lateral_bdr_width = style->value("lateral_bdr_width").toInt();
    top_bdr_height = style->value("top_bdr_height").toInt();
    bottom_bdr_height = style->value("bottom_bdr_height").toInt();
    style->endGroup(); // Border
    style->beginGroup("Header");
    header_active_pix = stl_path + style->value("header_active_pix").toString();
    header_inactive_pix = stl_path + style->value("header_inactive_pix").toString();
    title_color = style->value("title_color").value<QColor>();
    minmax_pix = stl_path + style->value("minmax_pix").toString();
    close_pix = stl_path + style->value("close_pix").toString();
    style->endGroup(); //Header
    style->endGroup(); //Frame
    ////// Dockbar //////
    style->beginGroup("Dockbar");
    dock_height = style->value("dock_height").toInt();
    style->endGroup(); //Dockbar
    ////// Launcher //////
    style->beginGroup("Launcher");
    style->beginGroup("Icon");
    app_icon = stl_path + style->value("application_pix").toString();
    style->endGroup(); // Icon
    style->endGroup(); // Launcher
}

void Frame::init()
{
    desk = QApplication::desktop();
    maximized = false;
    iconize = false;

    XSelectInput(QX11Info::display(), winId(), KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
                 KeymapStateMask | ButtonMotionMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask | FocusChangeMask |
                 ExposureMask | StructureNotifyMask | SubstructureRedirectMask | SubstructureNotifyMask);

    XSetWindowAttributes at;
    at.event_mask = ColormapChangeMask|PropertyChangeMask;
    XChangeWindowAttributes(QX11Info::display(), c_win, CWEventMask, &at);

    XGrabServer(QX11Info::display());

    // get the client geometry
    get_client_geometry();
    // get the client hints
    get_wm_hints();
    // get the client normal hints
    get_wm_normal_hints();
    // get the client name
    get_wm_name();
    // get the client prot
    get_wm_protocols();
    // get the client icon to show on header
    get_icon();
    // set the frame geometry
    set_frame_geometry();

    XSetWindowBorderWidth(QX11Info::display(), c_win, 0);  //client
    XSetWindowBorderWidth(QX11Info::display(), winId(), 0);  //frame

    // ***THE MOST IMPORTANT FUNCTION*** // reparent client with frame
    XReparentWindow(QX11Info::display(), c_win, winId(), lateral_bdr_width, top_bdr_height);
    qDebug() << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";
    qDebug() << "Reparent Client:" << c_win << "with Frame:" << winId() << "- Name:" << cl_name();
    qDebug() << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << "\n";

    XAddToSaveSet(QX11Info::display(), c_win);
    // move and resize client
    XMoveResizeWindow(QX11Info::display(), c_win, lateral_bdr_width, top_bdr_height+3, client_w, client_h);

    // move the frame in desktop center and resize
    move((desk->width()/2)-(frame_w/2), (desk->height()/2)-(frame_h/2));
    
    //if the frame is too large, maximize
    if(frame_w >= QApplication::desktop()->width()-dock_height/2 || frame_h >= QApplication::desktop()->height()-dock_height)
        maximize();
    else
    resize(frame_w, frame_h); // normal size

    if (frame_type != "Splash") // Splash frame with no border
    {
        // create frame borders
        create_border();
        // set the header pixmap
        set_inactive();
    }

    // send _NET_ACTIVE_WINDOW property
    Atom _net_active_window = XInternAtom(QX11Info::display(), "_NET_ACTIVE_WINDOW", False);
    XClientMessageEvent xev;
    xev.type = ClientMessage;
    xev.window = c_win;
    xev.message_type = _net_active_window;
    xev.format = 32;
    xev.data.l[0] = 1; // from application
    xev.data.l[1] = CurrentTime;
    xev.data.l[2] = 0;
    XSendEvent(QX11Info::display(), QApplication::desktop()->winId(), False, StructureNotifyMask, (XEvent *)&xev);

    // map client
    XMapWindow(QX11Info::display(), c_win);
    XSync(QX11Info::display(), false);
    XUngrabServer(QX11Info::display());

    // show frame
    show();
}

void Frame::get_client_geometry()
{
    XWindowAttributes attr;
    XGetWindowAttributes(QX11Info::display(), c_win, &attr);
    client_x = attr.x;
    client_y = attr.y;
    client_w = attr.width;
    client_h = attr.height;

    if (client_w >= desk->width()-10)
        client_w = desk->width()-10;

    if (client_h >= desk->height()-dock_height-20)
        client_h = desk->height()-dock_height-20;

    qDebug() << "Client_x:" << client_x << "Client_y:" << client_y << "Client_w:" << client_w << "Client_h:" << client_h;
}

void Frame::set_frame_geometry()
{
    qDebug() << "Frame type:" << frame_type;

    if (frame_type == "Splash")
    {
        // set spacing between client and frame window
        diff_border_h = 0; // height space
        diff_border_w = 0; // width space
        // set frame width and height
        frame_w = client_w;
        frame_h = client_h;
        frame_x = client_x;
        frame_y = client_y;
        lateral_bdr_width = 0;
        top_bdr_height = 0;
    }
    else
    {
        // set spacing between client and frame window
        diff_border_h = top_bdr_height+bottom_bdr_height+3; // height space
        diff_border_w = 2*lateral_bdr_width; // width space
        // set frame width and height
        frame_w = client_w + diff_border_w;
        frame_h = client_h + diff_border_h;
        frame_x = client_x;
        frame_y = client_y;
    }
}

void Frame::update_style()
{
    read_settings();
    tl_bdr->setFixedSize(top_bdr_height, top_bdr_height);
    tr_bdr->setFixedSize(top_bdr_height, top_bdr_height);
    tm_bdr->set_pixmap(header_active_pix, header_inactive_pix, title_color);
    tm_bdr->setFixedHeight(top_bdr_height);
    bm_bdr->setFixedHeight(bottom_bdr_height);
    bl_bdr->setFixedSize(top_bdr_height, bottom_bdr_height);
    br_bdr->setFixedSize(top_bdr_height, bottom_bdr_height);
    get_client_geometry();
    set_frame_geometry();
    XMoveResizeWindow(QX11Info::display(), c_win, lateral_bdr_width, top_bdr_height+3, client_w, client_h); // update client
    resize(frame_w, frame_h); //update frame
}

void Frame::get_wm_hints()
{
    XWMHints *xwmhints;

    if ((xwmhints = XGetWMHints(QX11Info::display(), c_win)) != NULL)
    {
        if (xwmhints->flags & StateHint && xwmhints->initial_state == IconicState)
        {
            set_client_state(IconicState);
        }
        XFree(xwmhints);
    }
}

void Frame::get_wm_normal_hints() // Poor implementation of many applications ....
{
    XSizeHints *xsizehints;
    long hints = NULL;
    long wm_flags = NULL;
    int win_grav = 0;
    int norm_x = 0;
    int norm_y = 0;
    int norm_w = 0;
    int norm_h = 0;

    if ((xsizehints = XAllocSizeHints()) != NULL)
    {
        if (XGetWMNormalHints(QX11Info::display(), c_win, xsizehints, &hints) == 0)
            return;

        wm_flags = xsizehints->flags;
        win_grav = xsizehints->win_gravity;
        qDebug() << "Window flags:" << wm_flags << "Window grav:" << win_grav;

        if (wm_flags & PPosition)
        {
            client_x = xsizehints->x;
            client_y = xsizehints->y;
            qDebug() << "PPosition:" << norm_x << norm_y;
        }
        if (wm_flags & PSize)
        {
            if(xsizehints->width != 0)
            client_w = xsizehints->width;
            if(xsizehints->height != 0)
            client_h = xsizehints->height;
            qDebug() << "PSize:" << norm_w << norm_h;
        }
        if (wm_flags & PBaseSize)
        {
            if(xsizehints->base_width != 0)
            client_w = xsizehints->base_width;
            if(xsizehints->base_height != 0)
            client_h = xsizehints->base_height;
            qDebug() << "PBaseSize:" << norm_w << norm_h;
        }
        if (client_w >= desk->width()-10)
        client_w = desk->width()-10;

        if (client_h >= desk->height()-dock_height-20)
        client_h = desk->height()-dock_height-20;
    }
}

void Frame::set_client_state(int state)
{
    ulong data[2];
    data[0] = (ulong)state;
    data[1] = (ulong)None;
    Atom wm_state = XInternAtom(QX11Info::display(), "WM_STATE", FALSE);
    XChangeProperty(QX11Info::display(), c_win, wm_state, wm_state, 32, PropModeReplace, (uchar *)data, 2);
}

void Frame::set_focus(long timestamp) // set to focus to child
{
    XSetInputFocus(QX11Info::display(), c_win, RevertToParent, CurrentTime);

    Atom wm_take_focus = XInternAtom(QX11Info::display(), "WM_TAKE_FOCUS", False);
    if (prot_take_focus) // WM_TAKE_FOCUS protocol
        send_wm_protocols(wm_take_focus, timestamp);
}

void Frame::withdraw()
{
    unmap();
    XUnmapWindow(QX11Info::display(), c_win);
    set_client_state(WithdrawnState);
}

void Frame::unmap()
{
    hide();
}

void Frame::map()
{
    show();
    XMapWindow(QX11Info::display(), winId());
    XMapWindow(QX11Info::display(), c_win);
    set_client_state(NormalState);
    iconize = false;
}

void Frame::raise()
{
    show();
    XRaiseWindow(QX11Info::display(), winId());
    XRaiseWindow(QX11Info::display(), c_win);
    set_client_state(NormalState);
    iconize = false;
    dock->add(this); // transition from systray to raised (add to pager)
}

void Frame::iconify()
{
    unmap();
    set_client_state(IconicState);
    dock->add(this);  // add frame to dockbar (pager)
    iconize = true;
}

void Frame::get_wm_name()  // get WM_NAME
{
    char *name;
    Atom type;
    int format;
    unsigned long nitems=0;
    unsigned long extra=0;
    unsigned char *data=NULL;
    Atom _net_wm_name = XInternAtom(QX11Info::display(), "_NET_WM_NAME", False);
    Atom wm_locale_name = XInternAtom(QX11Info::display(), "WM_LOCALE_NAME", False);

    XGetWindowProperty(QX11Info::display(), c_win, _net_wm_name, 0, 500, FALSE, AnyPropertyType, &type, &format, &nitems, &extra, &data);

    if (data != NULL)
    {
        wm_name = QString::fromUtf8((char *)data);
        XFree(data);
    }
    else if (XFetchName(QX11Info::display(), c_win, &name) && name != NULL)
    {
        wm_name = name;
        XFree(name);
    }
    else  // use class hints
    {
        XClassHint ch;

        if (XGetClassHint(QX11Info::display(), c_win, &ch))
        {
            res_name = ch.res_name;
            res_class = ch.res_class;
            XFree(ch.res_name);
            XFree(ch.res_class);
            wm_name = res_name;
        }
    }
    qDebug() << "wm_name:" << wm_name;

    XGetWindowProperty(QX11Info::display(), c_win, wm_locale_name, 0, 500, FALSE, AnyPropertyType, &type, &format, &nitems, &extra, &data);
    if (data != NULL)
    {
        qDebug() << "wm_locale_name:" << QString::fromUtf8((char *)data);
        XFree(data);
    }
}

void Frame::get_wm_protocols()
{
    Atom *protocols;
    int nprot,i;
    prot_delete = false;
    prot_take_focus = false;

    Atom wm_delete_window = XInternAtom(QX11Info::display(), "WM_DELETE_WINDOW", False);
    Atom wm_take_focus = XInternAtom(QX11Info::display(), "WM_TAKE_FOCUS", False);

    if (XGetWMProtocols(QX11Info::display(), c_win, &protocols, &nprot))
    {
        for (i=0; i < nprot; i++)
        {
            if (protocols[i] == wm_delete_window)
            {
                prot_delete = true;
            }
            else if (protocols[i] == wm_take_focus)
            {
                prot_take_focus = true;
            }
        }
        XFree(protocols);
    }
}

void Frame::resize_request(int cw, int ch)  // client requested resize
{
    resize(cw+diff_border_w, ch+diff_border_h);
    XResizeWindow(QX11Info::display(), c_win, cw, ch);
}

void Frame::set_active() // set header active
{
    tm_bdr->set_active();
}

void Frame::set_inactive() // set header inactive
{
    tm_bdr->set_inactive();
}

void Frame::send_wm_protocols(long data0, long data1)  // send protocol message to child window
{
    XEvent event;
    Atom wm_protocols = XInternAtom(QX11Info::display(), "WM_PROTOCOLS", False);
    memset(&event, 0, sizeof(event));
    event.xclient.type = ClientMessage;
    event.xclient.window = c_win;
    event.xclient.message_type = wm_protocols;
    event.xclient.format = 32;
    event.xclient.data.l[0] = data0;
    event.xclient.data.l[1] = data1;
    XSendEvent(QX11Info::display(), c_win, False, 0L, &event);
}

void Frame::get_icon()
{
    Atom type_ret = None;
    unsigned char *data = 0;
    int format = 0;
    unsigned long n = 0;
    unsigned long extra = 0;
    int width = 0;
    int height = 0;

    Atom _net_wm_icon = XInternAtom(QX11Info::display(), "_NET_WM_ICON", False);

    if (XGetWindowProperty(QX11Info::display(), c_win, _net_wm_icon, 0, 1, False,
                           XA_CARDINAL, &type_ret, &format, &n, &extra, (unsigned char **)&data) == Success && data)
    {
        width = data[0];
        qDebug() << "Icon width:" << width;
        XFree(data);
    }
    if (XGetWindowProperty(QX11Info::display(), c_win, _net_wm_icon, 1, 1, False,
                           XA_CARDINAL, &type_ret, &format, &n, &extra, (unsigned char **)&data) == Success && data)
    {
        height = data[0];
        qDebug() << "Icon height:" << height;
        XFree(data);
    }
    if (XGetWindowProperty(QX11Info::display(), c_win, _net_wm_icon, 2, width*height, False,
                           XA_CARDINAL, &type_ret, &format, &n, &extra, (unsigned char **)&data) == Success && data)
    {
        QImage img(data, width, height, QImage::Format_ARGB32);
        wicon = QPixmap::fromImage(img);
        XFree(data);
    }
    else // if no icon is found
    {
        QPixmap pix(app_icon);
        wicon = pix;
    }
}

void Frame::get_colormaps()
{
    XWindowAttributes attr;
    XGetWindowAttributes(QX11Info::display(), c_win, &attr);
    cmap = attr.colormap;
}

void Frame::set_colormaps(Colormap cmap)
{
    if (cmap == None)
        cmap = DefaultColormap(QX11Info::display(), DefaultScreen(QX11Info::display()));

    XInstallColormap(QX11Info::display(), cmap);
}


////////// WINDOW BORDER CREATION //////////////

void Frame::create_border()
{
    layout = new QGridLayout(this); // 3x3 grid for window borders
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);

    QString path = QCoreApplication::applicationDirPath(); // application directory
  
    // center frame where client apps is shown
    c_bdr = new Border(this);
    layout->addWidget(c_bdr, 1, 1);
    // top left border (icon)
    tl_bdr = new Border(this);
    tl_bdr->setToolTip(QString("Minimize/Maximize"));
    tl_bdr->setFixedSize(top_bdr_height, top_bdr_height);
    tl_bdr->setPixmap(minmax_pix);
    tl_bdr->setScaledContents(true);
    tl_bdr->setAlignment(Qt::AlignCenter);
    layout->addWidget(tl_bdr, 0, 0);
    // top right border (icon)
    tr_bdr = new Border(this);
    tr_bdr->setToolTip(QString("Close"));
    tr_bdr->setFixedSize(top_bdr_height, top_bdr_height);
    tr_bdr->setPixmap(close_pix);
    tr_bdr->setScaledContents(true);
    tr_bdr->setAlignment(Qt::AlignCenter);
    layout->addWidget(tr_bdr, 0, 2);
    // top mid header border (move frame)
    tm_bdr = new Header(cl_icon(), cl_name());
    tm_bdr->set_pixmap(header_active_pix, header_inactive_pix, title_color);
    tm_bdr->setFixedHeight(top_bdr_height);
    layout->addWidget(tm_bdr, 0, 1);
    // bottom mid border
    bm_bdr = new Border(this);
    bm_bdr->setFixedHeight(bottom_bdr_height);
    bm_bdr->setCursor(Qt::SizeVerCursor);
    layout->addWidget(bm_bdr, 2, 1);
    // bottom left border
    bl_bdr = new Border(this);
    bl_bdr->setFixedSize(top_bdr_height, bottom_bdr_height);
    bl_bdr->setCursor(Qt::SizeBDiagCursor);
    layout->addWidget(bl_bdr, 2, 0);
    // bottom right border
    br_bdr = new Border(this);
    br_bdr->setFixedSize(top_bdr_height, bottom_bdr_height);
    br_bdr->setCursor(Qt::SizeFDiagCursor);
    layout->addWidget(br_bdr, 2, 2);
    // left border
    l_bdr = new Border(this);
    l_bdr->setCursor(Qt::SizeHorCursor);
    layout->addWidget(l_bdr, 1, 0);
    // right border
    r_bdr = new Border(this);
    r_bdr->setCursor(Qt::SizeHorCursor);
    layout->addWidget(r_bdr, 1, 2);

    // top left (icon)
    connect(tl_bdr, SIGNAL(mouse_left_press()), this, SLOT(iconify()));
    connect(tl_bdr, SIGNAL(mouse_right_press()), this, SLOT(maximize()));
    // top right (icon)
    connect(tr_bdr, SIGNAL(mouse_left_press()), this, SLOT(destroy()));
    // top mid (title bar)
    connect(tm_bdr, SIGNAL(mouse_double_click()), this, SLOT(iconify()));
    connect(tm_bdr, SIGNAL(mouse_left_press(QMouseEvent *)), this, SLOT(press_top_mid(QMouseEvent *)));
    connect(tm_bdr, SIGNAL(mouse_move(QMouseEvent *)), this, SLOT(move_top_mid(QMouseEvent *)));
    // bottom left
    connect(bl_bdr, SIGNAL(mouse_left_press(QMouseEvent *)), this, SLOT(press_bottom_left(QMouseEvent *)));
    connect(bl_bdr, SIGNAL(mouse_move(QMouseEvent *)), this, SLOT(move_bottom_left(QMouseEvent *)));
    // bottom right
    connect(br_bdr, SIGNAL(mouse_left_press(QMouseEvent *)), this, SLOT(press_bottom_right(QMouseEvent *)));
    connect(br_bdr, SIGNAL(mouse_move(QMouseEvent *)), this, SLOT(move_bottom_right(QMouseEvent *)));
    // bottom mid
    connect(bm_bdr, SIGNAL(mouse_left_press(QMouseEvent *)), this, SLOT(press_bottom_mid(QMouseEvent *)));
    connect(bm_bdr, SIGNAL(mouse_move(QMouseEvent *)), this, SLOT(move_bottom_mid(QMouseEvent *)));
    // left
    connect(l_bdr, SIGNAL(mouse_left_press(QMouseEvent *)), this, SLOT(press_left(QMouseEvent *)));
    connect(l_bdr, SIGNAL(mouse_move(QMouseEvent *)), this, SLOT(move_left(QMouseEvent *)));
    // right
    connect(r_bdr, SIGNAL(mouse_left_press(QMouseEvent *)), this, SLOT(press_right(QMouseEvent *)));
    connect(r_bdr, SIGNAL(mouse_move(QMouseEvent *)), this, SLOT(move_right(QMouseEvent *)));
}

////////// TITLE BAR (HEADER) MOVEMENT //////////////

void Frame::press_top_mid(QMouseEvent *event)
{
    mousepos = event->pos()+tm_bdr->pos();  // offset
}

void Frame::move_top_mid(QMouseEvent *event)
{
    QPoint p(event->globalPos()-mousepos);
    if (p.x() <= 0)
        p.setX(0);
    if (p.y() <= 0)
        p.setY(0);
    else
        move(p.x(), p.y());
}

////////// BOTTOM LEFT RESIZE //////////////
void Frame::press_bottom_left(QMouseEvent *event)
{
    mousepos = event->globalPos();
}

void Frame::move_bottom_left(QMouseEvent *event)
{
    QPoint dpos = event->globalPos()-mousepos;
    int resw = width()-dpos.x();
    int resh = height()+dpos.y();
    int resx = x()-resw+width();
    //move and resize parent
    move(resx, y());
    resize(resw, resh);
    XResizeWindow(QX11Info::display(), c_win, resw-diff_border_w, resh-diff_border_h); //client
    mousepos = event->globalPos();
}

////////// BOTTOM RIGHT RESIZE //////////////

void Frame::press_bottom_right(QMouseEvent *event)
{
    mousepos = event->globalPos();
}

void Frame::move_bottom_right(QMouseEvent *event)
{
    QPoint dpos = event->globalPos()-mousepos;
    int resw = width()+dpos.x();
    int resh = height()+dpos.y();
    //move and resize parent
    move(x(), y());
    resize(resw, resh);
    XResizeWindow(QX11Info::display(), c_win, resw-diff_border_w, resh-diff_border_h); //client
    mousepos = event->globalPos();
}

////////// BOTTOM MID RESIZE //////////////

void Frame::press_bottom_mid(QMouseEvent *event)
{
    mousepos = event->globalPos();
}

void Frame::move_bottom_mid(QMouseEvent *event)
{
    QPoint dpos = event->globalPos()-mousepos;
    int resh = height()+dpos.y();
    int resw = width();
    //move and resize parent
    move(x(), y());
    resize(resw, resh);
    XResizeWindow(QX11Info::display(), c_win, resw-diff_border_w, resh-diff_border_h);
    mousepos = event->globalPos();
}

////////// RIGHT RESIZE //////////////

void Frame::press_right(QMouseEvent *event)
{
    mousepos = event->globalPos();
}

void Frame::move_right(QMouseEvent *event)
{
    QPoint dpos = event->globalPos()-mousepos;
    int resw = width()+dpos.x();
    int resh = height();
    //move and resize parent
    move(x(), y());
    resize(resw, resh);
    XResizeWindow(QX11Info::display(), c_win, resw-diff_border_w, resh-diff_border_h); //client
    mousepos = event->globalPos();
}

////////// LEFT RESIZE //////////////

void Frame::press_left(QMouseEvent *event)
{
    mousepos = event->globalPos();
}

void Frame::move_left(QMouseEvent *event)
{
    QPoint dpos = event->globalPos()-mousepos;
    int resw = width()-dpos.x();
    int resh = height();
    int resx = x()-resw+width();
    //move and resize parent
    move(resx, y());
    resize(resw, resh);
    XResizeWindow(QX11Info::display(), c_win, resw-diff_border_w, resh-diff_border_h); //client
    mousepos = event->globalPos();
}

////////// DESTROY WINDOW //////////////
void Frame::destroy()
{
    if (prot_delete)
    {
        qDebug() << "--> Send wm_delete_window";
        Atom wm_delete_window = XInternAtom(QX11Info::display(), "WM_DELETE_WINDOW", FALSE);
        send_wm_protocols(wm_delete_window, CurrentTime);
    }
    else
    {
        qDebug() << "--> Call XKillClient";
        XKillClient(QX11Info::display(), c_win);
        XSync(QX11Info::display(), FALSE);
    }
}

////////// MAXIMIZE WINDOW //////////////
void Frame::maximize()
{
    if (! maximized)
    {
        // save parent dimension
        n_px = x();
        n_py = y();
        n_pw = width();
        n_ph = height();
        // maximize parent with (0,0) vertex and screen dimension-dockbar height
        m_pw = desk->width();
        m_ph = desk->height()-dock_height;
        move(desk->x(), desk->y());
        resize(m_pw, m_ph);
        raise();
        // maximize client
        XResizeWindow(QX11Info::display(), c_win, width()-diff_border_w, height()-diff_border_h);
        maximized = true;
    }
    else
    {
        // set last parent dimension
        move(n_px, n_py);
        resize(n_pw, n_ph);
        raise();
        // set last client dimension
        XResizeWindow(QX11Info::display(), c_win, width()-diff_border_w, height()-diff_border_h);
        maximized = false;
    }
}