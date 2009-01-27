////////////////////////////////////////
//  File      : deskdev.cpp           //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "deskdev.h"

////////////////////////////////////////

Deskdev::Deskdev(const QString &device, const QString &mnt_path, const QString &label, const QString &type, QWidget *parent) : QLabel(parent)
{
    device_name = device;
    mount_path = mnt_path;
    device_label = label;
    device_type = type;
    setFixedSize(100, 50);
    setToolTip(tr("Mount path: ") + mnt_path);
    read_settings();
    init();
    show();
}

Deskdev::~Deskdev()
{}

void Deskdev::read_settings()
{
    // get style path
    antico = new QSettings(QCoreApplication::applicationDirPath() + "/antico.cfg", QSettings::IniFormat, this);
    antico->beginGroup("Style");
    QString stl_name = antico->value("name").toString();
    QString stl_path = antico->value("path").toString();
    antico->endGroup(); //Style
    // get style values
    style = new QSettings(stl_path + stl_name, QSettings::IniFormat, this);
    style->beginGroup("Deskdev");
    d_disk_pix = stl_path + style->value("d_disk_pix").toString();
    d_cdrom_pix = stl_path + style->value("d_cdrom_pix").toString();
    d_dev_col = style->value("name_color").value<QColor>();
    style->endGroup(); //Deskdev
}

void Deskdev::init()
{ 
    if (device_type == "disk") // select the right pixmap (cdrom-disk)
        dev_pix = QPixmap(d_disk_pix);
    else // is cdrom
        dev_pix = QPixmap(d_cdrom_pix);
}

void Deskdev::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setWindow(-50, -50, 100, 50);
    painter.drawPixmap(QRect(-16, -50, 32, 32), dev_pix, QRect(0, 0, dev_pix.width(), dev_pix.height()));// deskdev pix
    painter.setPen(d_dev_col);
    painter.drawText(-50, -15, 100, 20, Qt::AlignHCenter|Qt::TextWordWrap, device_name); // deskdev name
}

void Deskdev::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mousepos = event->pos();
        grabMouse(QCursor(Qt::SizeAllCursor));
    }
}

void Deskdev::mouseMoveEvent(QMouseEvent *event)
{
    QPoint p = event->globalPos()-mousepos;
    move(p.x(), p.y());
}

void Deskdev::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    releaseMouse();
}

void Deskdev::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        Filedialog *open_dir = new Filedialog(tr("Device contents:"), "Close", this);
        open_dir->set_filter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
        open_dir->set_path(mount_path);
    }
}

void Deskdev::update_style()
{
    read_settings();
    update();
}

