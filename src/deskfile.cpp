////////////////////////////////////////
//  File      : deskfile.cpp          //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "deskfile.h"

////////////////////////////////////////

Deskfile::Deskfile(const QString &file_nm, const QString &file_pth, QString pix, QWidget *parent) : QLabel(parent)
{
    file_name = file_nm;
    file_path = file_pth;
    d_file_pix = QPixmap(pix);
    read_settings();
    init();
    show();
}

Deskfile::~Deskfile()
{}

void Deskfile::init()
{
    setFixedSize(100, 50);
    setToolTip(file_path + file_name);
    main_menu = new QMenu(this);
    // show the Category apps list for open the file
    open_menu = main_menu->addMenu(QIcon(open_with_pix), "Open with");
    cat_menu = new Categorymenu(open_menu);
    QAction *del_file = main_menu->addAction(QIcon(delete_link_pix), "Delete");
    connect(del_file, SIGNAL(triggered()), this, SLOT(del_file()));
}

void Deskfile::read_settings()
{
    // get style path
    antico = new QSettings(QCoreApplication::applicationDirPath() + "/antico.cfg", QSettings::IniFormat, this);
    antico->beginGroup("Style");
    QString stl_name = antico->value("name").toString();
    QString stl_path = antico->value("path").toString();
    antico->endGroup(); //Style
    // get style values
    style = new QSettings(stl_path + stl_name, QSettings::IniFormat, this);
    style->beginGroup("Deskfile");
    d_file_col = style->value("name_color").value<QColor>();
    style->endGroup(); //Deskfile
    style->beginGroup("Other");
    delete_link_pix = stl_path + style->value("delete_link_pix").toString();
    open_with_pix = stl_path + style->value("open_with_pix").toString();
    style->endGroup(); //Other
}

void Deskfile::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setWindow(-50, -50, 100, 50);
    painter.drawPixmap(QRect(-16, -50, 32, 32), d_file_pix, QRect(0, 0, d_file_pix.width(), d_file_pix.height()));// deskfile pix
    painter.setPen(d_file_col);
    painter.drawText(-50, -15, 100, 20, Qt::AlignHCenter, file_name); // deskfile name
}

void Deskfile::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mousepos = event->pos();
        grabMouse(QCursor(Qt::SizeAllCursor));
    }
}

void Deskfile::mouseMoveEvent(QMouseEvent *event)
{
    QPoint p = event->globalPos()-mousepos;
    move(p.x(), p.y());
}

void Deskfile::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    releaseMouse();
    // save new deskfile position on desk writing on antico.cfg
    antico->beginGroup("Desktop");
    antico->beginGroup("File");
    antico->beginGroup(file_name); // File name
    antico->setValue("pos", pos());
    antico->endGroup(); // File name
    antico->endGroup(); // File
    antico->endGroup(); // Desktop
}

void Deskfile::del_file()
{
    // remove the deskicon from desk and from antico.cfg
    antico->beginGroup("Desktop");
    antico->beginGroup("File");
    antico->remove(file_name);
    antico->endGroup(); // File
    antico->endGroup(); // Desktop
    close();
}

void Deskfile::contextMenuEvent(QContextMenuEvent *event)
{
    cat_menu->set_cmd_arguments(file_path + file_name); // set the file path+name as argument
    main_menu->exec(event->globalPos());
}

void Deskfile::update_style()
{
    read_settings();
    update();
}