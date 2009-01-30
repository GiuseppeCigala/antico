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
    settings.endGroup(); // Desktop Entry

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
        qDebug() << "---------------------------------------------";
        return icon_path;
    }
    else if (xpm_file.exists())
    {
        icon_path = xpm_path;
        qDebug() << "Pixmap (.xpm) path:" << xpm_path;
        qDebug() << "---------------------------------------------";
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
    parse_desktop_file();
    update_menu();
}

Categorymenu::~Categorymenu()
{}

void Categorymenu::init()
{
    audiovideo_menu = main_menu->addMenu(QIcon(audiovideo_pix), tr("AudioVideo"));
    system_menu = main_menu->addMenu(QIcon(system_pix), tr("System"));
    development_menu = main_menu->addMenu(QIcon(devel_pix), tr("Development"));
    graphics_menu = main_menu->addMenu(QIcon(graphics_pix), tr("Graphics"));
    network_menu = main_menu->addMenu(QIcon(network_pix), tr("Network"));
    office_menu = main_menu->addMenu(QIcon(office_pix), tr("Office"));
    utility_menu = main_menu->addMenu(QIcon(utility_pix), tr("Utility"));
    main_menu->addSeparator();

    connect(audiovideo_menu, SIGNAL(triggered(QAction *)), this, SLOT(run_menu(QAction *)));
    connect(system_menu, SIGNAL(triggered(QAction *)), this, SLOT(run_menu(QAction *)));
    connect(development_menu, SIGNAL(triggered(QAction *)), this, SLOT(run_menu(QAction *)));
    connect(graphics_menu, SIGNAL(triggered(QAction *)), this, SLOT(run_menu(QAction *)));
    connect(network_menu, SIGNAL(triggered(QAction *)), this, SLOT(run_menu(QAction *)));
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
    network_pix = stl_path + style->value("network_pix").toString();
    graphics_pix = stl_path + style->value("graphics_pix").toString();
    devel_pix = stl_path + style->value("development_pix").toString();
    system_pix = stl_path + style->value("system_pix").toString();
    audiovideo_pix = stl_path + style->value("audiovideo_pix").toString();
    style->endGroup(); // Icon
    style->endGroup(); // Launcher
}

void Categorymenu::update_menu()
{
    cat_menu.insert("Office", office_menu);
    cat_menu.insert("Network", network_menu);
    cat_menu.insert("Graphics", graphics_menu);
    cat_menu.insert("Development", development_menu);
    cat_menu.insert("System", system_menu);
    cat_menu.insert("AudioVideo", audiovideo_menu);
    cat_menu.insert("Utility", utility_menu);

    utility_menu->clear(); // update Category menu
    office_menu->clear();
    network_menu->clear();
    graphics_menu->clear();
    development_menu->clear();
    system_menu->clear();
    audiovideo_menu->clear();

    parse_desktop_file();

    /////////// SET BY USER ///////////
    antico->beginGroup("Launcher");
    antico->beginGroup("Category");

    QHashIterator<QString, QMenu *> cat_iter(cat_menu);
    while (cat_iter.hasNext())
    {
        cat_iter.next();
        antico->beginGroup(cat_iter.key()); // category name
        for (int i = 0; i < antico->childGroups().size(); i++) 
        {
            antico->beginGroup(antico->childGroups().value(i)); // App name
            QString name = antico->value("name").toString();
            QString exec = antico->value("exec").toString();
            QString icon = antico->value("pix").toString();
            add_app(cat_iter.value(), name, exec, icon);
            antico->endGroup(); // App name
        }
        antico->endGroup();
    }
    antico->endGroup(); // Category
    antico->endGroup(); // Launcher
    
    /////////// READ FROM .DESKTOP FILE ///////////
    antico->beginGroup("Launcher-Desk"); 
    antico->beginGroup("Category");

    QHashIterator<QString, QMenu *> cat_desk_iter(cat_menu);
    while (cat_desk_iter.hasNext())
    {
        cat_desk_iter.next();
        antico->beginGroup(cat_desk_iter.key()); // category name
        for (int i = 0; i < antico->childGroups().size(); i++) 
        {
            antico->beginGroup(antico->childGroups().value(i)); // App name
            QString name = antico->value("name").toString();
            QString exec = antico->value("exec").toString();
            QString icon = antico->value("pix").toString();
            add_app(cat_desk_iter.value(), name, exec, icon);
            antico->endGroup(); // App name
        }
        antico->endGroup();
    }
    antico->endGroup(); // Category
    antico->endGroup(); // Launcher-Desk
}

void Categorymenu::add_app(QMenu *category_menu, const QString &name, const QString &exec, const QString &icon) // add application and icon on Category menu
{
    QAction *act = new QAction(QIcon(icon), name, category_menu);
    act->setData(exec);
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
    audiovideo_menu->setIcon(QIcon(audiovideo_pix));
    system_menu->setIcon(QIcon(system_pix));
    development_menu->setIcon(QIcon(devel_pix));
    graphics_menu->setIcon(QIcon(graphics_pix));
    network_menu->setIcon(QIcon(network_pix));
    office_menu->setIcon(QIcon(office_pix));
    utility_menu->setIcon(QIcon(utility_pix));
}

void Categorymenu::parse_desktop_file()
{
    antico->remove("Launcher-Desk"); // clear Launcher-Desk entry

    QDir desktop_dir("/usr/share/applications/");
    QFileInfoList list = desktop_dir.entryInfoList();
    qDebug() << "Number of .desktop file:" << list.size();

    QList<QString> cat_list;
    cat_list.append("Office");
    cat_list.append("Network");
    cat_list.append("Graphics");
    cat_list.append("Development");
    cat_list.append("System");
    cat_list.append("AudioVideo");
    cat_list.append("Utility");

    QString lang = QLocale::system().name(); // (it_IT)
    QString country = lang.section('_', 0, 0); // (it_IT -> it)
    QString locale_name = QString("Name").append("[").append(country).append("]").append("="); // Name[it]

    for (int i = 0; i < list.size(); i++)
    {
        QString name;
        QString exec;
        QString icon_name;
        QString icon_path;
        QString categories;
        QString full_path;
        QString args;
        QStringList split_path;
        QString app_name;

        QFileInfo finf = list.at(i);
        QString file_name = finf.fileName();

        if (file_name == "." || file_name == "..")
            continue;

        QFile file("/usr/share/applications/" + file_name);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Problem to read .desktop file:" << file_name;
            continue;
        }

        QTextStream stream(&file);
        QString line;

        ///////////// PARSE THE .DESKTOP FILE /////////////
        do
        {
            line = stream.readLine();

            if (! line.isNull() || ! line.isEmpty())
            {
                if (line.startsWith("Categories="))
                {
                    QString new_line = line.remove("Categories=");
                    QStringList split = new_line.split(";"); // get the single categories

                    for (int i = 0; i < split.size(); ++i)
                    {
                        if (cat_list.contains(split.at(i)))
                            categories = split.at(i); // get the category
                    }
                    qDebug() << "Categories:" << categories;
                }
                if (line.startsWith("Name="))
                {
                    name = line.remove("Name="); // default name
                    qDebug() << "Name:" << name;
                }
                if (line.startsWith(locale_name))
                {
                    name = line.remove(locale_name); // locale name
                    qDebug() << "Locale Name:" << name;
                }
                if (line.startsWith("Exec="))
                {
                    exec = line.remove("Exec=");
                    if (! exec.startsWith('/')) // full path not already set
                        exec.prepend("/usr/bin/");
                    qDebug() << "Exec:" << exec;
                }
                if (line.startsWith("Icon="))
                {
                    icon_name = line.remove("Icon=");
                    qDebug() << "Icon:" << icon_name;
                }
            }
        }
        while (!line.isNull());

        if (! exec.isEmpty() && ! categories.isEmpty() && ! name.isEmpty() && ! icon_name.isEmpty())
        {

            full_path = exec.section(' ', 0, 0); // remove eventually args after app name (/usr/bin/vlc %U -> /usr/bin/vlc)
            qDebug() << "full_path:" << full_path;
            args = exec.section(' ', 1, 1); // save args after app name with space (/usr/bin/vlc %U -> _%U)

            if (args.startsWith('-')) // leave only args i.e. for OpenOffice (soffice -writer)
                qDebug() << "args:" << args;
            else
                args.clear();

            split_path = full_path.split('/', QString::SkipEmptyParts); // (/usr/bin/vlc -> usr, bin, vlc)
            qDebug() << "split_path:" << split_path;
            app_name = split_path.last(); // (usr, bin, vlc -> vlc)
            qDebug() << "App_name:" << app_name;
            qDebug() << "---------------------------------------------";

            if (! icon_name.startsWith('/')) // full icon path not already set
            {
                // remove the extension (.png/.xpm)
                if (icon_name.endsWith(".png"))
                    icon_name.remove(".png");
                else if (icon_name.endsWith(".xpm"))
                    icon_name.remove(".xpm");
                Appicon *app_ico = new Appicon(); // get application icon path
                icon_path = app_ico->get_app_icon(icon_name);
            }
            else
                icon_path = icon_name; // full icon path already set

            if (! cat_list.contains(categories))
                categories = "Utility";

            antico->beginGroup("Launcher-Desk");
            antico->beginGroup("Category");
            antico->beginGroup(categories);
            antico->beginGroup(app_name + args); // for i.e. soffice -writer

            antico->setValue("name", name);
            if (args.isEmpty())
                antico->setValue("exec", full_path);
            else
                antico->setValue("exec", full_path.append(" ").append(args));
            antico->setValue("pix", icon_path);

            antico->endGroup(); // App name
            antico->endGroup(); // Category type
            antico->endGroup(); // Category group
            antico->endGroup(); // Launcher-Desk
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// FILEICON ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

Fileicon::Fileicon() : QFileIconProvider()
{
    read_settings();
    // set the files extension
    devel << "h" << "cpp" << "o" << "sh" << "py" << "rb";
    graphics << "png" << "jpg" << "svg";
    system << "cfg" << "stl" << "pro" << "iso" << "img" << "bin" << "theme" << "conf";
    office << "odt" << "odf" << "odp" << "od" << "txt" << "pdf";
    audiovideo << "wav" << "mp3" << "ogg" << "mpg" << "avi";
    network << "htm" << "html" << "xlm";
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

    for (int i = 0; i < graphics.size(); i++)
    {
        if (graphics.at(i) == suff)
        {
            QIcon ico(graphics_pix);
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
    for (int i = 0; i < audiovideo.size(); i++)
    {
        if (audiovideo.at(i) == suff)
        {
            QIcon ico(audiovideo_pix);
            return ico;
        }
    }
    for (int i = 0; i < network.size(); i++)
    {
        if (network.at(i) == suff)
        {
            QIcon ico(network_pix);
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

    for (int i = 0; i < graphics.size(); i++)
    {
        if (graphics.at(i) == suff)
        {
            return graphics_pix;
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
    for (int i = 0; i < audiovideo.size(); i++)
    {
        if (audiovideo.at(i) == suff)
        {
            return audiovideo_pix;
        }
    }
    for (int i = 0; i < network.size(); i++)
    {
        if (network.at(i) == suff)
        {
            return network_pix;
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
    network_pix = stl_path + style->value("network_pix").toString();
    graphics_pix = stl_path + style->value("graphics_pix").toString();
    devel_pix = stl_path + style->value("development_pix").toString();
    system_pix = stl_path + style->value("system_pix").toString();
    audiovideo_pix = stl_path + style->value("audiovideo_pix").toString();
    style->endGroup(); // Icon
    style->endGroup(); // Launcher
    style->beginGroup("Deskfolder");
    d_folder_pix = stl_path + style->value("d_folder_pix").toString();
    style->endGroup(); // Deskfolder
}
