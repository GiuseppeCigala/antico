////////////////////////////////////////
//  File      : launcher.cpp          //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "launcher.h"

////////////////////////////////////////

Launcher::Launcher(Antico *a, QWidget *parent) : QLabel(parent)
{
    app = a;
    read_settings();
    init();
    setPixmap(QPixmap(launcher_pix));
    setScaledContents(true);
    show();
}

Launcher::~Launcher()
{}

void Launcher::init()
{
    main_menu = new QMenu(this);
    cat_menu = new Categorymenu(main_menu); // add Category menu on Launcher

    connect(main_menu, SIGNAL(triggered(QAction *)), this, SLOT(run_command(QAction *))); // Quit, Run, Refresh, Manager

    quit = new QAction(tr("Quit WM"), this);
    shutdown = new QAction(tr("Shutdown PC"), this);
    restart = new QAction(tr("Restart PC"), this);
    refresh = new QAction(tr("Refresh WM"), this);
    run = new QAction(tr("Run..."), this);
    manag = new QAction(tr("Manager"), this);
    show_desk = new QAction(tr("Show Desktop"), this);
    quit->setIcon(QIcon(quit_pix));
    quit->setData("quit");
    shutdown->setIcon(QIcon(shutdown_pix));
    shutdown->setData("shutdown");
    restart->setIcon(QIcon(restart_pix));
    restart->setData("restart");
    refresh->setIcon(QIcon(refresh_pix));
    refresh->setData("refresh");
    show_desk->setIcon(QIcon(show_pix));
    show_desk->setData("show");
    run->setIcon(QIcon(run_pix));
    run->setData("run");
    manag->setIcon(QIcon(manager_pix));
    manag->setData("manager");
    main_menu->addAction(manag);
    main_menu->addAction(run);
    main_menu->addAction(show_desk);
    main_menu->addAction(quit);
    main_menu->addAction(shutdown);
    main_menu->addAction(restart);
    main_menu->addAction(refresh);
}

void Launcher::read_settings()
{
    // get style path
    QSettings *antico = new QSettings(QCoreApplication::applicationDirPath() + "/antico.cfg", QSettings::IniFormat, this);
    antico->beginGroup("Style");
    QString stl_name = antico->value("name").toString();
    QString stl_path = antico->value("path").toString();
    antico->endGroup(); //Style
    // get action icons
    style = new QSettings(stl_path + stl_name, QSettings::IniFormat, this);
    style->beginGroup("Launcher");
    style->beginGroup("Icon");
    launcher_pix = stl_path + style->value("launcher_pix").toString();
    app_pix = stl_path + style->value("application_pix").toString();
    quit_pix = stl_path + style->value("quit_pix").toString();
    shutdown_pix = stl_path + style->value("shutdown_pix").toString();
    restart_pix = stl_path + style->value("restart_pix").toString();
    refresh_pix = stl_path + style->value("refresh_pix").toString();
    show_pix = stl_path + style->value("show_pix").toString();
    run_pix = stl_path + style->value("run_pix").toString();
    manager_pix = stl_path + style->value("manager_pix").toString();
    style->endGroup(); // Icon
    style->endGroup(); // Launcher
}

void Launcher::run_command(QAction *act)
{
    QString cmd = act->data().toString();

    if (cmd == "quit")
        app->wm_quit();
    if (cmd == "shutdown")
        app->shutdown();
    if (cmd == "restart")
        app->restart();
    if (cmd == "refresh")
        app->wm_refresh();
    if (cmd == "show")
        app->show_desktop();
    if (cmd == "manager")
        new Manager();
    if (cmd == "run")
        new Runner();
}

void Launcher::mousePressEvent(QMouseEvent *event)
{
    cat_menu->update_menu();

    if (event->button() == Qt::LeftButton)
    {
        QPoint p = mapToGlobal(QPoint(0, 0));
        QSize s(main_menu->sizeHint());
        p.setY(p.y()-s.height());
        main_menu->exec(p);
    }
}

void Launcher::update_style()
{
    read_settings();
    setPixmap(QPixmap(launcher_pix));
    setScaledContents(true);
    quit->setIcon(QIcon(quit_pix));
    shutdown->setIcon(QIcon(shutdown_pix));
    restart->setIcon(QIcon(restart_pix));
    refresh->setIcon(QIcon(refresh_pix));
    show_desk->setIcon(QIcon(show_pix));
    run->setIcon(QIcon(run_pix));
    manag->setIcon(QIcon(manager_pix));
    cat_menu->update_style(); // update category pixmaps
}




