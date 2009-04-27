////////////////////////////////////////
//  File      : deskicon.cpp          //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "deskicon.h"

////////////////////////////////////////

Deskicon::Deskicon(Frame *frame, QWidget *parent) : QLabel(parent)
{
    frm = frame;
    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    grab_pix = QPixmap::grabWindow(frm->winId(), 0, 0, frm->width(), frm->height());
    setPixmap(grab_pix.scaled(frm->width()/4, frm->height()/4, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setToolTip(frm->cl_name());
    read_settings();
    show();
}

Deskicon::~Deskicon()
{
    delete frm;
}

void Deskicon::read_settings()
{
    // get style path
    QSettings *antico = new QSettings(QCoreApplication::applicationDirPath() + "/antico.cfg", QSettings::IniFormat, this);
    antico->beginGroup("Style");
    QString stl_name = antico->value("name").toString();
    QString stl_path = antico->value("path").toString();
    antico->endGroup(); //Style
    // get style values
    QSettings *style = new QSettings(stl_path + stl_name, QSettings::IniFormat, this);
    style->beginGroup("Other");
    close_desk_pix = stl_path + style->value("close_dock_pix").toString();
    style->endGroup(); //Other
}

void Deskicon::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        frm->raise();
        emit destroy_deskicon(this);
    }
}

void Deskicon::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mousepos = event->pos();
        grabMouse(QCursor(Qt::SizeAllCursor));
    }
    if (event->button() == Qt::RightButton)
    {
        QMenu *menu = new QMenu(this);
        menu->addAction(QIcon(close_desk_pix), tr("Close"));
        menu->popup(event->globalPos());
        connect(menu, SIGNAL(triggered(QAction *)), this, SLOT(run_menu(QAction *)));
    }
}

void Deskicon::mouseMoveEvent(QMouseEvent *event)
{
    QPoint p = event->globalPos()-mousepos;
    move(p.x(), p.y());
}

void Deskicon::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    setGeometry(x()-width()/24, y()-height()/24, width()+width()/12, height()+height()/12);
    setPixmap(grab_pix.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Deskicon::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    setGeometry(x()+width()/26, y()+height()/26, width()-width()/13, height()-height()/13);
    setPixmap(grab_pix.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Deskicon::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    releaseMouse();
}

void Deskicon::run_menu(QAction *act)
{
    if (act->text() == tr("Close"))
    {
        emit destroy_deskicon(this);
        frm->destroy();
        close();
    }
}