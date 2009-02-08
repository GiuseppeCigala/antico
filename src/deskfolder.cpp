////////////////////////////////////////
//  File      : deskfolder.cpp        //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "deskfolder.h"

////////////////////////////////////////

Deskfolder::Deskfolder(Filedialog *dial, Categorymenu *menu, const QString &dir_nm, const QString &dir_pth, const QRect &geom, QWidget *parent) : QWidget(parent)
{
    file_dialog = dial;
    cat_menu = menu;
    read_settings();
    init();
    dir_name = dir_nm;
    dir_path = dir_pth;
    geometry = geom;
    show();
}

Deskfolder::~Deskfolder()
{
    delete file_dialog;
    delete cat_menu;
    delete &open_menu;
    delete &dir_name;
    delete &dir_path;
    delete &geometry;
    delete &d_folder_pix;
    delete &d_folder_col;
    delete &geometry;
    delete &delete_link_pix;
    delete &open_with_pix;
    delete &pix;
}

void Deskfolder::init()
{
    setFixedSize(100, 50);
    zoom = false;
    setToolTip(dir_path);
    main_menu = new QMenu(this);
    // show the Category apps list for open the file
    open_menu = main_menu->addMenu(QIcon(open_with_pix), tr("Open with"));
    
    QList <QMenu *> menu_list = cat_menu->get_menus();
    for (int i = 0; i <  menu_list.size(); ++i)
    {
        open_menu->addMenu(menu_list.at(i));
    }
   
    QAction *del_folder = main_menu->addAction(QIcon(delete_link_pix), tr("Delete link"));
    connect(del_folder, SIGNAL(triggered()), this, SLOT(del_folder()));
}

void Deskfolder::read_settings()
{
    // get style path
    antico = new QSettings(QCoreApplication::applicationDirPath() + "/antico.cfg", QSettings::IniFormat, this);
    antico->beginGroup("Style");
    QString stl_name = antico->value("name").toString();
    QString stl_path = antico->value("path").toString();
    antico->endGroup(); //Style
    // get style values
    style = new QSettings(stl_path + stl_name, QSettings::IniFormat, this);
    style->beginGroup("Deskfolder");
    d_folder_pix = stl_path + style->value("d_folder_pix").toString();
    d_folder_col = style->value("name_color").value<QColor>();
    geometry = style->value("geometry").value<QRect>();
    style->endGroup(); //Deskfolder
    style->beginGroup("Other");
    delete_link_pix = stl_path + style->value("delete_link_pix").toString();
    open_with_pix = stl_path + style->value("open_with_pix").toString();
    style->endGroup(); //Other
    pix = QPixmap(d_folder_pix); // set deskfolder pixmap
}

void Deskfolder::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setWindow(-50, -50, 100, 50);
    if (zoom)
    {
        painter.drawPixmap(QRect(-18, -50, 36, 36), pix, QRect(0, 0, pix.width(), pix.height()));// deskfolder pix
    }
    else
    {
        painter.drawPixmap(QRect(-16, -50, 32, 32), pix, QRect(0, 0, pix.width(), pix.height()));// deskfolder pix
    }
    painter.setPen(d_folder_col);
    painter.drawText(-50, -15, 100, 20, Qt::AlignHCenter, dir_name); // deskfolder name
}

void Deskfolder::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mousepos = event->pos();
        grabMouse(QCursor(Qt::SizeAllCursor));
    }
}

void Deskfolder::mouseMoveEvent(QMouseEvent *event)
{
    QPoint p = event->globalPos()-mousepos;
    move(p.x(), p.y());
}

void Deskfolder::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    releaseMouse();
    // save new deskfolder position on desk writing on antico.cfg
    antico->beginGroup("Desktop");
    antico->beginGroup("Folder");
    antico->beginGroup(dir_name); // Icon name
    antico->setValue("pos", pos());
    antico->endGroup(); // Folder name
    antico->endGroup(); // Folder
    antico->endGroup(); // Desktop
}

void Deskfolder::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        file_dialog->set_type(tr("Folder contents:"), "Close");
        file_dialog->setGeometry(geometry);
        file_dialog->set_path(dir_path);

        if (file_dialog->exec() == QDialog::Rejected) // on close button
        {
            // save deskicon geometry
            antico->beginGroup("Desktop");
            antico->beginGroup("Folder");
            antico->beginGroup(dir_name);
            antico->setValue("geometry", file_dialog->geometry());
            antico->endGroup(); //name
            antico->endGroup(); //Folder
            antico->endGroup(); //Desktop
            geometry = file_dialog->geometry(); // update geometry
        }
    }
}

void Deskfolder::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    zoom = true;
    update();
}

void Deskfolder::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    zoom = false;
    update();
}

void Deskfolder::contextMenuEvent(QContextMenuEvent *event)
{
    cat_menu->set_cmd_arguments(dir_path); // set the dir path as argument
    main_menu->exec(event->globalPos());
}

void Deskfolder::del_folder()
{
    emit destroy_deskfolder(this);
    // remove the deskfolder from desk and from antico.cfg
    antico->beginGroup("Desktop");
    antico->beginGroup("Folder");
    antico->remove(dir_name);
    antico->endGroup(); // Folder
    antico->endGroup(); // Desktop
}

void Deskfolder::update_style()
{
    read_settings();
    update();
}