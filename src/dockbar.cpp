////////////////////////////////////////
//  File      : dockbar.cpp           //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "dockbar.h"

////////////////////////////////////////

Dockbar::Dockbar(Antico *a, QWidget *parent) : QLabel(parent)
{
    app = a;
    dock_layout = new QHBoxLayout(this);
    setLayout(dock_layout);
    dock_layout->setContentsMargins(0, 0, 0, 0);
    dock_layout->setSpacing(1);
    dock_layout->setSizeConstraint(QLayout::SetNoConstraint);

    read_settings();
    set_geometry();

    // add launcher to dockbar
    lchr = new Launcher(a, this);
    lchr->setFixedSize(dock_height-1, dock_height-1);
    // add systray to dockbar
    sys = new Systray(this);
    sys->setFixedSize(dock_height*6, dock_height-1);
    // add clock to dockbar
    clk = new Dateclock(this);
    clk->setFixedSize(dock_height*2, dock_height-1);
    // for set dockicon on dockbar
    d_frame = new QFrame(this);
    d_frame->setFixedSize(QApplication::desktop()->width()-lchr->width()-sys->width()-clk->width(), dock_height-1);

    icon_layout = new QHBoxLayout();
    d_frame->setLayout(icon_layout);
    icon_layout->setAlignment(Qt::AlignLeft);
    icon_layout->setContentsMargins(0, 0, 0, 0);
    icon_layout->setSpacing(1);
    dock_layout->insertWidget(0, lchr);
    dock_layout->insertWidget(1, d_frame);
    dock_layout->insertSpacing(2, 50);
    dock_layout->insertWidget(3, sys);
    dock_layout->insertWidget(4, clk);

    show();
}

Dockbar::~Dockbar()
{
    delete app;
    delete icon_layout;
    delete d_icon;
    delete &dock_icons;
    delete &dock_pix;
    delete &dock_height;
}

void Dockbar::read_settings()
{
    // get style path
    antico = new QSettings(QCoreApplication::applicationDirPath() + "/antico.cfg", QSettings::IniFormat, this);
    antico->beginGroup("Style");
    QString stl_name = antico->value("name").toString();
    QString stl_path = antico->value("path").toString();
    antico->endGroup(); //Style
    // get style values
    style = new QSettings(stl_path + stl_name, QSettings::IniFormat, this);
    style->beginGroup("Dockbar");
    dock_pix = stl_path + style->value("dock_pix").toString();
    dock_height = style->value("dock_height").toInt();
    style->endGroup(); //Dockbar
}

void Dockbar::set_geometry()
{
    setPixmap(dock_pix);
    setScaledContents(true);
    setGeometry(0, QApplication::desktop()->height()-dock_height, QApplication::desktop()->width(), dock_height);
}

void Dockbar::update_dockicon_name(const QString &name, Frame *frm)
{
     if (dock_icons.contains(frm->winId())) // if already present
     {
         Dockicon *d_icon = dock_icons.value(frm->winId());
         d_icon->update_name(name);
     }
}

void Dockbar::add(Frame *frm)
{
    if (! dock_icons.contains(frm->winId())) // if not already present
    {
        d_icon = new Dockicon(frm, sys);
        dock_icons.insert(frm->winId(), d_icon); // save the Frame winId/Dockicon
        qDebug() << "Dockicons added to Dockbar. Frame:" << frm->winId();
        icon_layout->addWidget(d_icon);
        update_size();
        connect(d_icon, SIGNAL(show_dock(Dockicon *)), SLOT(remove(Dockicon *))); // show iconize dockicon and update dockbar size
        connect(d_icon, SIGNAL(destroy_dock(Dockicon *)), SLOT(remove(Dockicon *))); // delete iconize dockicon and update dockbar size
    }
}

void Dockbar::remove(Dockicon *d_icon) // remove from "Close" right button mouse on Dockbar
{
    dock_icons.remove(dock_icons.key(d_icon));
    qDebug() << "Dockicon remove. Num. after deletion:" << dock_icons.size();
    d_icon->close();
    update_size();
}

void Dockbar::remove(Window win_id) //remove from "Close" cmd on Systray (_NET_SYSTEM_TRAY protocol) if Dockicon is still mapped
{
    if (dock_icons.contains(win_id))
    {
        Dockicon *d_icon = dock_icons.value(win_id);
        dock_icons.remove(win_id);
        qDebug() << "Dockicon remove from Systray cmd. Num. after deletion:" << dock_icons.size();
        d_icon->close();
        update_size();
    }
    else
    {
        sys->remove(win_id); // check in the System Tray eventually Sysicon still mapped
    }
}

void Dockbar::update_size()
{
    if (! dock_icons.isEmpty())
    {
        int num = dock_icons.size();
        qDebug() << "Dockicon num:" << num;
        d_length = d_frame->width()/num;

        if (d_length >= d_frame->width()/4) // max dockicon size = d_frame size/4
            d_length = d_frame->width()/4;

        qDebug() << "Dockicon length:" << d_length;

        foreach(Dockicon *icon, dock_icons)
        {
            icon->setFixedSize(d_length-2, dock_height-5);
            icon_layout->addWidget(icon);
        }
    }
}

void Dockbar::update_style()
{
    read_settings();
    set_geometry();
    update_size();

    // update dockicons
    foreach(Dockicon *icon, dock_icons)
    icon->update_style();
    // update sysicons on systray
    sys->update_style();
    sys->setFixedSize(dock_height*6, dock_height-1);
    // update launcher
    lchr->update_style();
    lchr->setFixedSize(dock_height-1, dock_height-1);
    // update dateclock
    clk->update_style();
    clk->setFixedSize(dock_height*2, dock_height-1);
    // update dock frame
    d_frame->setFixedSize(QApplication::desktop()->width()-lchr->width()-sys->width()-clk->width(), dock_height-1);
}