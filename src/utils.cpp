////////////////////////////////////////
//  File      : utils.cpp             //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "utils.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// APPICON /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

Appicon::Appicon(QObject *parent) : QObject(parent)
{}

Appicon::~Appicon()
{}

QString Appicon::get_app_icon(const QString &app) // select the application from ".desktop" file
{
    QString app_icon;
    // get style path
    QSettings settings("/usr/share/applications/" + app + ".desktop", QSettings::IniFormat);
    qDebug() << "Application:" << app;
    settings.beginGroup("Desktop Entry");
    QString icon = settings.value("Icon").toString();
    qDebug() << "Icon:" << icon;

    if (icon.isEmpty()) // if not defined, set default application icon
    {
        // get style path
        QSettings *antico = new QSettings(QCoreApplication::applicationDirPath() + "/antico.cfg", QSettings::IniFormat, this);
        antico->beginGroup("Style");
        QString stl_name = antico->value("name").toString();
        QString stl_path = antico->value("path").toString();
        antico->endGroup(); //Style
        // get style values
        QSettings *style = new QSettings(stl_path + stl_name, QSettings::IniFormat, this);
        style->beginGroup("Launcher");
        style->beginGroup("Icon");
        app_icon = stl_path + style->value("application_pix").toString();
        style->endGroup(); // Icon
        style->endGroup(); // Launcher
        return app_icon;
    }
    else
    {
        // remove the extension (.png/.xpm)
        if (icon.endsWith(".png"))
            icon.remove(".png");
        else if (icon.endsWith(".xpm"))
            icon.remove(".xpm");
        // first search in /pixmaps directory
        app_icon = search_in_path(QString("/usr/share/pixmaps/"), icon);
        if (!app_icon.isEmpty())
            return app_icon;
        // else search in /icons directory
        app_icon = search_in_path(QString("/usr/share/icons/hicolor/32x32/apps/"), icon);
        if (!app_icon.isEmpty())
            return app_icon;
    }
    return "";
}

QString Appicon::search_in_path(const QString &path, const QString &icon)
{
    QString icon_path;
    QString png_path(path + icon + (".png"));
    QFile png_file(png_path);
    QString xpm_path(path + icon + (".xpm"));
    QFile xpm_file(xpm_path);

    if (png_file.exists())
    {
        icon_path = png_path;
        qDebug() << "Pixmap (.png) path:" << png_path;
        return icon_path;
    }
    else if (xpm_file.exists())
    {
        icon_path = xpm_path;
        qDebug() << "Pixmap (.xpm) path:" << xpm_path;
        return icon_path;
    }
    return "";
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// CATEGORYMENU ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

Categorymenu::Categorymenu(QMenu *menu, QWidget *parent) : QWidget(parent)
{
    main_menu = menu;
    read_settings();
    init();
    update_menu();
}

Categorymenu::~Categorymenu()
{}

void Categorymenu::init()
{
    audio_menu = main_menu->addMenu(QIcon(audio_pix), "Audio");
    video_menu = main_menu->addMenu(QIcon(video_pix), "Video");
    system_menu = main_menu->addMenu(QIcon(system_pix), "System");
    development_menu = main_menu->addMenu(QIcon(devel_pix), "Development");
    graphic_menu = main_menu->addMenu(QIcon(graphic_pix), "Graphic");
    internet_menu = main_menu->addMenu(QIcon(internet_pix), "Internet");
    office_menu = main_menu->addMenu(QIcon(office_pix), "Office");
    utility_menu = main_menu->addMenu(QIcon(utility_pix), "Utility");
    main_menu->addSeparator();

    connect(audio_menu, SIGNAL(triggered(QAction *)), this, SLOT(run_menu(QAction *)));
    connect(video_menu, SIGNAL(triggered(QAction *)), this, SLOT(run_menu(QAction *)));
    connect(system_menu, SIGNAL(triggered(QAction *)), this, SLOT(run_menu(QAction *)));
    connect(development_menu, SIGNAL(triggered(QAction *)), this, SLOT(run_menu(QAction *)));
    connect(graphic_menu, SIGNAL(triggered(QAction *)), this, SLOT(run_menu(QAction *)));
    connect(internet_menu, SIGNAL(triggered(QAction *)), this, SLOT(run_menu(QAction *)));
    connect(office_menu, SIGNAL(triggered(QAction *)), this, SLOT(run_menu(QAction *)));
    connect(utility_menu, SIGNAL(triggered(QAction *)), this, SLOT(run_menu(QAction *)));

}

void Categorymenu::read_settings()
{
    antico = new QSettings(QCoreApplication::applicationDirPath() + "/antico.cfg", QSettings::IniFormat);
    antico->beginGroup("Style");
    QString stl_name = antico->value("name").toString();
    QString stl_path = antico->value("path").toString();
    antico->endGroup(); //Style
    // get category menu icons
    style = new QSettings(stl_path + stl_name, QSettings::IniFormat);
    style->beginGroup("Launcher");
    style->beginGroup("Icon");
    utility_pix = stl_path + style->value("utility_pix").toString();
    office_pix = stl_path + style->value("office_pix").toString();
    internet_pix = stl_path + style->value("internet_pix").toString();
    graphic_pix = stl_path + style->value("graphic_pix").toString();
    devel_pix = stl_path + style->value("development_pix").toString();
    system_pix = stl_path + style->value("system_pix").toString();
    audio_pix = stl_path + style->value("audio_pix").toString();
    video_pix = stl_path + style->value("video_pix").toString();
    style->endGroup(); // Icon
    style->endGroup(); // Launcher
}

void Categorymenu::update_menu()
{
    utility_menu->clear(); // update Category menu
    office_menu->clear();
    internet_menu->clear();
    graphic_menu->clear();
    development_menu->clear();
    system_menu->clear();
    audio_menu->clear();
    video_menu->clear();

    antico->beginGroup("Launcher");
    antico->beginGroup("Category");
    //////////////////////////////////////////////////////////////////////////
    antico->beginGroup("Utility");
    for (int i = 0; i < antico->childGroups().size(); i++) // update Utility menu
    {
        antico->beginGroup(antico->childGroups().value(i)); // App name
        QString name = antico->value("name").toString();
        QString path = antico->value("path").toString();
        QString icon = antico->value("icon").toString();
        add_app(utility_menu, name, path, icon);
        antico->endGroup(); // App name
    }
    antico->endGroup(); // Utility
    //////////////////////////////////////////////////////////////////////////
    antico->beginGroup("Office");
    for (int i = 0; i < antico->childGroups().size(); i++) // update Office menu
    {
        antico->beginGroup(antico->childGroups().value(i)); // App name
        QString name = antico->value("name").toString();
        QString path = antico->value("path").toString();
        QString icon = antico->value("icon").toString();
        add_app(office_menu, name, path, icon);
        antico->endGroup(); // App name
    }
    antico->endGroup(); // Office
    //////////////////////////////////////////////////////////////////////////
    antico->beginGroup("Internet");
    for (int i = 0; i < antico->childGroups().size(); i++) // update Internet menu
    {
        antico->beginGroup(antico->childGroups().value(i)); // App name
        QString name = antico->value("name").toString();
        QString path = antico->value("path").toString();
        QString icon = antico->value("icon").toString();
        add_app(internet_menu, name, path, icon);
        antico->endGroup(); // App name
    }
    antico->endGroup(); // Internet
    //////////////////////////////////////////////////////////////////////////
    antico->beginGroup("Graphic");
    for (int i = 0; i < antico->childGroups().size(); i++) // update Graphic menu
    {
        antico->beginGroup(antico->childGroups().value(i)); // App name
        QString name = antico->value("name").toString();
        QString path = antico->value("path").toString();
        QString icon = antico->value("icon").toString();
        add_app(graphic_menu, name, path, icon);
        antico->endGroup(); // App name
    }
    antico->endGroup(); // Graphic
    //////////////////////////////////////////////////////////////////////////
    antico->beginGroup("Development");
    for (int i = 0; i < antico->childGroups().size(); i++) // update Development menu
    {
        antico->beginGroup(antico->childGroups().value(i)); // App name
        QString name = antico->value("name").toString();
        QString path = antico->value("path").toString();
        QString icon = antico->value("icon").toString();
        add_app(development_menu, name, path, icon);
        antico->endGroup(); // App name
    }
    antico->endGroup(); // Development
    //////////////////////////////////////////////////////////////////////////
    antico->beginGroup("System");
    for (int i = 0; i < antico->childGroups().size(); i++) // update System menu
    {
        antico->beginGroup(antico->childGroups().value(i)); // App name
        QString name = antico->value("name").toString();
        QString path = antico->value("path").toString();
        QString icon = antico->value("icon").toString();
        add_app(system_menu, name, path, icon);
        antico->endGroup(); // App name
    }
    antico->endGroup(); // System
    //////////////////////////////////////////////////////////////////////////
    antico->beginGroup("Audio");
    for (int i = 0; i < antico->childGroups().size(); i++) // update Audio menu
    {
        antico->beginGroup(antico->childGroups().value(i)); // App name
        QString name = antico->value("name").toString();
        QString path = antico->value("path").toString();
        QString icon = antico->value("icon").toString();
        add_app(audio_menu, name, path, icon);
        antico->endGroup(); // App name
    }
    antico->endGroup(); // Audio
    //////////////////////////////////////////////////////////////////////////
    antico->beginGroup("Video");
    for (int i = 0; i < antico->childGroups().size(); i++) // update Video menu
    {
        antico->beginGroup(antico->childGroups().value(i)); // App name
        QString name = antico->value("name").toString();
        QString path = antico->value("path").toString();
        QString icon = antico->value("icon").toString();
        add_app(video_menu, name, path, icon);
        antico->endGroup(); // App name
    }
    antico->endGroup(); // Video
    //////////////////////////////////////////////////////////////////////////
    antico->endGroup(); // Category
    antico->endGroup(); // Launcher
}

void Categorymenu::add_app(QMenu *category_menu, const QString &name, const QString &path, const QString &icon) // add application and icon on Category menu
{
    QAction *act = new QAction(QIcon(icon), name, category_menu);
    act->setData(path);
    category_menu->addAction(act);
}

void Categorymenu::run_menu(QAction *act)
{
    QString cmd = act->data().toString();
    qDebug() << "Command arguments on run:" << cmd_arguments;

    if (cmd_arguments == "")
        QProcess::startDetached(cmd); // start Application from Category menu
    else
        QProcess::startDetached(cmd + " " + cmd_arguments); //start Application + arguments from Category menu
}

void Categorymenu::set_cmd_arguments(const QString &arg) // add arguments after application name on run
{
    qDebug() << "Command arguments on run:" << arg;
    cmd_arguments = arg;
}

void Categorymenu::update_style()
{
    read_settings();
    audio_menu->setIcon(QIcon(audio_pix));
    video_menu->setIcon(QIcon(video_pix));
    system_menu->setIcon(QIcon(system_pix));
    development_menu->setIcon(QIcon(devel_pix));
    graphic_menu->setIcon(QIcon(graphic_pix));
    internet_menu->setIcon(QIcon(internet_pix));
    office_menu->setIcon(QIcon(office_pix));
    utility_menu->setIcon(QIcon(utility_pix));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// FILEICON ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

Fileicon::Fileicon() : QFileIconProvider()
{
    read_settings();
    // set the files extension
    devel << "h" << "cpp" << "o" << "sh" << "py" << "rb";
    graphic << "png" << "jpg" << "svg";
    system << "cfg" << "stl" << "pro" << "iso" << "img" << "bin" << "theme" << "conf";
    office << "odt" << "odf" << "odp" << "od" << "txt" << "pdf";
    video << "ogg" << "mpg" << "avi";
    audio << "wav" << "mp3";
    internet << "htm" << "html" << "xlm";
    utility << "gz" << "zip" << "bz2";
}

Fileicon::~Fileicon()
{}

QIcon Fileicon::icon(const QFileInfo &info) const
{
    if (info.isDir())
    {
        QIcon ico(d_folder_pix);
        return ico;
    }

    QString suff = info.suffix(); // get the file extension

    for (int i = 0; i < graphic.size(); i++)
    {
        if (graphic.at(i) == suff)
        {
            QIcon ico(graphic_pix);
            return ico;
        }
    }
    for (int i = 0; i < devel.size(); i++)
    {
        if (devel.at(i) == suff)
        {
            QIcon ico(devel_pix);
            return ico;
        }
    }
    for (int i = 0; i < system.size(); i++)
    {
        if (system.at(i) == suff)
        {
            QIcon ico(system_pix);
            return ico;
        }
    }
    for (int i = 0; i < office.size(); i++)
    {
        if (office.at(i) == suff)
        {
            QIcon ico(office_pix);
            return ico;
        }
    }
    for (int i = 0; i < video.size(); i++)
    {
        if (video.at(i) == suff)
        {
            QIcon ico(video_pix);
            return ico;
        }
    }
    for (int i = 0; i < audio.size(); i++)
    {
        if (audio.at(i) == suff)
        {
            QIcon ico(audio_pix);
            return ico;
        }
    }
    for (int i = 0; i < internet.size(); i++)
    {
        if (internet.at(i) == suff)
        {
            QIcon ico(internet_pix);
            return ico;
        }
    }
    for (int i = 0; i < utility.size(); i++)
    {
        if (utility.at(i) == suff)
        {
            QIcon ico(utility_pix);
            return ico;
        }
    }

    return QApplication::style()->standardIcon(QStyle::SP_FileIcon); //default icon
}

QIcon Fileicon::icon()
{
    QIcon ico(utility_pix); //default icon
    return ico;
}

QString Fileicon::type(const QFileInfo &info) const
{
    QString suff = info.suffix(); // get the file extension

    for (int i = 0; i < graphic.size(); i++)
    {
        if (graphic.at(i) == suff)
        {
            return graphic_pix;
        }
    }
    for (int i = 0; i < devel.size(); i++)
    {
        if (devel.at(i) == suff)
        {
            return devel_pix;
        }
    }
    for (int i = 0; i < system.size(); i++)
    {
        if (system.at(i) == suff)
        {
            return system_pix;
        }
    }
    for (int i = 0; i < office.size(); i++)
    {
        if (office.at(i) == suff)
        {
            return office_pix;
        }
    }
    for (int i = 0; i < video.size(); i++)
    {
        if (video.at(i) == suff)
        {
            return video_pix;
        }
    }
    for (int i = 0; i < audio.size(); i++)
    {
        if (audio.at(i) == suff)
        {
            return audio_pix;
        }
    }
    for (int i = 0; i < internet.size(); i++)
    {
        if (internet.at(i) == suff)
        {
            return internet_pix;
        }
    }
    for (int i = 0; i < utility.size(); i++)
    {
        if (utility.at(i) == suff)
        {
            return utility_pix;
        }
    }
    return system_pix; // default pix
}

void Fileicon::read_settings()
{
    antico = new QSettings(QCoreApplication::applicationDirPath() + "/antico.cfg", QSettings::IniFormat);
    antico->beginGroup("Style");
    QString stl_name = antico->value("name").toString();
    QString stl_path = antico->value("path").toString();
    antico->endGroup(); //Style
    // get category menu icons
    style = new QSettings(stl_path + stl_name, QSettings::IniFormat);
    style->beginGroup("Launcher");
    style->beginGroup("Icon");
    utility_pix = stl_path + style->value("utility_pix").toString();
    office_pix = stl_path + style->value("office_pix").toString();
    internet_pix = stl_path + style->value("internet_pix").toString();
    graphic_pix = stl_path + style->value("graphic_pix").toString();
    devel_pix = stl_path + style->value("development_pix").toString();
    system_pix = stl_path + style->value("system_pix").toString();
    audio_pix = stl_path + style->value("audio_pix").toString();
    video_pix = stl_path + style->value("video_pix").toString();
    style->endGroup(); // Icon
    style->endGroup(); // Launcher
    style->beginGroup("Deskfolder");
    d_folder_pix = stl_path + style->value("d_folder_pix").toString();
    style->endGroup(); // Deskfolder
}
