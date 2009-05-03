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
    ok_button_pix_path = stl_path + style->value("ok_button_pix").toString();
    close_button_pix_path = stl_path + style->value("close_button_pix").toString();
    style->endGroup(); //Message
    style->beginGroup("Launcher");
    system_pix_path = stl_path + style->value("system_pix").toString();
    utility_pix_path = stl_path + style->value("utility_pix").toString();
    style->endGroup(); //Launcher
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
    QPushButton *ok_but = new QPushButton(QIcon(ok_button_pix_path), tr("Ok"), this);
    QPushButton *close_but = new QPushButton(QIcon(close_button_pix_path), tr("Close"), this);
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
    system_tab();
}

void Settings::display_tab()
{
    QFrame *display_frm = new QFrame(this);
    tab->addTab(display_frm, QIcon(utility_pix_path), tr("Display"));
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
    hor_res_spin->setRange(0, 200);
    QLabel *ver_res = new QLabel(this);
    ver_res_spin = new QSpinBox(this);
    ver_res_spin->setRange(0, 200);
    composite_set->setReadOnly(true);
    virtual_desk_set->setReadOnly(true);
    display_num->setReadOnly(true);
    screen_num->setReadOnly(true);
    scr_width_val->setReadOnly(true);
    scr_height_val->setReadOnly(true);
    color_dph_num->setReadOnly(true);
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

void Settings::system_tab()
{
    QFrame *system_frm = new QFrame(this);
    tab->addTab(system_frm, QIcon(system_pix_path), tr("System"));
    QGridLayout *system_layout = new QGridLayout();
    system_frm->setLayout(system_layout);
    QLabel *cursor_flash = new QLabel(this);
    cursor_flash_spin = new QSpinBox(this);
    cursor_flash_spin->setRange(0, 2000); // default = 1000 milliseconds
    QLabel *double_click = new QLabel(this);
    double_click_spin = new QSpinBox(this);
    double_click_spin->setRange(0, 1000); // default = 400 milliseconds
    QLabel *keyboard_input = new QLabel(this);
    keyboard_input_spin = new QSpinBox(this);
    keyboard_input_spin->setRange(0, 1000); // default = 400 milliseconds
    QLabel *wheel_scroll = new QLabel(this);
    wheel_scroll_spin = new QSpinBox(this);
    wheel_scroll_spin->setRange(0, 20); // default = 3 lines
    cursor_flash->setText(tr("Cursor flash time:"));
    double_click->setText(tr("Double click interval:"));
    keyboard_input->setText(tr("Keyboard input interval:"));
    wheel_scroll->setText(tr("Wheel scroll lines:"));
    cursor_flash_spin->setValue(qApp->cursorFlashTime());
    double_click_spin->setValue(qApp->doubleClickInterval());
    keyboard_input_spin->setValue(qApp->keyboardInputInterval());
    wheel_scroll_spin->setValue(qApp->wheelScrollLines());

    system_layout->addWidget(cursor_flash, 0, 0);
    system_layout->addWidget(cursor_flash_spin, 0, 1);
    system_layout->addWidget(double_click, 1, 0);
    system_layout->addWidget(double_click_spin, 1, 1);
    system_layout->addWidget(keyboard_input, 2, 0);
    system_layout->addWidget(keyboard_input_spin, 2, 1);
    system_layout->addWidget(wheel_scroll, 3, 0);
    system_layout->addWidget(wheel_scroll_spin, 3, 1);
}

void Settings::ok_pressed()
{
    QX11Info::setAppDpiX(scr_num, hor_res_spin->value());
    QX11Info::setAppDpiY(scr_num, ver_res_spin->value());
    qApp->setCursorFlashTime(cursor_flash_spin->value());
    qApp->setDoubleClickInterval(double_click_spin->value());
    qApp->setKeyboardInputInterval(keyboard_input_spin->value());
    qApp->setWheelScrollLines(wheel_scroll_spin->value());
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