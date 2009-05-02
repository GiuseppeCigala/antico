////////////////////////////////////////
//  File      : settings.cpp          //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "settings.h"

////////////////////////////////////////

Settings::Settings(QWidget *parent) : QDialog(parent)
{
    setSizeGripEnabled(true);
    setWindowModality(Qt::WindowModal);
    read_settings();
    init();
    show();
}

Settings::~Settings()
{}

void Settings::read_settings()
{
    // get style path
    QSettings *antico = new QSettings(QCoreApplication::applicationDirPath() + "/antico.cfg", QSettings::IniFormat, this);
    antico->beginGroup("Style");
    stl_name = antico->value("name").toString();
    stl_path = antico->value("path").toString();
    antico->endGroup(); //Style
    // get style values
    QSettings *style = new QSettings(stl_path + stl_name, QSettings::IniFormat, this);
    style->beginGroup("Message");
    ok_button_pix = stl_path + style->value("ok_button_pix").toString();
    close_button_pix = stl_path + style->value("close_button_pix").toString();
    style->endGroup(); //Message
}

void Settings::paintEvent(QPaintEvent *)
{
    QPixmap pixmap(size());
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.setBrush(Qt::black);
    painter.drawRoundRect(pixmap.rect(), 5, 5);
    setMask(pixmap.createMaskFromColor(Qt::white));

    QPainter painter1(this);
    painter1.setRenderHint(QPainter::Antialiasing);
    painter1.setPen(QPen(Qt::darkGray, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter1.drawRoundedRect(0, 0, width(), height(), 5, 5, Qt::RelativeSize);
}

void Settings::init()
{
    tab = new QTabWidget(this);
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    setLayout(main_layout);
    QLabel *lab = new QLabel(tr("SETTINGS"), this);
    lab->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);

    QGroupBox *ok_close_box = new QGroupBox(this);
    QHBoxLayout *ok_close_layout = new QHBoxLayout();
    ok_close_box->setLayout(ok_close_layout);
    QPushButton *ok_but = new QPushButton(QIcon(QPixmap(ok_button_pix)), tr("Ok"), this);
    QPushButton* close_but = new QPushButton(QIcon(QPixmap(close_button_pix)), tr("Close"), this);
    ok_but->setDefault(true);
    connect(ok_but, SIGNAL(clicked()), this, SLOT(ok_pressed()));
    connect(close_but, SIGNAL(clicked()), this, SLOT(close_pressed()));
    ok_close_layout->addStretch(1);
    ok_close_layout->addWidget(ok_but);
    ok_close_layout->addWidget(close_but);

    main_layout->addWidget(lab);
    main_layout->addWidget(tab);
    main_layout->addWidget(ok_close_box);

    display_tab();
}

void Settings::display_tab()
{
    QFrame *display_frm = new QFrame(this);
    tab->addTab(display_frm, tr("Display"));
    QGridLayout *display_layout = new QGridLayout();
    display_frm->setLayout(display_layout);
    QLabel *composite = new QLabel(this);
    QLineEdit *composite_set = new QLineEdit(this);
    QLabel *virtual_desk = new QLabel(this);
    QLineEdit *virtual_desk_set = new QLineEdit(this);
    QLabel *display = new QLabel(this);
    QLineEdit *display_num = new QLineEdit(this);
    QLabel *screen = new QLabel(this);
    QLineEdit *screen_num = new QLineEdit(this);
    QLabel *scr_width = new QLabel(this);
    QLineEdit *scr_width_val = new QLineEdit(this);
    QLabel *scr_height = new QLabel(this);
    QLineEdit *scr_height_val = new QLineEdit(this);
    QLabel *color_dph = new QLabel(this);
    QLineEdit *color_dph_num = new QLineEdit(this);
    QLabel *hor_res = new QLabel(this);
    hor_res_spin = new QSpinBox(this);
    QLabel *ver_res = new QLabel(this);
    ver_res_spin = new QSpinBox(this);
    composite_set->setReadOnly(true);
    virtual_desk_set->setReadOnly(true);
    display_num->setReadOnly(true);
    screen_num->setReadOnly(true);
    scr_width_val->setReadOnly(true);
    scr_height_val->setReadOnly(true);
    color_dph_num->setReadOnly(true);
    composite->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    virtual_desk->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    display->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    screen->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    scr_width->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    scr_height->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    color_dph->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    hor_res->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    ver_res->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    composite->setText(tr("Composite:"));
    virtual_desk->setText(tr("Virtual Desktop:"));
    display->setText(tr("Display:"));
    screen->setText(tr("Screen num:"));
    scr_width->setText(tr("Screen width:"));
    scr_height->setText(tr("Screen height:"));
    color_dph->setText(tr("Color depth:"));
    hor_res->setText(tr("Horizontal resol:"));
    ver_res->setText(tr("Vertical resol:"));
    scr_num = QX11Info::appScreen();
    QDesktopWidget *desktop = QApplication::desktop();
    QRect geometry = desktop->screenGeometry(scr_num);
    int depth = QX11Info::appDepth(scr_num);
    display_num->setText(QString("%1").arg(XDisplayName(NULL)));
    screen_num->setText(QString("%1").arg(scr_num));
    scr_width_val->setText(QString("%1").arg(geometry.width()));
    scr_height_val->setText(QString("%1").arg(geometry.height()));
    color_dph_num->setText(QString("%1").arg(depth));
    hor_res_spin->setValue(QX11Info::appDpiX(scr_num));
    ver_res_spin->setValue(QX11Info::appDpiY(scr_num));

    if (QX11Info::isCompositingManagerRunning()) // true
        composite_set->setText(tr("True"));
    else
        composite_set->setText(tr("False"));
    if (desktop->isVirtualDesktop()) // true
        virtual_desk_set->setText(tr("True"));
    else
        virtual_desk_set->setText(tr("False"));

    display_layout->addWidget(composite, 0, 0);
    display_layout->addWidget(composite_set, 0, 1);
    display_layout->addWidget(virtual_desk, 1, 0);
    display_layout->addWidget(virtual_desk_set, 1, 1);
    display_layout->addWidget(display, 2, 0);
    display_layout->addWidget(display_num, 2, 1);
    display_layout->addWidget(screen, 3, 0);
    display_layout->addWidget(screen_num, 3, 1);
    display_layout->addWidget(scr_width, 4, 0);
    display_layout->addWidget(scr_width_val, 4, 1);
    display_layout->addWidget(scr_height, 5, 0);
    display_layout->addWidget(scr_height_val, 5, 1);
    display_layout->addWidget(color_dph, 6, 0);
    display_layout->addWidget(color_dph_num, 6, 1);
    display_layout->addWidget(hor_res, 7, 0);
    display_layout->addWidget(hor_res_spin, 7, 1);
    display_layout->addWidget(ver_res, 8, 0);
    display_layout->addWidget(ver_res_spin, 8, 1);
}

void Settings::ok_pressed()
{
    QX11Info::setAppDpiX(scr_num, hor_res_spin->value());
    QX11Info::setAppDpiY(scr_num, ver_res_spin->value());
}

void Settings::close_pressed()
{
    close();
}

void Settings::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mousepos = event->pos();
        grabMouse(QCursor(Qt::SizeAllCursor));
        raise();
    }
}

void Settings::mouseMoveEvent(QMouseEvent *event)
{
    QPoint p = event->globalPos()-mousepos;
    move(p.x(), p.y());
}

void Settings::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    releaseMouse();
}