////////////////////////////////////////
//  File      : antico.cpp            //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "antico.h"

////////////////////////////////////////

Antico::Antico(int &argc, char **argv) : QApplication(argc, argv)
{
    set_event_names();
    next_frame = 0; // for [Alt+Tab] key combination
    // get the atoms (ICCCM/EWMH)
    get_atoms();
    // set application settings on first installation
    set_settings();
    // create dockbar
    dock = new Dockbar(this);
    //create desk
    dsk = new Desk();
    // send ClientMessage to root for supported hints
    send_supported_hints();
    // run application from startup list
    run_app_at_startup();
}

Antico::~Antico()
{
    delete dsk;
}

void Antico::set_event_names()
{
    event_names.insert(0, "");
    event_names.insert(1, "");
    event_names.insert(2, "KeyPress");
    event_names.insert(3, "KeyRelease");
    event_names.insert(4, "ButtonPress");
    event_names.insert(5, "ButtonRelease");
    event_names.insert(6, "MotionNotify");
    event_names.insert(7, "EnterNotify");
    event_names.insert(8, "LeaveNotify");
    event_names.insert(9, "FocusIn");
    event_names.insert(10, "FocusOut");
    event_names.insert(11, "KeymapNotify");
    event_names.insert(12, "Expose");
    event_names.insert(13, "GraphicsExpose");
    event_names.insert(14, "NoExpose");
    event_names.insert(15, "VisibilityNotify");
    event_names.insert(16, "CreateNotify");
    event_names.insert(17, "DestroyNotify");
    event_names.insert(18, "UnmapNotify");
    event_names.insert(19, "MapNotify");
    event_names.insert(20, "MapRequest");
    event_names.insert(21, "ReparentNotify");
    event_names.insert(22, "ConfigureNotify");
    event_names.insert(23, "ConfigureRequest");
    event_names.insert(24, "GravityNotify");
    event_names.insert(25, "ResizeRequest");
    event_names.insert(26, "CirculateNotify");
    event_names.insert(27, "CirculateRequest");
    event_names.insert(28, "PropertyNotify");
    event_names.insert(29, "SelectionClear");
    event_names.insert(30, "SelectionRequest");
    event_names.insert(31, "SelectionNotify");
    event_names.insert(32, "ColormapNotify");
    event_names.insert(33, "ClientMessage");
    event_names.insert(34, "MappingNotify");
}

void Antico::get_atoms()
{
    // get WM protocols required by ICCCM
    wm_protocols = XInternAtom(QX11Info::display(), "WM_PROTOCOLS", False);
    wm_delete_window = XInternAtom(QX11Info::display(), "WM_DELETE_WINDOW", False);
    wm_change_state = XInternAtom(QX11Info::display(), "WM_CHANGE_STATE", False);
    wm_state = XInternAtom(QX11Info::display(), "WM_STATE", False);
    wm_take_focus = XInternAtom(QX11Info::display(), "WM_TAKE_FOCUS", False);
    wm_resource_manager = XInternAtom(QX11Info::display(), "RESOURCE_MANAGER", False);
    wm_colormaps = XInternAtom(QX11Info::display(), "WM_COLORMAP_WINDOWS", False);
    wm_hints = XInternAtom(QX11Info::display(), "WM_HINTS", False);
    wm_normal_hints = XInternAtom(QX11Info::display(), "WM_NORMAL_HINTS", False);
    wm_name = XInternAtom(QX11Info::display(), "WM_NAME", False);
    wm_transient_for = XInternAtom(QX11Info::display(), "WM_TRANSIENT_FOR", False);

    // extensions required by EWMH
    _net_wm_name = XInternAtom(QX11Info::display(), "_NET_WM_NAME", False);
    _net_wm_icon = XInternAtom(QX11Info::display(), "_NET_WM_ICON", False);
    _net_wm_user_time = XInternAtom(QX11Info::display(), "_NET_WM_USER_TIME", False);
    _net_supported = XInternAtom(QX11Info::display(), "_NET_SUPPORTED", False);
    _net_wm_window_type = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE", False);
    _net_wm_window_type_normal = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE_NORMAL", False);
    _net_wm_window_type_desktop = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE_DESKTOP", False);
    _net_wm_window_type_dialog = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE_DIALOG", False);
    _net_wm_window_type_splash = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE_SPLASH", False);
    _kde_net_wm_system_tray_window_for = XInternAtom(QX11Info::display(), "_KDE_NET_WM_SYSTEM_TRAY_WINDOW_FOR", False);
}

void Antico::send_supported_hints()
{
    // maximum five 32-bit values for message
    Atom _net_supported = XInternAtom(QX11Info::display(), "_NET_SUPPORTED", False);
    qDebug("Sending _NET_SUPPORTED");
    XClientMessageEvent xev1;
    XClientMessageEvent xev2;
    XClientMessageEvent xev3;
    XClientMessageEvent xev4;
    XClientMessageEvent xev5;

    xev1.type = ClientMessage;
    xev1.window = QApplication::desktop()->winId();
    xev1.message_type = _net_supported;
    xev1.format = 32;
    xev1.data.l[0] = wm_protocols;
    xev1.data.l[1] = wm_delete_window;
    xev1.data.l[2] = wm_change_state;
    xev1.data.l[3] = wm_state;
    xev1.data.l[4] = wm_take_focus;
    XSendEvent(QX11Info::display(), QApplication::desktop()->winId(), False,
               (SubstructureNotifyMask | SubstructureRedirectMask), (XEvent *)&xev1);

    xev2.type = ClientMessage;
    xev2.window = QApplication::desktop()->winId();
    xev2.message_type = _net_supported;
    xev2.format = 32;
    xev2.data.l[0] = wm_resource_manager;
    xev2.data.l[1] = wm_colormaps;
    xev2.data.l[2] = wm_hints;
    xev2.data.l[3] = wm_normal_hints;
    xev2.data.l[4] = wm_name;
    XSendEvent(QX11Info::display(), QApplication::desktop()->winId(), False,
               (SubstructureNotifyMask | SubstructureRedirectMask), (XEvent *)&xev2);

    xev3.type = ClientMessage;
    xev3.window = QApplication::desktop()->winId();
    xev3.message_type = _net_supported;
    xev3.format = 32;
    xev3.data.l[0] = wm_transient_for;
    xev3.data.l[1] = _net_wm_name;
    xev3.data.l[2] = _net_wm_icon;
    xev3.data.l[3] = _net_supported;
    xev3.data.l[4] = _net_wm_user_time;
    XSendEvent(QX11Info::display(), QApplication::desktop()->winId(), False,
               (SubstructureNotifyMask | SubstructureRedirectMask), (XEvent *)&xev3);

    xev4.type = ClientMessage;
    xev4.window = QApplication::desktop()->winId();
    xev4.message_type = _net_supported;
    xev4.format = 32;
    xev4.data.l[0] = _net_wm_window_type;
    xev4.data.l[1] = _net_wm_window_type_normal;
    xev4.data.l[2] = _net_wm_window_type_desktop;
    xev4.data.l[3] = _net_wm_window_type_dialog;
    xev4.data.l[4] = _net_wm_window_type_splash;
    XSendEvent(QX11Info::display(), QApplication::desktop()->winId(), False,
               (SubstructureNotifyMask | SubstructureRedirectMask), (XEvent *)&xev4);

    xev5.type = ClientMessage;
    xev5.window = QApplication::desktop()->winId();
    xev5.message_type = _net_supported;
    xev5.format = 32;
    xev5.data.l[0] = _kde_net_wm_system_tray_window_for;
    XSendEvent(QX11Info::display(), QApplication::desktop()->winId(), False,
               (SubstructureNotifyMask | SubstructureRedirectMask), (XEvent *)&xev5);
}


bool Antico::x11EventFilter(XEvent *event)
{
    Frame *frm;
    XPropertyEvent *pev;
    XClientMessageEvent *mev;
    KeySym sym;
    uint mod;
    uint keymask1 = Mod1Mask & 0x0F;

    if (event->type!=6) // ignore Motion event
    {
        qDebug() << "--------------------------------------------------------------------------------------------";
        qDebug() << "XEvent:" << event_names.value(event->type) << " ( WId:" << event->xany.window << ")";
    }

    switch (event->type)
    {
    case MapRequest:
        qDebug() << "[MapRequest]" << event->xmaprequest.window;

        if ((frm = mapping_clients.value(event->xmaprequest.window)) != NULL)
        {
            qDebug() << "--> Map client:" << event->xmaprequest.window;
            frm->map();
        }
        else
        {
            qDebug() << "--> Map new client:" << event->xmaprequest.window;
            create_frame(event->xmaprequest.window, dock); // create new frame for client
        }
        return false;
        break;

    case MapNotify:
        qDebug() << "[MapNotify]";

        if (event->xmap.event != event->xmap.window)
            return true;
        else
            return false;
        break;

    case UnmapNotify:
        qDebug() << "[UnmapNotify]";

        if ((frm = mapping_clients.value(event->xunmap.window)) != NULL)
        {
            if (event->xunmap.send_event)
            {
                frm->withdraw();
                dock->remove(frm->winId()); // remove Dockicon still mapped
                qDebug() << "UnmapNotify for frame:" << frm->winId() << "- Name:" << frm->cl_name();
            }
            else
            {
                frm->unmap();
                qDebug() << "UnmapNotify for frame:" << frm->winId() << "- Name:" << frm->cl_name();
            }
            return true;
        }
        if (event->xunmap.event != event->xunmap.window)
            return true;

        return false;
        break;

    case DestroyNotify:
        qDebug() << "[DestroyNotify]";

        if ((frm = mapping_clients.value(event->xdestroywindow.window)) != NULL)
        {
            qDebug() << "--> Destroy frame:" << frm->winId() << "- Name:" << frm->cl_name() << "- Client:" << event->xdestroywindow.window;
            mapping_clients.remove(event->xdestroywindow.window);
            mapping_frames.remove(frm->winId());
            dock->remove(frm->winId()); // remove eventually Dockicon or Sysicon still mapped
            delete frm;
            return true;
        }
        if (event->xdestroywindow.event != event->xdestroywindow.window)
            return true;

        return false;
        break;

    case ConfigureRequest:
        qDebug() << "[ConfigureRequest]";
        XWindowChanges wc;

        if ((frm = mapping_clients.value(event->xconfigurerequest.window)) != NULL)
        {
            qDebug() << "--> Request from map frame:" << frm->winId() << "- Name:" << frm->cl_name() << "- Client:" << event->xconfigurerequest.window;

            if (event->xconfigurerequest.value_mask & (CWWidth|CWHeight|CWX|CWY))
            {
                int cx,cy,cw,ch;

                if (event->xconfigurerequest.value_mask & CWWidth)
                    cw = event->xconfigurerequest.width;
                else
                    cw = frm->cl_w();

                if (event->xconfigurerequest.value_mask & CWHeight)
                    ch = event->xconfigurerequest.height;
                else
                    ch = frm->cl_h();

                if ((event->xconfigurerequest.value_mask & CWX))
                    cx = event->xconfigurerequest.x;
                else
                    cx = frm->cl_x();

                if ((event->xconfigurerequest.value_mask & CWY))
                    cy = event->xconfigurerequest.y;
                else
                    cy = frm->cl_y();

                frm->resize_request(cw, ch);
                event->xconfigurerequest.value_mask &= ~(CWWidth|CWHeight|CWX|CWY);
            }

            if (! event->xconfigurerequest.value_mask)
                return true;

            wc.width = frm->width();
            wc.height = frm->height();
            wc.x = frm->x();
            wc.y = frm->y();
            wc.border_width = 0;
            wc.sibling = event->xconfigurerequest.above;
            wc.stack_mode = event->xconfigurerequest.detail;
            XConfigureWindow(QX11Info::display(), frm->winId(), event->xconfigurerequest.value_mask, &wc);
            send_configurenotify(frm);
        }
        else  // never mapped window
        {
            qDebug() << "--> Request from unmap client:" << event->xconfigurerequest.window;
            wc.x = event->xconfigurerequest.x;
            wc.y = event->xconfigurerequest.y;
            wc.width = event->xconfigurerequest.width;
            wc.height = event->xconfigurerequest.height;
            event->xconfigurerequest.value_mask &= (CWX|CWY|CWWidth|CWHeight);
            XConfigureWindow(QX11Info::display(), event->xconfigurerequest.window, event->xconfigurerequest.value_mask, &wc);
        }
        return false;
        break;

    case ConfigureNotify:
        qDebug() << "[ConfigureNotify]";

        if (event->xconfigure.event != event->xconfigure.window)
            return true;

        return false;
        break;

    case ReparentNotify:
        qDebug() << "[ReparentNotify]";

        if ((frm = mapping_clients.value(event->xreparent.window)) != NULL && event->xreparent.parent != frm->winId())
        {
            qDebug() << "ReparentNotify for frame:" << frm->winId() << "- Name:" << frm->cl_name();
            mapping_clients.remove(event->xreparent.window);
        }
        return true;
        break;

    case CirculateRequest:
        qDebug() << "[CirculateRequest]";

        if ((frm = mapping_clients.value(event->xcirculaterequest.window)) != NULL)
        {
            if (event->xcirculaterequest.place == PlaceOnTop)
            {
                frm->raise();
                qDebug() << "--> CirculateRequest for frame:" << frm->winId() << "- Name:" << frm->cl_name() << "- Client:" << event->xcirculaterequest.window;
            }
        }
        else
            XLowerWindow(QX11Info::display(), event->xcirculaterequest.window);

        return false;
        break;

    case PropertyNotify:
        qDebug() << "[PropertyNotify]";
        qDebug() << "Atom: " << XGetAtomName(QX11Info::display(), event->xproperty.atom) << '\n';
        pev = &event->xproperty;

        if ((frm = mapping_clients.value(event->xproperty.window)) != NULL)
        {
            qDebug() << "Client already mapped from frame:" << frm->winId() << "- Name:" << frm->cl_name() << "- Client:" << event->xproperty.window;

            if (pev->atom == wm_hints)
            {
                qDebug() << "---> wm_hints";
                frm->get_wm_hints();
            }
            if (pev->atom == wm_normal_hints)
            {
                qDebug() << "---> wm_normal_hints";
                frm->get_wm_normal_hints();
            }
            else if (pev->atom == wm_name || pev->atom == _net_wm_name)
            {
                qDebug() << "---> wm_name";
                frm->get_wm_name();
                frm->update_name();
            }
            if (pev->atom == wm_state)
            {
                qDebug() << "---> wm_state";
            }
            else if (pev->atom == wm_colormaps)
            {
                qDebug() << "---> wm_colormap_windows";
                frm->get_colormaps();
            }
            else if (pev->atom == wm_transient_for)
            {
                qDebug() << "---> wm_transient_for";
                XGrabServer(QX11Info::display());
            }
            else if (pev->atom == _net_wm_user_time)
            {
                qDebug() << "---> _net_wm_user_time";
                set_active_frame(frm); // activation of selected frame and unactivation of others
            }
            return true;
        }
        else if ((frm = mapping_frames.value(event->xproperty.window)) != NULL)
        {
            qDebug() << "Frame already mapped:" << event->xproperty.window << "- Name:" << frm->cl_name() << "- Client:" << frm->cl_win();
            return false;
        }
        else
        {
            qDebug() << "Client not mapped:" << event->xproperty.window;
            return false;
        }
        break;

    case ButtonPress:
        qDebug() << "[ButtonPress]";

        if ((frm = mapping_frames.value(event->xbutton.window)) != NULL) // get the frame from his winId
        {
            qDebug() << "Button press for map frame:" << event->xbutton.window;
            set_active_frame(frm);
        }
        else if (event->xbutton.window != dsk->winId())
        {
            qDebug() << "Button press for not map client:" << event->xbutton.window;
            XRaiseWindow(QX11Info::display(), event->xbutton.window);
        }
        return false;
        break;

    case ClientMessage:
        qDebug() << "[ClientMessage]";
        mev = &event->xclient;

        if (mev->message_type == wm_change_state && event->xclient.format == 32 &&
                event->xclient.data.l[0] == IconicState && (frm = mapping_clients.value(event->xclient.window)) != NULL)
        {
            qDebug() << "---> wm_change_state";
            frm->iconify();
            return true;
        }
        return false;
        break;

    case ColormapNotify:
        qDebug() << "[ColormapNotify]";

        if ((frm = mapping_clients.value(event->xcolormap.window)) != NULL)
        {
            qDebug() << "ColormapNotify for frame:" << frm->winId() << " - Name:" << frm->cl_name() << " - Client:" << event->xproperty.window;
            frm->set_colormaps(event->xcolormap.colormap);
            return true;
        }
        return false;
        break;

    case EnterNotify:
        qDebug() << "[EnterNotify]";

        if ((frm = mapping_frames.value(event->xcrossing.window)) != NULL)
        {
            frm->set_focus(event->xcrossing.time);
            qDebug() << "Enter in map frame:" << frm->winId();
        }
        else
        {
            XSetInputFocus(QX11Info::display(), event->xcrossing.window, RevertToParent, CurrentTime);
            qDebug() << "Enter in not map client:" << event->xcrossing.window;
        }
        return false;
        break;

    case LeaveNotify:
        qDebug() << "[LeaveNotify]";

        return false;
        break;

    case KeyPress:
        qDebug() << "[KeyPress]";

        sym = (int)XLookupKeysym(&event->xkey, 0);
        mod = event->xkey.state & keymask1 ;

        if (sym == XK_Tab && mod == keymask1)
        {
            qDebug() << "Press [Alt+Tab] - Scroll active apps";
            raise_next_frame();
        }
        return false;
        break;

    case Expose:
        qDebug() << "[Expose]";

        if (event->xexpose.window == dock->winId()) // don't cover the dockbar by other apps
            XRaiseWindow(QX11Info::display(), event->xbutton.window);

        return false;
        break;

    default:
        return false;
        break;
    }
}

void Antico::create_frame(Window c_win, Dockbar *dock) // create new frame around the client app
{
    print_window_prop(c_win);

    if (check_net_sys_tray_for(c_win))
        return;
    else
    {
        check_window_type(c_win);
        check_wm_transient_for(c_win);
    }

    /////// MAP THE NEW CLIENT ////////
    qDebug() << "Mapping window type:" << frame_type.at(0);
    Frame *frm = new Frame(c_win, frame_type.at(0), dock); // select always the first type in list (preferred)
    mapping_clients.insert(c_win, frm); // save the client winId/frame
    mapping_frames.insert(frm->winId(), frm); // save the frame winId/frame
    frame_type.clear(); // clear the window type list
    dock->add(frm); // add frame to dockbar (pager)
}

bool Antico::check_net_sys_tray_for(Window c_win)
{
    Atom type_ret = None;
    unsigned char *data = 0;
    int format = 0;
    unsigned long n = 0;
    unsigned long extra = 0;

    if (XGetWindowProperty(QX11Info::display(), c_win, _kde_net_wm_system_tray_window_for, 0, 100, False,
                           AnyPropertyType, &type_ret, &format, &n, &extra, (unsigned char **)&data) == Success && data)
    {
        qDebug() << "_KDE_NET_WM_SYSTEM_TRAY_WINDOW_FOR: WINDOW ADD TO SYSTEM TRAY";
        dock->system_tray()->add(c_win); // add to Systray
        XFree(data);
        return true;
    }
    XFree(data);
    return false;
}

void Antico::check_window_type(Window c_win) // chech the window type before mapping
{
    Atom type_ret = None;
    Atom *win_type;
    unsigned char *data = 0;
    int format = 0;
    unsigned long n = 0;
    unsigned long extra = 0;

    if (XGetWindowProperty(QX11Info::display(), c_win, _net_wm_window_type, 0, 100, False,
                           AnyPropertyType, &type_ret, &format, &n, &extra, (unsigned char **) &data) == Success && data)
    {
        win_type = (Atom *)data;

        for (unsigned i = 0; i < n; i++)
        {
            if (win_type[i] == _net_wm_window_type_normal)
            {
                frame_type << "Normal";
                qDebug() << "Window type: NORMAL TYPE";
            }
            else if (win_type[i] == _net_wm_window_type_dialog)
            {
                frame_type << "Dialog";
                qDebug() << "Window type: DIALOG TYPE";
            }
            else if (win_type[i] == _net_wm_window_type_splash)
            {
                frame_type << "Splash";
                qDebug() << "Window type: SPLASH TYPE";
            }
            else if (win_type[i] == _net_wm_window_type_desktop)
            {
                frame_type << "Desktop";
                qDebug() << "Window type: DESKTOP TYPE";
            }
            else
            {
                /// DEFAULT WINDOW TYPE ///
                frame_type << "Normal";
                qDebug() << "Window type: UNKNOWN TYPE. SET AS NORMAL";
            }
        }
    }
    /// IF PROPERY NOT SET ///
    frame_type << "Normal";
    qDebug() << "Window type: UNKNOWN TYPE. SET AS NORMAL";

    XFree(data);
}

void Antico::check_wm_transient_for(Window c_win)
{
    Atom type_ret = None;
    unsigned char *data = 0;
    int format = 0;
    unsigned long n = 0;
    unsigned long extra = 0;

    if (XGetWindowProperty(QX11Info::display(), c_win, wm_transient_for, 0, 100, False,
                           AnyPropertyType, &type_ret, &format, &n, &extra, (unsigned char **)&data) == Success && data)
    {
        frame_type << "Splash";
        qDebug() << "Window type: WM_TRANSIENT_FOR. SET AS SPLASH";
        XFree(data);
    }
}

void Antico::print_window_prop(Window c_win) // print the window properties
{
    int num_prop_ret = 0;
    Atom *list_prop = XListProperties(QX11Info::display(), c_win, &num_prop_ret);
    qDebug() << "Properties num:" << num_prop_ret;

    qDebug() << "---------------------------------------------";

    for (int i = 0; i < num_prop_ret; i++)
    {
        char *name = XGetAtomName(QX11Info::display(), list_prop[i]);
        qDebug() << i << "- Window Property:" << name;
        XFree(name);
    }

    qDebug() << "---------------------------------------------";
}

void Antico::raise_next_frame() // raise next frame on [Alt+Tab] key combination
{
    QList<Frame *> frm_list = mapping_clients.values();

    if (frm_list.size() == 0)
        return;

    if (frm_list.size() <= next_frame)
        next_frame = 0; // start from first element

    qDebug() << "Frame raise: " << frm_list.at(next_frame)->winId();
    set_active_frame(frm_list.at(next_frame));
    next_frame++;
}

void Antico::set_active_frame(Frame *frm) // activation of selected frame and unactivation of others
{
    Frame *frame;

    foreach(frame, mapping_frames)
    {
        if (frame == frm)
        {
            qDebug() << "Frame raise and active: " << frame->winId() << "- Name:" << frame->cl_name();
            frame->set_active();
            frame->raise();
        }
        else
        {
            qDebug() << "Frame inactive:"  << frame->winId() << "- Name:" << frame->cl_name();
            frame->set_inactive();
        }
    }
}

void Antico::send_configurenotify(Frame *frm)
{
    XConfigureEvent ce;

    ce.type = ConfigureNotify;
    ce.event = frm->cl_win();
    ce.window = ce.event;
    ce.x = frm->cl_x();
    ce.y = frm->cl_y();
    ce.width = frm->cl_w();
    ce.height = frm->cl_h();
    ce.above = None;
    ce.border_width = 0;
    ce.override_redirect = 0;
    XSendEvent(QX11Info::display(), ce.window, False, StructureNotifyMask, (XEvent *)&ce);
}

void Antico::wm_quit()
{
    Msgbox *msg = new Msgbox();
    msg->setText("<b>Quit the WM</b>");
    msg->setInformativeText("Are you sure to quit the WM ?");
    msg->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg->setIcon(QMessageBox::Warning);
    int ret = msg->exec();

    if (ret == QMessageBox::Ok)
    {
        Frame *frm;

        foreach(frm, mapping_clients)
        {
            qDebug() << "Quit process:" << frm->cl_win();
            XRemoveFromSaveSet(QX11Info::display(), frm->cl_win());
            XReparentWindow(QX11Info::display(), frm->cl_win(), QX11Info::appRootWindow(QX11Info::appScreen()), frm->cl_x(), frm->cl_y());
            frm->destroy();
        }
        mapping_clients.clear();
        XSync(QX11Info::display(), FALSE);
        flush();
        quit(); // quit the WM
    }
}

void Antico::wm_refresh()
{
    qDebug() << "Refreshing Antico WM ...";

    dsk->update_style(); //update desktop, all deskicons and all deskapps
    dock->update_style(); //update dockbar and all dockicons

    Frame *frm;
    foreach(frm, mapping_clients) //update all map apps
    {
        frm->update_style();
    }

    XSync(QX11Info::display(), FALSE);
    flush();
}

void Antico::shutdown()
{
    Msgbox *msg = new Msgbox();
    msg->setText("<b>Shutdown the PC</b>");
    msg->setInformativeText("Are you sure to shutdown the PC ?");
    msg->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg->setIcon(QMessageBox::Warning);
    int ret = msg->exec();

    if (ret == QMessageBox::Ok)
    {
        qDebug() << "Shutdown ...";
        QDBusConnection bus = QDBusConnection::systemBus();
        QDBusInterface hal("org.freedesktop.Hal", "/org/freedesktop/Hal/devices/computer", "org.freedesktop.Hal.Device.SystemPowerManagement", bus);
        hal.call("Shutdown");
        wm_quit();
    }
}

void Antico::restart()
{
    Msgbox *msg = new Msgbox();
    msg->setText("<b>Restart the PC</b>");
    msg->setInformativeText("Are you sure to restart the PC ?");
    msg->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg->setIcon(QMessageBox::Warning);
    int ret = msg->exec();

    if (ret == QMessageBox::Ok)
    {
        qDebug() << "Restart ...";
        QDBusConnection bus = QDBusConnection::systemBus();
        QDBusInterface hal("org.freedesktop.Hal", "/org/freedesktop/Hal/devices/computer", "org.freedesktop.Hal.Device.SystemPowerManagement", bus);
        hal.call("Reboot");
        wm_quit();
    }
}

void Antico::show_desktop()
{
    qDebug() << "Show Desktop. Iconize all the apps ...";

    Frame *frm;
    foreach(frm, mapping_clients)
    {
        if (!frm->is_iconize()) // if not yet iconize
            frm->iconify();
    }

    XSync(QX11Info::display(), FALSE);
    flush();
}

void Antico::run_app_at_startup()
{
    // default path
    QSettings *antico = new QSettings(QCoreApplication::applicationDirPath() + "/antico.cfg", QSettings::IniFormat);

    antico->beginGroup("Startup");

    for (int i = 0; i < antico->childGroups().size(); i++)
    {
        QString app_name = antico->childGroups().value(i);
        antico->beginGroup(app_name); // App group
        QString name = antico->value("name").toString();
        QString path = antico->value("path").toString();
        QProcess::startDetached(path + name); // run the application
        antico->endGroup(); // Name
    }
    antico->endGroup(); // Startup
}

void Antico::set_settings()
{
    // default path
    QSettings *antico = new QSettings(QCoreApplication::applicationDirPath() + "/antico.cfg", QSettings::IniFormat);
    // set default style on first installation, if no "/antico.cfg" is set
    if (antico->childGroups().isEmpty())
    {
        qDebug() << "Set default settings ...";
        antico->beginGroup("Style");
        antico->setValue("name", "default.stl");
        antico->setValue("path", QCoreApplication::applicationDirPath() + "/theme/default/");
        antico->endGroup(); //Style
        antico->sync();
    }
    if (antico->status() == QSettings::AccessError)
        qDebug () << "Error on setting antico.cfg";

    QSettings *style = new QSettings(QCoreApplication::applicationDirPath() + "/theme/default/default.stl", QSettings::IniFormat);
    // set default icon on first installation, if no "/default.stl" is set
    if (style->childGroups().isEmpty())
    {
        qDebug() << "Set style settings ...";
        /////////////////////////////////////////////////////////////////////////
        style->beginGroup("Launcher");
        style->beginGroup("Icon");
        style->setValue("launcher_pix", "antico.png");
        style->setValue("application_pix", "application.png");
        style->setValue("quit_pix", "quit.png");
        style->setValue("shutdown_pix", "shutdown.png");
        style->setValue("restart_pix", "restart.png");
        style->setValue("refresh_pix", "refresh.png");
        style->setValue("show_pix", "show.png");
        style->setValue("run_pix", "run.png");
        style->setValue("manager_pix", "manager.png");
        style->setValue("utility_pix", "utility.png");
        style->setValue("office_pix", "office.png");
        style->setValue("internet_pix", "internet.png");
        style->setValue("graphic_pix", "graphic.png");
        style->setValue("development_pix", "development.png");
        style->setValue("system_pix", "system.png");
        style->setValue("audio_pix", "audio.png");
        style->setValue("video_pix", "video.png");
        style->endGroup(); //Icon
        style->endGroup(); //Launcher
        /////////////////////////////////////////////////////////////////////////
        style->beginGroup("Frame");
        style->beginGroup("Border");
        style->setValue("lateral_bdr_width", 3);
        style->setValue("top_bdr_height", 16);
        style->setValue("bottom_bdr_height", 8);
        style->endGroup(); //Border
        style->beginGroup("Header");
        style->setValue("header_active_pix", "header_active.png");
        style->setValue("header_inactive_pix", "header_inactive.png");
        style->setValue("title_color", QColor(Qt::white));
        style->setValue("minmax_pix", "minmax.png");
        style->setValue("close_pix", "close.png");
        style->endGroup(); //Header
        style->endGroup(); //Frame
        /////////////////////////////////////////////////////////////////////////
        style->beginGroup("Desktop");
        style->beginGroup("Wallpaper");
        style->setValue("wall_pix", "wallpaper.png");
        style->endGroup(); //Wallpaper
        style->endGroup(); //Desktop
        /////////////////////////////////////////////////////////////////////////
        style->beginGroup("Dockbar");
        style->setValue("dock_pix", "dockbar.png");
        style->setValue("dock_height", 40);
        style->endGroup(); //Dockbar
        /////////////////////////////////////////////////////////////////////////
        style->beginGroup("Dockicon");
        style->setValue("d_icon_pix", "dockicon.png");
        style->setValue("title_color", QColor(Qt::white));
        style->endGroup(); //Dockicon
        /////////////////////////////////////////////////////////////////////////
        style->beginGroup("Sysicon");
        style->setValue("s_icon_pix", "sysicon.png");
        style->endGroup(); //Sysicon
        /////////////////////////////////////////////////////////////////////////
        style->beginGroup("Deskfolder");
        style->setValue("d_folder_pix", "deskfolder.png");
        style->setValue("name_color", QColor(Qt::white));
        style->endGroup(); //Deskfolder
        /////////////////////////////////////////////////////////////////////////
        style->beginGroup("Deskfile");
        style->setValue("name_color", QColor(Qt::white));
        style->endGroup(); //Deskfile
        /////////////////////////////////////////////////////////////////////////
        style->beginGroup("Deskapp");
        style->setValue("name_color", QColor(Qt::white));
        style->endGroup(); //Deskapp
        /////////////////////////////////////////////////////////////////////////
        style->beginGroup("Deskdev");
        style->setValue("d_disk_pix", "disk.png");
        style->setValue("d_cdrom_pix", "cdrom.png");
        style->setValue("name_color", QColor(Qt::white));
        style->endGroup(); //Deskapp
        /////////////////////////////////////////////////////////////////////////
        style->beginGroup("Dateclock");
        style->setValue("date_color", QColor(Qt::white));
        style->setValue("clock_color", QColor(Qt::white));
        style->endGroup(); //DateClock
        /////////////////////////////////////////////////////////////////////////
        style->beginGroup("Other");
        style->setValue("folder_link_pix", "folder_link.png");
        style->setValue("file_link_pix", "file_link.png");
        style->setValue("app_link_pix", "app_link.png");
        style->setValue("delete_link_pix", "delete_link.png");
        style->setValue("delete_file_pix", "delete_file.png");
        style->setValue("close_dock_pix", "close_dock.png");
        style->setValue("add_to_sys_pix", "add_to_sys.png");
        style->setValue("open_with_pix", "open_with.png");
        style->endGroup(); //Other
        /////////////////////////////////////////////////////////////////////////
        style->sync();
    }
    if (style->status() == QSettings::AccessError)
        qDebug () << "Error on setting default.stl";

}
