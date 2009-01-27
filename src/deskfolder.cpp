////////////////////////////////////////
//  File      : deskfolder.cpp        //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "deskfolder.h"

////////////////////////////////////////

Deskfolder::Deskfolder(const QString &dir_nm, const QString &dir_pth, const QRect &geom, QWidget *parent) : QLabel(parent)
{
    setFixedSize(100, 50);
    read_settings();
    dir_name = dir_nm;
    dir_path = dir_pth;
    geometry = geom;
    setToolTip(dir_path);
    show();
}

Deskfolder::~Deskfolder()
{}

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
    style->endGroup(); //Other
    pix = QPixmap(d_folder_pix); // set deskfolder pixmap
}

void Deskfolder::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setWindow(-50, -50, 100, 50);
    painter.drawPixmap(QRect(-16, -50, 32, 32), pix, QRect(0, 0, pix.width(), pix.height()));// deskfolder pix
    painter.setPen(d_folder_col);
    painter.drawText(-50, -15, 100, 20, Qt::AlignHCenter, dir_name); // deskfolder name
}

void Deskfolder::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        menu = new QMenu(this);
        QAction *del_folder = menu->addAction(QIcon(delete_link_pix), tr("Delete link"));
        menu->popup(event->globalPos());
        connect(del_folder, SIGNAL(triggered()), this, SLOT(del_folder()));
    }
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
        Filedialog *open_dir = new Filedialog(tr("Folder contents:"), "Close", this);
        open_dir->set_filter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
        open_dir->setGeometry(geometry);
        open_dir->set_path(dir_path);

        if (open_dir->exec() == QDialog::Rejected) // on close button
        {
            // save deskicon geometry
            antico->beginGroup("Desktop");
            antico->beginGroup("Folder");
            antico->beginGroup(dir_name);
            antico->setValue("geometry", open_dir->geometry());
            antico->endGroup(); //name
            antico->endGroup(); //Folder
            antico->endGroup(); //Desktop
            geometry = open_dir->geometry(); // update geometry
        }
    }
}

void Deskfolder::del_folder()
{
    // remove the deskfolder from desk and from antico.cfg
    antico->beginGroup("Desktop");
    antico->beginGroup("Folder");
    antico->remove(dir_name);
    antico->endGroup(); // Folder
    antico->endGroup(); // Desktop
    close();
}

void Deskfolder::update_style()
{
    read_settings();
    update();
}