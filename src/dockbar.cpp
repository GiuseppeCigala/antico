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
    file_dialog = app->get_file_dialog();
    dock_layout = new QHBoxLayout(this);
    setLayout(dock_layout);
    dock_layout->setContentsMargins(0, 0, 0, 0);
    dock_layout->setSpacing(1);
    dock_layout->setSizeConstraint(QLayout::SetNoConstraint);
    setAcceptDrops(true); // for drag and drop from Filedialog
    read_settings();
    init();
    set_geometry();

    // add launcher to dockbar
    lchr = new Launcher(a, this);
    lchr->setFixedSize(dock_height-1, dock_height-1);
    // for set dockapp on dockbar
    d_app_widget = new QWidget(this);
    // for set dockicon on dockbar
    d_icon_widget = new QWidget(this);
    // add systray to dockbar
    sys = new Systray(this);
    // add clock to dockbar
    clk = new Dateclock(this);
    clk->setFixedSize(dock_height*2, dock_height-1);

    icon_layout = new QHBoxLayout();
    d_icon_widget->setLayout(icon_layout);
    icon_layout->setAlignment(Qt::AlignLeft);
    icon_layout->setContentsMargins(0, 0, 0, 0);
    icon_layout->setSpacing(1);

    app_layout = new QHBoxLayout();
    d_app_widget->setLayout(app_layout);
    app_layout->setAlignment(Qt::AlignLeft);
    app_layout->setContentsMargins(0, 0, 0, 0);
    app_layout->setSpacing(1);

    dock_layout->insertWidget(0, lchr);
    dock_layout->insertWidget(1, d_app_widget, 1);
    dock_layout->insertWidget(2, d_icon_widget, 6); // max stretch factor
    dock_layout->insertWidget(3, sys, 3);
    dock_layout->insertWidget(4, clk);

    set_dockapp(); // at startup, restore dockapps on dockbar

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
    style->beginGroup("Other");
    app_link_pix = stl_path + style->value("app_link_pix").toString();
    style->endGroup(); //Other
}

void Dockbar::init()
{
    menu = new QMenu(this);
    menu->addAction(QIcon(app_link_pix), tr("New link to application"));
    connect(menu, SIGNAL(triggered(QAction *)), this, SLOT(run_menu(QAction *)));
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
        d_icon->set_state("Normal");
        dock_icons.insert(frm->winId(), d_icon); // save the Frame winId/Dockicon
        qDebug() << "Dockicon added to Dockbar. Frame:" << frm->winId();
        update_dockicon_size();
        connect(d_icon, SIGNAL(destroy_dockicon(Dockicon *)), this, SLOT(remove_dockicon(Dockicon *))); // delete iconize dockicon and update dockbar size
    }
}

void Dockbar::unmap(Frame *frm)
{
    if (dock_icons.contains(frm->winId())) // iconize Dockicon if present
    {
        Dockicon *d_icon = dock_icons.value(frm->winId());
        d_icon->set_state("Iconize");
    }
}

void Dockbar::remove(Frame *frm)
{
    if (dock_icons.contains(frm->winId())) // remove Dockicon if present
    {
        Dockicon *d_icon = dock_icons.value(frm->winId());
        remove_dockicon(d_icon);
    }
}

void Dockbar::map(Frame *frm)
{
    if (dock_icons.contains(frm->winId())) // map Dockicon if present
    {
        Dockicon *d_icon = dock_icons.value(frm->winId());
        d_icon->set_state("Normal");
    }
}

void Dockbar::remove_dockicon(Dockicon *d_icon) // remove from "Close" right button mouse on Dockbar
{
    dock_icons.remove(dock_icons.key(d_icon));
    qDebug() << "Dockicon remove. Num. after deletion:" << dock_icons.size();
    d_icon->close();
    update_dockicon_size();
}

void Dockbar::remove_dockicon(Window win_id) //remove from "Close" cmd on Systray (_NET_SYSTEM_TRAY protocol) if Dockicon is still mapped
{
    if (dock_icons.contains(win_id))
    {
        Dockicon *d_icon = dock_icons.value(win_id);
        dock_icons.remove(win_id);
        qDebug() << "Dockicon remove from Systray cmd. Num. after deletion:" << dock_icons.size();
        d_icon->close();
        update_dockicon_size();
    }
    else
    {
        sys->remove(win_id); // check in the System Tray eventually Sysicon still mapped
    }
}

void Dockbar::remove_dockapp(Dockapp *d_app) // remove from "Delete link" right button mouse on Dockbar
{
    dock_apps.removeOne(d_app);
    qDebug() << "Dockapp remove. Num. after deletion:" << dock_apps.size();
    d_app->close();
}

void Dockbar::update_dockicon_size()
{
    if (! dock_icons.isEmpty())
    {
        int num = dock_icons.size();
        qDebug() << "Dockicon num:" << num;
        d_length = d_icon_widget->width()/num;

        if (d_length >= d_icon_widget->width()/3) // max dockicon size = d_icon_widget size/3
            d_length = d_icon_widget->width()/3;

        qDebug() << "Dockicon length:" << d_length;

        foreach(Dockicon *d_icon, dock_icons)
        {
            d_icon->setFixedSize(d_length-2, dock_height-5);
            icon_layout->addWidget(d_icon);
        }
    }
}

void Dockbar::run_menu(QAction *act)
{
    if (act->text().compare(tr("New link to application")) == 0)
    {
        file_dialog->clear();
        file_dialog->set_type(tr("New link to application:"), "OK_Cancel");

        if (file_dialog->exec() == QDialog::Accepted)
        {
            QString path = file_dialog->get_selected_path();
            QString name = file_dialog->get_selected_name();
            QPoint pos = menu->pos();
            QFileInfo pathinfo(path+name);

            if (! name.isEmpty() && pathinfo.isExecutable())
            {
                create_dock_app(name, path, this);
            }
        }
    }
}

void Dockbar::set_dockapp()
{
    // read dockapp name, path and pixmap and restore on dockbar
    antico->beginGroup("Dockbar");
    antico->beginGroup("App");

    for (int i = 0; i < antico->childGroups().size(); i++)
    {
        antico->beginGroup(antico->childGroups().value(i)); // App name

        QString name = antico->value("name").toString();
        QString path = antico->value("path").toString();
        QString pix = antico->value("pix").toString();
        Dockapp *d_app = new Dockapp(name, path, pix, this);
        dock_apps << d_app; // save the dockapp

        d_app->setFixedSize(dock_height-2, dock_height-2);
        app_layout->addWidget(d_app);

        connect(d_app, SIGNAL(destroy_dockapp(Dockapp *)), this, SLOT(remove_dockapp(Dockapp *))); // delete dockapp form list

        antico->endGroup(); // App name
    }
    antico->endGroup(); //App
    antico->endGroup(); //Dockapp
}

void Dockbar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        menu->exec(event->globalPos());
    }
}

void Dockbar::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "dragEnterEvent";
    event->acceptProposedAction();
}

void Dockbar::dragMoveEvent(QDragMoveEvent *event)
{
    qDebug() << "dragMoveEvent";
    event->acceptProposedAction();
}

void Dockbar::dropEvent(QDropEvent *event) // add apps on Dockbar by drag&drop from Filedialog
{
    if (event->proposedAction() == Qt::LinkAction)
    {
        qDebug() << "dropEvent";
        QTreeView *tree_view = (QTreeView *)event->source();
        QDirModel *dir_model = (QDirModel *)tree_view->model();
        QModelIndex selection = tree_view->currentIndex();

        QString name = dir_model->fileName(tree_view->currentIndex());
        qDebug() << "Selected name:" << name;

        if (! dir_model->isDir(selection)) // is not a directory
        {
            QString filepath = dir_model->filePath(selection);
            QFileInfo pathinfo(filepath);
            QString path = pathinfo.absolutePath(); // remove the file name from path
            path.append("/"); // add slash at the end
            qDebug() << "Selected path:" << path;

            QFileInfo nameinfo(name);
            Fileicon *prov = (Fileicon *)dir_model->iconProvider();
            QString icon = prov->type(nameinfo); // get the file icon

            if (! name.isEmpty() && pathinfo.isExecutable())
            {
                create_dock_app(name, path, this);
            }
        }
    }
}

void Dockbar::create_dock_app(const QString &name, const QString &path, QWidget *parent)
{
    Appicon app_icon; // get application icon
    QString pix = app_icon.get_app_icon(name);
    Dockapp *d_app = new Dockapp(name, path, pix, parent); // new dockbar application
    dock_apps << d_app; // save the new dockapp
    // save new dockapp name, path and pix
    antico->beginGroup("Dockbar");
    antico->beginGroup("App");
    antico->beginGroup(name);
    antico->setValue("name", name);
    antico->setValue("exec", path + name);
    antico->setValue("pix", pix);
    antico->endGroup(); //name
    antico->endGroup(); //App
    antico->endGroup(); //Dockbar

    d_app->setFixedSize(dock_height-2, dock_height-2);
    app_layout->addWidget(d_app);

    connect(d_app, SIGNAL(destroy_dockapp(Dockapp *)), this, SLOT(remove_dockapp(Dockapp *))); // delete dockapp form list
}

void Dockbar::update_style()
{
    read_settings();
    set_geometry();
    update_dockicon_size();
    // update launcher
    lchr->update_style();
    lchr->setFixedSize(dock_height-1, dock_height-1);
    // update dockicons
    foreach(Dockicon *d_icon, dock_icons)
    d_icon->update_style();
    // update dockapps
    foreach(Dockapp *d_app, dock_apps)
    {
        d_app->update_style();
        d_app->setFixedSize(dock_height-2, dock_height-2);
    }
    // update sysicons on systray
    sys->update_style();
    // update dateclock
    clk->update_style();
    clk->setFixedSize(dock_height*2, dock_height-1);
}