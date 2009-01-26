////////////////////////////////////////
//  File      : manager.cpp           //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "manager.h"

////////////////////////////////////////

Manager::Manager(QWidget *parent) : QDialog(parent)
{
    // get style path
    antico = new QSettings(QCoreApplication::applicationDirPath() + "/antico.cfg", QSettings::IniFormat, this);
    antico->beginGroup("Style");
    stl_name = antico->value("name").toString();
    stl_path = antico->value("path").toString();
    antico->endGroup(); //Style
    // get style values
    style = new QSettings(stl_path + stl_name, QSettings::IniFormat, this);
    init(); // before that read_settings
    read_settings();
    setSizeGripEnabled(true);
    setContentsMargins(0, 10, 0, 0);
    show();
}

Manager::~Manager()
{}

void Manager::init()
{
    tab = new QTabWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
    QLabel *lab = new QLabel("<b>MANAGER</b>", this);
    lab->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    layout->addWidget(lab);
    layout->addWidget(tab);
    style_tab();
    add_app_tab();
    remove_app_tab();
    run_app_tab();
}

void Manager::read_settings()
{
    style_sel_tx->setText(stl_name); // show actual style name
    /////////////////////////////////////////////////////////////////////////
    style->beginGroup("Frame");
    style->beginGroup("Border");
    top_bdr_spinBox->setValue(style->value("top_bdr_height").toInt());
    lateral_bdr_spinBox->setValue(style->value("lateral_bdr_width").toInt());
    bottom_bdr_spinBox->setValue(style->value("bottom_bdr_height").toInt());
    style->endGroup(); // Border
    style->beginGroup("Header");
    header_active_pix_path = stl_path + style->value("header_active_pix").toString();
    header_inactive_pix_path = stl_path + style->value("header_inactive_pix").toString();
    header_active_pix->setToolTip(style->value("header_active_pix").toString());
    header_inactive_pix->setToolTip(style->value("header_inactive_pix").toString());
    header_active_pix->setPixmap(QPixmap(header_active_pix_path));
    header_inactive_pix->setPixmap(QPixmap(header_inactive_pix_path));
    minmax_pix_path = stl_path + style->value("minmax_pix").toString();
    minmax_pix->setToolTip(style->value("minmax_pix").toString());
    minmax_pix->setPixmap(QPixmap(minmax_pix_path));
    close_pix_path = stl_path + style->value("close_pix").toString();
    close_pix->setToolTip(style->value("close_pix").toString());
    close_pix->setPixmap(QPixmap(close_pix_path));
    header_col_lab->setPalette(QPalette(style->value("title_color").value<QColor>()));
    header_col_lab->setAutoFillBackground(true);
    style->endGroup(); //Header
    style->endGroup(); //Frame
    /////////////////////////////////////////////////////////////////////////
    style->beginGroup("Dockbar");
    dockbar_pix_path = stl_path + style->value("dock_pix").toString();
    dockbar_pix->setToolTip(style->value("dock_pix").toString());
    dockbar_pix->setPixmap(QPixmap(dockbar_pix_path));
    dockbar_height_spinBox->setValue(style->value("dock_height").toInt());
    style->endGroup(); // Dockbar
    /////////////////////////////////////////////////////////////////////////
    style->beginGroup("Dockicon");
    dockicon_pix_path = stl_path + style->value("d_icon_pix").toString();
    dockicon_pix->setToolTip(style->value("d_icon_pix").toString());
    dockicon_pix->setPixmap(QPixmap(dockicon_pix_path));
    dockicon_col_lab->setPalette(QPalette(style->value("title_color").value<QColor>()));
    dockicon_col_lab->setAutoFillBackground(true);
    style->endGroup(); // Dockicon
    /////////////////////////////////////////////////////////////////////////
    style->beginGroup("Deskapp");
    deskapp_col_lab->setPalette(QPalette(style->value("name_color").value<QColor>()));
    deskapp_col_lab->setAutoFillBackground(true);
    style->endGroup(); // Deskapp
    /////////////////////////////////////////////////////////////////////////
    style->beginGroup("Sysicon");
    sysicon_pix_path = stl_path + style->value("s_icon_pix").toString();
    sysicon_pix->setToolTip(style->value("s_icon_pix").toString());
    sysicon_pix->setPixmap(QPixmap(sysicon_pix_path));
    style->endGroup(); // Sysicon
    /////////////////////////////////////////////////////////////////////////
    style->beginGroup("Desktop");
    style->beginGroup("Wallpaper");
    desktop_pix_path = stl_path + style->value("wall_pix").toString();
    desktop_pix->setToolTip(style->value("wall_pix").toString());
    desktop_pix->setPixmap(QPixmap(desktop_pix_path));
    style->endGroup(); // Wallpaper
    style->endGroup(); // Desktop
    /////////////////////////////////////////////////////////////////////////
    style->beginGroup("Deskfolder");
    deskfolder_pix_path = stl_path + style->value("d_folder_pix").toString();
    deskfolder_pix->setToolTip(style->value("d_folder_pix").toString());
    deskfolder_pix->setPixmap(QPixmap(deskfolder_pix_path));
    deskfolder_col_lab->setPalette(QPalette(style->value("name_color").value<QColor>()));
    deskfolder_col_lab->setAutoFillBackground(true);
    style->endGroup(); // Deskfolder
    /////////////////////////////////////////////////////////////////////////
    style->beginGroup("Deskfile");
    deskfile_col_lab->setPalette(QPalette(style->value("name_color").value<QColor>()));
    deskfile_col_lab->setAutoFillBackground(true);
    style->endGroup(); // Deskfile
    /////////////////////////////////////////////////////////////////////////
    style->beginGroup("Deskdev");
    deskdev_disk_pix_path = stl_path + style->value("d_disk_pix").toString();
    deskdev_cdrom_pix_path = stl_path + style->value("d_cdrom_pix").toString();
    deskdev_disk_pix->setToolTip(style->value("d_disk_pix").toString());
    deskdev_cdrom_pix->setToolTip(style->value("d_cdrom_pix").toString());
    deskdev_disk_pix->setPixmap(QPixmap(deskdev_disk_pix_path));
    deskdev_cdrom_pix->setPixmap(QPixmap(deskdev_cdrom_pix_path));
    deskdev_col_lab->setPalette(QPalette(style->value("name_color").value<QColor>()));
    deskdev_col_lab->setAutoFillBackground(true);
    style->endGroup(); // Deskdev
    /////////////////////////////////////////////////////////////////////////
    style->beginGroup("Dateclock");
    date_col_lab->setPalette(QPalette(style->value("date_color").value<QColor>()));
    date_col_lab->setAutoFillBackground(true);
    clock_col_lab->setPalette(QPalette(style->value("clock_color").value<QColor>()));
    clock_col_lab->setAutoFillBackground(true);
    style->endGroup(); // Dateclock
    /////////////////////////////////////////////////////////////////////////
    style->beginGroup("Launcher");
    style->beginGroup("Icon");
    launcher_pix_path = stl_path + style->value("launcher_pix").toString();
    application_pix_path = stl_path + style->value("application_pix").toString();
    quit_pix_path = stl_path + style->value("quit_pix").toString();
    shutdown_pix_path = stl_path + style->value("shutdown_pix").toString();
    restart_pix_path = stl_path + style->value("restart_pix").toString();
    refresh_pix_path = stl_path + style->value("refresh_pix").toString();
    run_pix_path = stl_path + style->value("run_pix").toString();
    show_pix_path = stl_path + style->value("show_pix").toString();
    manager_pix_path = stl_path + style->value("manager_pix").toString();
    utility_pix_path = stl_path + style->value("utility_pix").toString();
    office_pix_path = stl_path + style->value("office_pix").toString();
    internet_pix_path = stl_path + style->value("internet_pix").toString();
    graphic_pix_path = stl_path + style->value("graphic_pix").toString();
    development_pix_path = stl_path + style->value("development_pix").toString();
    system_pix_path = stl_path + style->value("system_pix").toString();
    audio_pix_path = stl_path + style->value("audio_pix").toString();
    video_pix_path = stl_path + style->value("video_pix").toString();
    launcher_pix->setToolTip(style->value("launcher_pix").toString());
    application_pix->setToolTip(style->value("application_pix").toString());
    quit_pix->setToolTip(style->value("quit_pix").toString());
    shutdown_pix->setToolTip(style->value("shutdown_pix").toString());
    restart_pix->setToolTip(style->value("restart_pix").toString());
    refresh_pix->setToolTip(style->value("refresh_pix").toString());
    run_pix->setToolTip(style->value("run_pix").toString());
    show_pix->setToolTip(style->value("show_pix").toString());
    manager_pix->setToolTip(style->value("manager_pix").toString());
    utility_pix->setToolTip(style->value("utility_pix").toString());
    office_pix->setToolTip(style->value("office_pix").toString());
    internet_pix->setToolTip(style->value("internet_pix").toString());
    graphic_pix->setToolTip(style->value("graphic_pix").toString());
    development_pix->setToolTip(style->value("development_pix").toString());
    system_pix->setToolTip(style->value("system_pix").toString());
    audio_pix->setToolTip(style->value("audio_pix").toString());
    video_pix->setToolTip(style->value("video_pix").toString());
    launcher_pix->setPixmap(QPixmap(launcher_pix_path));
    application_pix->setPixmap(QPixmap(application_pix_path));
    quit_pix->setPixmap(QPixmap(quit_pix_path));
    shutdown_pix->setPixmap(QPixmap(shutdown_pix_path));
    restart_pix->setPixmap(QPixmap(restart_pix_path));
    refresh_pix->setPixmap(QPixmap(refresh_pix_path));
    run_pix->setPixmap(QPixmap(run_pix_path));
    show_pix->setPixmap(QPixmap(show_pix_path));
    manager_pix->setPixmap(QPixmap(manager_pix_path));
    utility_pix->setPixmap(QPixmap(utility_pix_path));
    office_pix->setPixmap(QPixmap(office_pix_path));
    internet_pix->setPixmap(QPixmap(internet_pix_path));
    graphic_pix->setPixmap(QPixmap(graphic_pix_path));
    development_pix->setPixmap(QPixmap(development_pix_path));
    system_pix->setPixmap(QPixmap(system_pix_path));
    audio_pix->setPixmap(QPixmap(audio_pix_path));
    video_pix->setPixmap(QPixmap(video_pix_path));
    style->endGroup(); // Icon
    style->endGroup(); // Launcher
    /////////////////////////////////////////////////////////////////////////
    style->beginGroup("Other");
    folder_link_pix_path = stl_path + style->value("folder_link_pix").toString();
    file_link_pix_path = stl_path + style->value("file_link_pix").toString();
    app_link_pix_path = stl_path + style->value("app_link_pix").toString();
    delete_link_pix_path = stl_path + style->value("delete_link_pix").toString();
    delete_file_pix_path = stl_path + style->value("delete_file_pix").toString();
    close_dock_pix_path = stl_path + style->value("close_dock_pix").toString();
    add_to_sys_pix_path = stl_path + style->value("add_to_sys_pix").toString();
    open_with_pix_path = stl_path + style->value("open_with_pix").toString();
    folder_link_pix->setToolTip(style->value("folder_link_pix").toString());
    file_link_pix->setToolTip(style->value("file_link_pix").toString());
    app_link_pix->setToolTip(style->value("app_link_pix").toString());
    delete_link_pix->setToolTip(style->value("delete_link_pix").toString());
    delete_file_pix->setToolTip(style->value("delete_file_pix").toString());
    close_dock_pix->setToolTip(style->value("close_dock_pix").toString());
    add_to_sys_pix->setToolTip(style->value("add_to_sys_pix").toString());
    open_with_pix->setToolTip(style->value("open_with_pix").toString());
    folder_link_pix->setPixmap(QPixmap(folder_link_pix_path));
    file_link_pix->setPixmap(QPixmap(file_link_pix_path));
    app_link_pix->setPixmap(QPixmap(app_link_pix_path));
    delete_link_pix->setPixmap(QPixmap(delete_link_pix_path));
    delete_file_pix->setPixmap(QPixmap(delete_file_pix_path));
    close_dock_pix->setPixmap(QPixmap(close_dock_pix_path));
    add_to_sys_pix->setPixmap(QPixmap(add_to_sys_pix_path));
    open_with_pix->setPixmap(QPixmap(open_with_pix_path));
    style->endGroup(); //Other
    /////////////////////////////////////////////////////////////////////////
}

void Manager::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::darkGray, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawRect(0, 0, width(), height());
}

void Manager::add_app_tab()
{
    QFrame *add_frm = new QFrame(this);
    add_frm->setFrameStyle(QFrame::Panel);
    tab->addTab(add_frm, QString("Add application"));
    QVBoxLayout *layout = new QVBoxLayout();
    add_frm->setLayout(layout);
    app_path = new QLineEdit(this); // show selection path
    dir_model = new QDirModel(this);
    Fileicon *prov = new Fileicon(); // get the files icon
    dir_model->setIconProvider(prov);
    QCompleter *completer = new QCompleter(this);
    completer->setModel(dir_model);
    app_path->setCompleter(completer);
    tree_view = new QTreeView(this);
    tree_view->setModel(dir_model);
    layout->addWidget(app_path);
    layout->addWidget(tree_view);

    QHBoxLayout *category_lay = new QHBoxLayout(this);
    layout->addLayout(category_lay);
    QLabel *category_lab = new QLabel("Select the category:", this);
    category_combo = new QComboBox(this);
    // add Category with icon on Combobox (if new Category is added, remember to update Launcher menu)
    style->beginGroup("Launcher");
    style->beginGroup("Icon");
    category_combo->addItem(QIcon(stl_path + style->value("utility_pix").toString()), "Utility");
    category_combo->addItem(QIcon(stl_path + style->value("office_pix").toString()), "Office");
    category_combo->addItem(QIcon(stl_path + style->value("internet_pix").toString()), "Internet");
    category_combo->addItem(QIcon(stl_path + style->value("graphic_pix").toString()), "Graphic");
    category_combo->addItem(QIcon(stl_path + style->value("development_pix").toString()), "Development");
    category_combo->addItem(QIcon(stl_path + style->value("system_pix").toString()), "System");
    category_combo->addItem(QIcon(stl_path + style->value("audio_pix").toString()), "Audio");
    category_combo->addItem(QIcon(stl_path + style->value("video_pix").toString()), "Video");
    style->endGroup(); // Icon
    style->endGroup(); // Launcher
    QPushButton *add_but = new QPushButton("Add", this);
    QPushButton* quit_but = new QPushButton("Quit", this);
    category_lay->addWidget(category_lab);
    category_lay->addWidget(category_combo);
    category_lay->addWidget(add_but);
    category_lay->addWidget(quit_but);

    connect(tree_view, SIGNAL(clicked(QModelIndex)), this, SLOT(show_path(QModelIndex)));
    connect(add_but, SIGNAL(pressed()), this, SLOT(add_app_pressed()));
    connect(quit_but, SIGNAL(pressed()), this, SLOT(quit_pressed()));
    connect(completer, SIGNAL(activated(QModelIndex)), this, SLOT(update_add_tree(QModelIndex)));
    connect(app_path, SIGNAL(returnPressed()), this, SLOT(path_completer()));
}

void Manager::remove_app_tab()
{
    QFrame *rem_frm = new QFrame(this);
    rem_frm->setFrameStyle(QFrame::Panel);
    tab->addTab(rem_frm, QString("Remove application"));
    QVBoxLayout *rem_layout = new QVBoxLayout(this);
    rem_frm->setLayout(rem_layout);

    app_tree = new QTreeWidget(this);
    app_tree->setColumnCount(1);
    app_tree->setHeaderLabel("Category/Applications");
    update_remove_list();

    QGridLayout *rem_grid = new QGridLayout(this);
    QPushButton *rem_but = new QPushButton("Remove", this);
    QPushButton* quit_but = new QPushButton("Quit", this);
    rem_grid->addWidget(rem_but, 0, 0);
    rem_grid->addWidget(quit_but, 0, 1);

    rem_layout->addWidget(app_tree);
    rem_layout->addLayout(rem_grid);

    connect(rem_but, SIGNAL(pressed()), this, SLOT(remove_app_pressed()));
    connect(quit_but, SIGNAL(pressed()), this, SLOT(quit_pressed()));
}

void Manager::update_add_tree(const QModelIndex &index)
{
    tree_view->scrollTo(index);
    tree_view->setCurrentIndex(index);
}

void Manager::path_completer() // on user button press in line_path 
{
    QModelIndex index = dir_model->index(app_path->text());
    
    if(index.isValid())
    {
        tree_view->setCurrentIndex(index);
    }
}

void Manager::run_app_tab()
{
    QFrame *run_frm = new QFrame(this);
    run_frm->setFrameStyle(QFrame::Panel);
    tab->addTab(run_frm, QString("Run at startup"));
    QVBoxLayout *run_layout = new QVBoxLayout(this);
    run_frm->setLayout(run_layout);

    run_list = new QListWidget(this);
    update_run_list();

    QGridLayout *run_grid = new QGridLayout(this);
    QPushButton *add_but = new QPushButton("Add", this);
    QPushButton *rem_but = new QPushButton("Remove", this);
    QPushButton* quit_but = new QPushButton("Quit", this);
    run_grid->addWidget(add_but, 0, 0);
    run_grid->addWidget(rem_but, 0, 1);
    run_grid->addWidget(quit_but, 0, 2);

    run_layout->addWidget(run_list);
    run_layout->addLayout(run_grid);

    connect(add_but, SIGNAL(pressed()), this, SLOT(add_run_app_pressed()));
    connect(rem_but, SIGNAL(pressed()), this, SLOT(remove_run_app_pressed()));
    connect(quit_but, SIGNAL(pressed()), this, SLOT(quit_pressed()));
}

void Manager::style_tab()
{
    QFrame *style_frm = new QFrame(this);
    tab->addTab(style_frm, QString("Style"));
    style_frm->setFrameStyle(QFrame::Panel);
    QVBoxLayout *style_layout = new QVBoxLayout(this);
    style_frm->setLayout(style_layout);
    ///////// STYLE SELECTION /////////
    QGridLayout *style_sel_layout = new QGridLayout(this);
    style_layout->addLayout(style_sel_layout);
    QLabel *style_sel_lb = new QLabel(QString("Actual style:"), this);
    style_sel_tx = new QLineEdit(this);
    style_sel_tx->setReadOnly(true);
    QPushButton *style_sel_but = new QPushButton("...", this);
    style_sel_but->setMaximumWidth(50);
    style_sel_layout->addWidget(style_sel_lb, 0, 0);
    style_sel_layout->addWidget(style_sel_tx, 0, 1);
    style_sel_layout->addWidget(style_sel_but, 0, 2);
    connect(style_sel_but, SIGNAL(clicked()), this, SLOT(select_style()));
    /////////////////////////
    QToolBox *tool_box = new QToolBox(this);
    style_layout->addWidget(tool_box);
    ///////// FRAME /////////
    QGroupBox *frame_box = new QGroupBox(this);
    QGridLayout *frame_layout = new QGridLayout(this);
    frame_box->setLayout(frame_layout);
    frame_layout->setColumnMinimumWidth(0, 50);
    frame_layout->setColumnMinimumWidth(1, 75);
    frame_layout->setColumnMinimumWidth(2, 75);
    QLabel *top_bdr_lb = new QLabel(QString("Top border height:"), this);
    QLabel *lateral_bdr_lb = new QLabel(QString("Lateral border width:"), this);
    QLabel *bottom_bdr_lb = new QLabel(QString("Bottom border height:"), this);
    QLabel *header_active_pix_lb = new QLabel(QString("Header active pixmap:"), this);
    QLabel *header_inactive_pix_lb = new QLabel(QString("Header inactive pixmap:"), this);
    QLabel *header_color = new QLabel(QString("Title color:"), this);
    QLabel *minmax_pix_lb = new QLabel(QString("Minimize/Maximize pixmap:"), this);
    QLabel *close_pix_lb = new QLabel(QString("Close pixmap:"), this);
    header_active_pix = new QLabel(this);
    header_inactive_pix = new QLabel(this);
    header_active_pix->setMaximumSize(32, 32);
    header_inactive_pix->setMaximumSize(32, 32);
    header_active_pix->setScaledContents(true);
    header_inactive_pix->setScaledContents(true);
    header_col_lab = new QLabel(this);
    header_col_lab->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    header_col_lab->setMaximumWidth(32);
    minmax_pix = new QLabel(this);
    minmax_pix->setMaximumSize(32, 32);
    minmax_pix->setScaledContents(true);
    close_pix = new QLabel(this);
    close_pix->setMaximumSize(32, 32);
    close_pix->setScaledContents(true);
    top_bdr_spinBox = new QSpinBox(this);
    top_bdr_spinBox->setMaximumWidth(50);
    lateral_bdr_spinBox = new QSpinBox(this);
    lateral_bdr_spinBox->setMaximumWidth(50);
    bottom_bdr_spinBox = new QSpinBox(this);
    bottom_bdr_spinBox->setMaximumWidth(50);
    QPushButton *header_active_pix_but = new QPushButton("...", this);
    QPushButton *header_inactive_pix_but = new QPushButton("...", this);
    header_active_pix_but->setMaximumWidth(50);
    header_inactive_pix_but->setMaximumWidth(50);
    QPushButton *minmax_pix_but = new QPushButton("...", this);
    minmax_pix_but->setMaximumWidth(50);
    QPushButton *close_pix_but = new QPushButton("...", this);
    close_pix_but->setMaximumWidth(50);
    QPushButton *header_col_but = new QPushButton("...", this);
    header_col_but->setMaximumWidth(50);
    frame_layout->addWidget(top_bdr_lb, 0, 0);
    frame_layout->addWidget(lateral_bdr_lb, 1, 0);
    frame_layout->addWidget(bottom_bdr_lb, 2, 0);
    frame_layout->addWidget(header_active_pix_lb, 3, 0);
    frame_layout->addWidget(header_inactive_pix_lb, 4, 0);
    frame_layout->addWidget(header_color, 5, 0);
    frame_layout->addWidget(minmax_pix_lb, 6, 0);
    frame_layout->addWidget(close_pix_lb, 7, 0);
    frame_layout->addWidget(top_bdr_spinBox, 0, 1);
    frame_layout->addWidget(lateral_bdr_spinBox, 1, 1);
    frame_layout->addWidget(bottom_bdr_spinBox, 2, 1);
    frame_layout->addWidget(header_active_pix, 3, 1);
    frame_layout->addWidget(header_inactive_pix, 4, 1);
    frame_layout->addWidget(header_col_lab, 5, 1);
    frame_layout->addWidget(minmax_pix, 6, 1);
    frame_layout->addWidget(close_pix, 7, 1);
    frame_layout->addWidget(header_active_pix_but, 3, 2);
    frame_layout->addWidget(header_inactive_pix_but, 4, 2);
    frame_layout->addWidget(header_col_but, 5, 2);
    frame_layout->addWidget(minmax_pix_but, 6, 2);
    frame_layout->addWidget(close_pix_but, 7, 2);
    ///////// DOCKBAR /////////
    QGroupBox *dockbar_box = new QGroupBox(this);
    QGridLayout *dockbar_layout = new QGridLayout(this);
    dockbar_box->setLayout(dockbar_layout);
    dockbar_layout->setColumnMinimumWidth(0, 50);
    dockbar_layout->setColumnMinimumWidth(1, 75);
    dockbar_layout->setColumnMinimumWidth(2, 75);
    QLabel *dockbar_height_lb = new QLabel(QString("Height:"), this);
    dockbar_height_spinBox = new QSpinBox(this);
    dockbar_height_spinBox->setMaximumWidth(50);
    QLabel *dockbar_pix_lb = new QLabel(QString("Pixmap:"), this);
    dockbar_pix = new QLabel(this);
    dockbar_pix->setMaximumSize(32, 32);
    dockbar_pix->setScaledContents(true);
    QPushButton *dockbar_pix_but = new QPushButton("...", this);
    dockbar_pix_but->setMaximumWidth(50);
    dockbar_layout->addWidget(dockbar_height_lb, 0, 0);
    dockbar_layout->addWidget(dockbar_height_spinBox, 0, 1);
    dockbar_layout->addWidget(dockbar_pix_lb, 1, 0);
    dockbar_layout->addWidget(dockbar_pix, 1, 1);
    dockbar_layout->addWidget(dockbar_pix_but, 1, 2);
    ///////// DOCKICON /////////
    QGroupBox *dockicon_box = new QGroupBox(this);
    QGridLayout *dockicon_layout = new QGridLayout(this);
    dockicon_box->setLayout(dockicon_layout);
    dockicon_layout->setColumnMinimumWidth(0, 50);
    dockicon_layout->setColumnMinimumWidth(1, 75);
    dockicon_layout->setColumnMinimumWidth(2, 75);
    QLabel *dockicon_pix_lb = new QLabel(QString("Pixmap:"), this);
    dockicon_pix = new QLabel(this);
    dockicon_pix->setMaximumSize(32, 32);
    dockicon_pix->setScaledContents(true);
    QLabel *dockicon_color = new QLabel(QString("Title color:"), this);
    dockicon_col_lab = new QLabel(this);
    dockicon_col_lab->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    dockicon_col_lab->setMaximumWidth(32);
    QPushButton *dockicon_pix_but = new QPushButton("...");
    dockicon_pix_but->setMaximumWidth(50);
    QPushButton *dockicon_col_but = new QPushButton("...");
    dockicon_col_but->setMaximumWidth(50);
    dockicon_layout->addWidget(dockicon_pix_lb, 0, 0);
    dockicon_layout->addWidget(dockicon_pix, 0, 1);
    dockicon_layout->addWidget(dockicon_pix_but, 0, 2);
    dockicon_layout->addWidget(dockicon_color, 1, 0);
    dockicon_layout->addWidget(dockicon_col_lab, 1, 1);
    dockicon_layout->addWidget(dockicon_col_but, 1, 2);
    ///////// DESKFOLDER /////////
    QGroupBox *deskfolder_box = new QGroupBox(this);
    QGridLayout *deskfolder_layout = new QGridLayout(this);
    deskfolder_box->setLayout(deskfolder_layout);
    deskfolder_layout->setColumnMinimumWidth(0, 50);
    deskfolder_layout->setColumnMinimumWidth(1, 75);
    deskfolder_layout->setColumnMinimumWidth(2, 75);
    QLabel *deskfolder_pix_lb = new QLabel(QString("Pixmap:"), this);
    deskfolder_pix = new QLabel(this);
    deskfolder_pix->setMaximumSize(32, 32);
    deskfolder_pix->setScaledContents(true);
    QLabel *deskfolder_color = new QLabel(QString("Name color:"), this);
    deskfolder_col_lab = new QLabel(this);
    deskfolder_col_lab->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    deskfolder_col_lab->setMaximumWidth(32);
    QPushButton *deskfolder_pix_but = new QPushButton("...", this);
    deskfolder_pix_but->setMaximumWidth(50);
    QPushButton *deskfolder_col_but = new QPushButton("...", this);
    deskfolder_col_but->setMaximumWidth(50);
    deskfolder_layout->addWidget(deskfolder_pix_lb, 0, 0);
    deskfolder_layout->addWidget(deskfolder_pix, 0, 1);
    deskfolder_layout->addWidget(deskfolder_pix_but, 0, 2);
    deskfolder_layout->addWidget(deskfolder_color, 1, 0);
    deskfolder_layout->addWidget(deskfolder_col_lab, 1, 1);
    deskfolder_layout->addWidget(deskfolder_col_but, 1, 2);
    ///////// DESKFILE /////////
    QGroupBox *deskfile_box = new QGroupBox(this);
    QGridLayout *deskfile_layout = new QGridLayout(this);
    deskfile_box->setLayout(deskfile_layout);
    deskfile_layout->setColumnMinimumWidth(0, 50);
    deskfile_layout->setColumnMinimumWidth(1, 75);
    deskfile_layout->setColumnMinimumWidth(2, 75);
    QLabel *deskfile_color = new QLabel(QString("Name color:"), this);
    deskfile_col_lab = new QLabel(this);
    deskfile_col_lab->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    deskfile_col_lab->setMaximumWidth(32);
    QPushButton *deskfile_col_but = new QPushButton("...", this);
    deskfile_col_but->setMaximumWidth(50);
    deskfile_layout->addWidget(deskfile_color, 0, 0);
    deskfile_layout->addWidget(deskfile_col_lab, 0, 1);
    deskfile_layout->addWidget(deskfile_col_but, 0, 2);
    ///////// DESKDEV /////////
    QGroupBox *deskdev_box = new QGroupBox(this);
    QGridLayout *deskdev_layout = new QGridLayout(this);
    deskdev_box->setLayout(deskdev_layout);
    deskdev_layout->setColumnMinimumWidth(0, 50);
    deskdev_layout->setColumnMinimumWidth(1, 75);
    deskdev_layout->setColumnMinimumWidth(2, 75);
    QLabel *deskdev_disk_pix_lb = new QLabel(QString("Disk pixmap:"), this);
    deskdev_disk_pix = new QLabel(this);
    deskdev_disk_pix->setMaximumSize(32, 32);
    deskdev_disk_pix->setScaledContents(true);
    QLabel *deskdev_cdrom_pix_lb = new QLabel(QString("CDRom pixmap:"), this);
    deskdev_cdrom_pix = new QLabel(this);
    deskdev_cdrom_pix->setMaximumSize(32, 32);
    deskdev_cdrom_pix->setScaledContents(true);
    QLabel *deskdev_color = new QLabel(QString("Name color:"), this);
    deskdev_col_lab = new QLabel(this);
    deskdev_col_lab->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    deskdev_col_lab->setMaximumWidth(32);
    QPushButton *deskdev_disk_pix_but = new QPushButton("...", this);
    deskdev_disk_pix_but->setMaximumWidth(50);
    QPushButton *deskdev_cdrom_pix_but = new QPushButton("...", this);
    deskdev_cdrom_pix_but->setMaximumWidth(50);
    QPushButton *deskdev_col_but = new QPushButton("...", this);
    deskdev_col_but->setMaximumWidth(50);
    deskdev_layout->addWidget(deskdev_disk_pix_lb, 0, 0);
    deskdev_layout->addWidget(deskdev_disk_pix, 0, 1);
    deskdev_layout->addWidget(deskdev_disk_pix_but, 0, 2);
    deskdev_layout->addWidget(deskdev_cdrom_pix_lb, 1, 0);
    deskdev_layout->addWidget(deskdev_cdrom_pix, 1, 1);
    deskdev_layout->addWidget(deskdev_cdrom_pix_but, 1, 2);
    deskdev_layout->addWidget(deskdev_color, 2, 0);
    deskdev_layout->addWidget(deskdev_col_lab, 2, 1);
    deskdev_layout->addWidget(deskdev_col_but, 2, 2);
    ///////// SYSICON /////////
    QGroupBox *sysicon_box = new QGroupBox(this);
    QGridLayout *sysicon_layout = new QGridLayout(this);
    sysicon_box->setLayout(sysicon_layout);
    sysicon_layout->setColumnMinimumWidth(0, 50);
    sysicon_layout->setColumnMinimumWidth(1, 75);
    sysicon_layout->setColumnMinimumWidth(2, 75);
    QLabel *sysicon_pix_lb = new QLabel(QString("Pixmap:"), this);
    sysicon_pix = new QLabel(this);
    sysicon_pix->setMaximumSize(32, 32);
    sysicon_pix->setScaledContents(true);
    QPushButton *sysicon_pix_but = new QPushButton("...", this);
    sysicon_pix_but->setMaximumWidth(50);
    sysicon_layout->addWidget(sysicon_pix_lb, 0, 0);
    sysicon_layout->addWidget(sysicon_pix, 0, 1);
    sysicon_layout->addWidget(sysicon_pix_but, 0, 2);
    ///////// DESKAPP /////////
    QGroupBox *deskapp_box = new QGroupBox(this);
    QGridLayout *deskapp_layout = new QGridLayout(this);
    deskapp_box->setLayout(deskapp_layout);
    deskapp_layout->setColumnMinimumWidth(0, 50);
    deskapp_layout->setColumnMinimumWidth(1, 75);
    deskapp_layout->setColumnMinimumWidth(2, 75);
    QLabel *deskapp_color = new QLabel(QString("Name color:"), this);
    deskapp_col_lab = new QLabel(this);
    deskapp_col_lab->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    deskapp_col_lab->setMaximumWidth(32);
    QPushButton *deskapp_col_but = new QPushButton("...", this);
    deskapp_col_but->setMaximumWidth(50);
    deskapp_layout->addWidget(deskapp_color, 0, 0);
    deskapp_layout->addWidget(deskapp_col_lab, 0, 1);
    deskapp_layout->addWidget(deskapp_col_but, 0, 2);
    ///////// DATECLOCK /////////
    QGroupBox *dateclock_box = new QGroupBox(this);
    QGridLayout *dateclock_layout = new QGridLayout(this);
    dateclock_box->setLayout(dateclock_layout);
    dateclock_layout->setColumnMinimumWidth(0, 50);
    dateclock_layout->setColumnMinimumWidth(1, 75);
    dateclock_layout->setColumnMinimumWidth(2, 75);
    QLabel *date_color = new QLabel(QString("Date color:"), this);
    QLabel *clock_color = new QLabel(QString("Clock color:"), this);
    date_col_lab = new QLabel(this);
    clock_col_lab = new QLabel(this);
    date_col_lab->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    clock_col_lab->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    date_col_lab->setMaximumWidth(32);
    clock_col_lab->setMaximumWidth(32);
    QPushButton *date_col_but = new QPushButton("...", this);
    QPushButton *clock_col_but = new QPushButton("...", this);
    date_col_but->setMaximumWidth(50);
    clock_col_but->setMaximumWidth(50);
    dateclock_layout->addWidget(date_color, 1, 0);
    dateclock_layout->addWidget(date_col_lab, 1, 1);
    dateclock_layout->addWidget(date_col_but, 1, 2);
    dateclock_layout->addWidget(clock_color, 0, 0);
    dateclock_layout->addWidget(clock_col_lab, 0, 1);
    dateclock_layout->addWidget(clock_col_but, 0, 2);
    ///////// DESKTOP /////////
    QGroupBox *desktop_box = new QGroupBox(this);
    QGridLayout *desktop_layout = new QGridLayout(this);
    desktop_box->setLayout(desktop_layout);
    QLabel *desktop_pix_lb = new QLabel(QString("Wallpaper:"), this);
    desktop_pix = new QLabel(this);
    desktop_pix->setMinimumSize(100, 100);
    desktop_pix->setScaledContents(true);
    QPushButton *desktop_pix_but = new QPushButton("...", this);
    desktop_pix_but->setMaximumWidth(50);
    desktop_layout->addWidget(desktop_pix_lb, 0, 0);
    desktop_layout->addWidget(desktop_pix, 0, 1);
    desktop_layout->addWidget(desktop_pix_but, 0, 2);
    ///////// LAUNCHER /////////
    QGroupBox *launcher_box = new QGroupBox(this);
    QGridLayout *launcher_layout = new QGridLayout(this);
    launcher_box->setLayout(launcher_layout);
    launcher_layout->setColumnMinimumWidth(0, 50);
    launcher_layout->setColumnMinimumWidth(1, 75);
    launcher_layout->setColumnMinimumWidth(2, 75);
    QLabel *launcher_pix_lb = new QLabel(QString("Launcher pixmap:"), this);
    QLabel *application_pix_lb = new QLabel(QString("Application pixmap:"), this);
    QLabel *quit_pix_lb = new QLabel(QString("Quit pixmap:"), this);
    QLabel *shutdown_pix_lb = new QLabel(QString("Shutdown pixmap:"), this);
    QLabel *restart_pix_lb = new QLabel(QString("Restart pixmap:"), this);
    QLabel *refresh_pix_lb = new QLabel(QString("Refresh pixmap:"), this);
    QLabel *run_pix_lb = new QLabel(QString("Run pixmap:"), this);
    QLabel *show_pix_lb = new QLabel(QString("Show Desktop pixmap:"), this);
    QLabel *manager_pix_lb = new QLabel(QString("Manager pixmap:"), this);
    QLabel *utility_pix_lb = new QLabel(QString("Utility pixmap:"), this);
    QLabel *office_pix_lb = new QLabel(QString("Office pixmap:"), this);
    QLabel *internet_pix_lb = new QLabel(QString("Internet pixmap:"), this);
    QLabel *graphic_pix_lb = new QLabel(QString("Graphic pixmap:"), this);
    QLabel *development_pix_lb = new QLabel(QString("Development pixmap:"), this);
    QLabel *system_pix_lb = new QLabel(QString("System pixmap:"), this);
    QLabel *audio_pix_lb = new QLabel(QString("Audio pixmap:"), this);
    QLabel *video_pix_lb = new QLabel(QString("Video pixmap:"), this);
    launcher_pix = new QLabel(this);
    application_pix = new QLabel(this);
    quit_pix = new QLabel(this);
    shutdown_pix = new QLabel(this);
    restart_pix = new QLabel(this);
    refresh_pix = new QLabel(this);
    run_pix = new QLabel(this);
    show_pix = new QLabel(this);
    manager_pix = new QLabel(this);
    utility_pix = new QLabel(this);
    office_pix = new QLabel(this);
    internet_pix = new QLabel(this);
    graphic_pix = new QLabel(this);
    development_pix = new QLabel(this);
    system_pix = new QLabel(this);
    audio_pix = new QLabel(this);
    video_pix = new QLabel(this);
    launcher_pix->setMaximumSize(32, 32);
    application_pix->setMaximumSize(32, 32);
    quit_pix->setMaximumSize(32, 32);
    shutdown_pix->setMaximumSize(32, 32);
    restart_pix->setMaximumSize(32, 32);
    refresh_pix->setMaximumSize(32, 32);
    run_pix->setMaximumSize(32, 32);
    show_pix->setMaximumSize(32, 32);
    manager_pix->setMaximumSize(32, 32);
    utility_pix->setMaximumSize(32, 32);
    office_pix->setMaximumSize(32, 32);
    internet_pix->setMaximumSize(32, 32);
    graphic_pix->setMaximumSize(32, 32);
    development_pix->setMaximumSize(32, 32);
    system_pix->setMaximumSize(32, 32);
    audio_pix->setMaximumSize(32, 32);
    video_pix->setMaximumSize(32, 32);
    launcher_pix->setScaledContents(true);
    application_pix->setScaledContents(true);
    quit_pix->setScaledContents(true);
    shutdown_pix->setScaledContents(true);
    restart_pix->setScaledContents(true);
    refresh_pix->setScaledContents(true);
    run_pix->setScaledContents(true);
    show_pix->setScaledContents(true);
    manager_pix->setScaledContents(true);
    utility_pix->setScaledContents(true);
    office_pix->setScaledContents(true);
    internet_pix->setScaledContents(true);
    graphic_pix->setScaledContents(true);
    development_pix->setScaledContents(true);
    system_pix->setScaledContents(true);
    audio_pix->setScaledContents(true);
    video_pix->setScaledContents(true);
    QPushButton *launcher_pix_but = new QPushButton("...", this);
    QPushButton *application_pix_but = new QPushButton("...", this);
    QPushButton *quit_pix_but = new QPushButton("...", this);
    QPushButton *shutdown_pix_but = new QPushButton("...", this);
    QPushButton *restart_pix_but = new QPushButton("...", this);
    QPushButton *refresh_pix_but = new QPushButton("...", this);
    QPushButton *run_pix_but = new QPushButton("...", this);
    QPushButton *show_pix_but = new QPushButton("...", this);
    QPushButton *manager_pix_but = new QPushButton("...", this);
    QPushButton *utility_pix_but = new QPushButton("...", this);
    QPushButton *office_pix_but = new QPushButton("...", this);
    QPushButton *internet_pix_but = new QPushButton("...", this);
    QPushButton *graphic_pix_but = new QPushButton("...", this);
    QPushButton *development_pix_but = new QPushButton("...", this);
    QPushButton *system_pix_but = new QPushButton("...", this);
    QPushButton *audio_pix_but = new QPushButton("...", this);
    QPushButton *video_pix_but = new QPushButton("...", this);
    launcher_pix_but->setMaximumWidth(50);
    application_pix_but->setMaximumWidth(50);
    quit_pix_but->setMaximumWidth(50);
    shutdown_pix_but->setMaximumWidth(50);
    restart_pix_but->setMaximumWidth(50);
    refresh_pix_but->setMaximumWidth(50);
    run_pix_but->setMaximumWidth(50);
    show_pix_but->setMaximumWidth(50);
    manager_pix_but->setMaximumWidth(50);
    utility_pix_but->setMaximumWidth(50);
    office_pix_but->setMaximumWidth(50);
    internet_pix_but->setMaximumWidth(50);
    graphic_pix_but->setMaximumWidth(50);
    development_pix_but->setMaximumWidth(50);
    system_pix_but->setMaximumWidth(50);
    audio_pix_but->setMaximumWidth(50);
    video_pix_but->setMaximumWidth(50);
    launcher_layout->addWidget(launcher_pix_lb, 0, 0);
    launcher_layout->addWidget(application_pix_lb, 1, 0);
    launcher_layout->addWidget(quit_pix_lb, 2, 0);
    launcher_layout->addWidget(shutdown_pix_lb, 3, 0);
    launcher_layout->addWidget(restart_pix_lb, 4, 0);
    launcher_layout->addWidget(refresh_pix_lb, 5, 0);
    launcher_layout->addWidget(run_pix_lb, 6, 0);
    launcher_layout->addWidget(show_pix_lb, 7, 0);
    launcher_layout->addWidget(manager_pix_lb, 8, 0);
    launcher_layout->addWidget(utility_pix_lb, 9, 0);
    launcher_layout->addWidget(office_pix_lb, 10, 0);
    launcher_layout->addWidget(internet_pix_lb, 11, 0);
    launcher_layout->addWidget(graphic_pix_lb, 12, 0);
    launcher_layout->addWidget(development_pix_lb, 13, 0);
    launcher_layout->addWidget(system_pix_lb, 14, 0);
    launcher_layout->addWidget(audio_pix_lb, 15, 0);
    launcher_layout->addWidget(video_pix_lb, 16, 0);
    launcher_layout->addWidget(launcher_pix, 0, 1);
    launcher_layout->addWidget(application_pix, 1, 1);
    launcher_layout->addWidget(quit_pix, 2, 1);
    launcher_layout->addWidget(shutdown_pix, 3, 1);
    launcher_layout->addWidget(restart_pix, 4, 1);
    launcher_layout->addWidget(refresh_pix, 5, 1);
    launcher_layout->addWidget(run_pix, 6, 1);
    launcher_layout->addWidget(show_pix, 7, 1);
    launcher_layout->addWidget(manager_pix, 8, 1);
    launcher_layout->addWidget(utility_pix, 9, 1);
    launcher_layout->addWidget(office_pix, 10, 1);
    launcher_layout->addWidget(internet_pix, 11, 1);
    launcher_layout->addWidget(graphic_pix, 12, 1);
    launcher_layout->addWidget(development_pix, 13, 1);
    launcher_layout->addWidget(system_pix, 14, 1);
    launcher_layout->addWidget(audio_pix, 15, 1);
    launcher_layout->addWidget(video_pix, 16, 1);
    launcher_layout->addWidget(launcher_pix_but, 0, 2);
    launcher_layout->addWidget(application_pix_but, 1, 2);
    launcher_layout->addWidget(quit_pix_but, 2, 2);
    launcher_layout->addWidget(shutdown_pix_but, 3, 2);
    launcher_layout->addWidget(restart_pix_but, 4, 2);
    launcher_layout->addWidget(refresh_pix_but, 5, 2);
    launcher_layout->addWidget(run_pix_but, 6, 2);
    launcher_layout->addWidget(show_pix_but, 7, 2);
    launcher_layout->addWidget(manager_pix_but, 8, 2);
    launcher_layout->addWidget(utility_pix_but, 9, 2);
    launcher_layout->addWidget(office_pix_but, 10, 2);
    launcher_layout->addWidget(internet_pix_but, 11, 2);
    launcher_layout->addWidget(graphic_pix_but, 12, 2);
    launcher_layout->addWidget(development_pix_but, 13, 2);
    launcher_layout->addWidget(system_pix_but, 14, 2);
    launcher_layout->addWidget(audio_pix_but, 15, 2);
    launcher_layout->addWidget(video_pix_but, 16, 2);
    ///////// OTHER /////////
    QGroupBox *other_box = new QGroupBox(this);
    QGridLayout *other_layout = new QGridLayout(this);
    other_box->setLayout(other_layout);
    other_layout->setColumnMinimumWidth(0, 50);
    other_layout->setColumnMinimumWidth(1, 75);
    other_layout->setColumnMinimumWidth(2, 75);
    QLabel *folder_link_pix_lb = new QLabel(QString("Folder link pixmap:"), this);
    QLabel *file_link_pix_lb = new QLabel(QString("File link pixmap:"), this);
    QLabel *app_link_pix_lb = new QLabel(QString("Application link pixmap:"), this);
    QLabel *delete_link_pix_lb = new QLabel(QString("Delete link pixmap:"), this);
    QLabel *delete_file_pix_lb = new QLabel(QString("Delete file pixmap:"), this);
    QLabel *close_dock_pix_lb = new QLabel(QString("Close Dock pixmap:"), this);
    QLabel *add_to_sys_pix_lb = new QLabel(QString("Add to SysTray pixmap:"), this);
    QLabel *open_with_pix_lb = new QLabel(QString("Open with pixmap:"), this);
    folder_link_pix = new QLabel(this);
    file_link_pix = new QLabel(this);
    app_link_pix = new QLabel(this);
    delete_link_pix = new QLabel(this);
    delete_file_pix = new QLabel(this);
    close_dock_pix = new QLabel(this);
    add_to_sys_pix = new QLabel(this);
    open_with_pix = new QLabel(this);
    folder_link_pix->setMaximumSize(32, 32);
    file_link_pix->setMaximumSize(32, 32);
    app_link_pix->setMaximumSize(32, 32);
    delete_link_pix->setMaximumSize(32, 32);
    delete_file_pix->setMaximumSize(32, 32);
    close_dock_pix->setMaximumSize(32, 32);
    add_to_sys_pix->setMaximumSize(32, 32);
    open_with_pix->setMaximumSize(32, 32);
    folder_link_pix->setScaledContents(true);
    file_link_pix->setScaledContents(true);
    app_link_pix->setScaledContents(true);
    delete_link_pix->setScaledContents(true);
    delete_file_pix->setScaledContents(true);
    close_dock_pix->setScaledContents(true);
    add_to_sys_pix->setScaledContents(true);
    open_with_pix->setScaledContents(true);
    QPushButton *folder_link_pix_but = new QPushButton("...", this);
    QPushButton *file_link_pix_but = new QPushButton("...", this);
    QPushButton *app_link_pix_but = new QPushButton("...", this);
    QPushButton *delete_link_pix_but = new QPushButton("...", this);
    QPushButton *delete_file_pix_but = new QPushButton("...", this);
    QPushButton *close_dock_pix_but = new QPushButton("...", this);
    QPushButton *add_to_sys_pix_but = new QPushButton("...", this);
    QPushButton *open_with_pix_but = new QPushButton("...", this);
    folder_link_pix_but->setMaximumWidth(50);
    file_link_pix_but->setMaximumWidth(50);
    app_link_pix_but->setMaximumWidth(50);
    delete_link_pix_but->setMaximumWidth(50);
    delete_file_pix_but->setMaximumWidth(50);
    close_dock_pix_but->setMaximumWidth(50);
    add_to_sys_pix_but->setMaximumWidth(50);
    open_with_pix_but->setMaximumWidth(50);
    other_layout->addWidget(folder_link_pix_lb, 0, 0);
    other_layout->addWidget(file_link_pix_lb, 1, 0);
    other_layout->addWidget(app_link_pix_lb, 2, 0);
    other_layout->addWidget(delete_link_pix_lb, 3, 0);
    other_layout->addWidget(delete_file_pix_lb, 4, 0);
    other_layout->addWidget(close_dock_pix_lb, 5, 0);
    other_layout->addWidget(add_to_sys_pix_lb, 6, 0);
    other_layout->addWidget(open_with_pix_lb, 7, 0);
    other_layout->addWidget(folder_link_pix, 0, 1);
    other_layout->addWidget(file_link_pix, 1, 1);
    other_layout->addWidget(app_link_pix, 2, 1);
    other_layout->addWidget(delete_link_pix, 3, 1);
    other_layout->addWidget(delete_file_pix, 4, 1);
    other_layout->addWidget(close_dock_pix, 5, 1);
    other_layout->addWidget(add_to_sys_pix, 6, 1);
    other_layout->addWidget(open_with_pix, 7, 1);
    other_layout->addWidget(folder_link_pix_but, 0, 2);
    other_layout->addWidget(file_link_pix_but, 1, 2);
    other_layout->addWidget(app_link_pix_but, 2, 2);
    other_layout->addWidget(delete_link_pix_but, 3, 2);
    other_layout->addWidget(delete_file_pix_but, 4, 2);
    other_layout->addWidget(close_dock_pix_but, 5, 2);
    other_layout->addWidget(add_to_sys_pix_but, 6, 2);
    other_layout->addWidget(open_with_pix_but, 7, 2);
    ///////// OK-QUIT /////////
    QGroupBox *ok_quit_box = new QGroupBox(this);
    style_layout->addWidget(ok_quit_box);
    QGridLayout *ok_quit_layout = new QGridLayout(this);
    ok_quit_box->setLayout(ok_quit_layout);
    QPushButton *ok_but = new QPushButton("OK", this);
    QPushButton* quit_but = new QPushButton("Quit", this);
    ok_but->setMaximumWidth(75);
    quit_but->setMaximumWidth(75);
    ok_quit_layout->addWidget(ok_but, 0, 0);
    ok_quit_layout->addWidget(quit_but, 0, 1);
    /////////////////////////////////////////////////////////
    tool_box->addItem(frame_box, "Frame settings");
    tool_box->addItem(dockbar_box, "Dockbar settings");
    tool_box->addItem(dockicon_box, "Dockbar icon");
    tool_box->addItem(desktop_box, "Desktop wallpaper");
    tool_box->addItem(deskfolder_box, "Desktop folder");
    tool_box->addItem(deskfile_box, "Desktop file");
    tool_box->addItem(deskapp_box, "Desktop application");
    tool_box->addItem(deskdev_box, "Desktop device");
    tool_box->addItem(sysicon_box, "System Tray icon");
    tool_box->addItem(dateclock_box, "Clock/Date");
    tool_box->addItem(launcher_box, "Launcher");
    tool_box->addItem(other_box, "Other");
    /////////////////////////////////////////////////////////

    connect(ok_but, SIGNAL(clicked()), this, SLOT(ok_frame_pressed()));
    connect(quit_but, SIGNAL(clicked()), this, SLOT(quit_pressed()));

    // mapper for pixmap selection (dockbar - dockicon - sysicon - deskfolder - deskdev - desktop - frame header active/inactive - frame button - all laucher button - other)
    QSignalMapper *pixmapMapper = new QSignalMapper(this);
    pixmapMapper->setMapping(dockbar_pix_but, dockbar_pix);
    pixmapMapper->setMapping(desktop_pix_but, desktop_pix);
    pixmapMapper->setMapping(dockicon_pix_but, dockicon_pix);
    pixmapMapper->setMapping(sysicon_pix_but, sysicon_pix);
    pixmapMapper->setMapping(deskfolder_pix_but, deskfolder_pix);
    pixmapMapper->setMapping(deskdev_disk_pix_but, deskdev_disk_pix);
    pixmapMapper->setMapping(deskdev_cdrom_pix_but, deskdev_cdrom_pix);
    pixmapMapper->setMapping(header_active_pix_but, header_active_pix);
    pixmapMapper->setMapping(header_inactive_pix_but, header_inactive_pix);
    pixmapMapper->setMapping(minmax_pix_but, minmax_pix);
    pixmapMapper->setMapping(close_pix_but, close_pix);
    ///////// Launcher //////////
    pixmapMapper->setMapping(launcher_pix_but, launcher_pix);
    pixmapMapper->setMapping(application_pix_but, application_pix);
    pixmapMapper->setMapping(quit_pix_but, quit_pix);
    pixmapMapper->setMapping(shutdown_pix_but, shutdown_pix);
    pixmapMapper->setMapping(restart_pix_but, restart_pix);
    pixmapMapper->setMapping(refresh_pix_but, refresh_pix);
    pixmapMapper->setMapping(run_pix_but, run_pix);
    pixmapMapper->setMapping(show_pix_but, show_pix);
    pixmapMapper->setMapping(manager_pix_but, manager_pix);
    pixmapMapper->setMapping(utility_pix_but, utility_pix);
    pixmapMapper->setMapping(office_pix_but, office_pix);
    pixmapMapper->setMapping(internet_pix_but, internet_pix);
    pixmapMapper->setMapping(graphic_pix_but, graphic_pix);
    pixmapMapper->setMapping(development_pix_but, development_pix);
    pixmapMapper->setMapping(system_pix_but, system_pix);
    pixmapMapper->setMapping(audio_pix_but, audio_pix);
    pixmapMapper->setMapping(video_pix_but, video_pix);
    ///////// Other //////////
    pixmapMapper->setMapping(folder_link_pix_but, folder_link_pix);
    pixmapMapper->setMapping(file_link_pix_but, file_link_pix);
    pixmapMapper->setMapping(app_link_pix_but, app_link_pix);
    pixmapMapper->setMapping(delete_link_pix_but, delete_link_pix);
    pixmapMapper->setMapping(delete_file_pix_but, delete_file_pix);
    pixmapMapper->setMapping(close_dock_pix_but, close_dock_pix);
    pixmapMapper->setMapping(add_to_sys_pix_but, add_to_sys_pix);
    pixmapMapper->setMapping(open_with_pix_but, open_with_pix);
    //////////////////////////////////////////////////////////////////////
    connect(dockbar_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(desktop_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(dockicon_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(sysicon_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(deskfolder_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(deskdev_disk_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(deskdev_cdrom_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(header_active_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(header_inactive_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(minmax_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(close_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    ///////// Launcher //////////
    connect(launcher_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(application_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(quit_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(shutdown_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(restart_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(refresh_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(run_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(show_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(manager_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(utility_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(office_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(internet_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(graphic_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(development_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(system_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(audio_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(video_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    ///////// Other //////////
    connect(folder_link_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(file_link_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(app_link_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(delete_link_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(delete_file_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(close_dock_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(add_to_sys_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));
    connect(open_with_pix_but, SIGNAL(clicked()), pixmapMapper, SLOT(map()));

    connect(pixmapMapper, SIGNAL(mapped(QWidget *)), this, SLOT(select_pixmap(QWidget *)));

    // mapper for color selection (frame header text - dockicon text - deskfile name - deskfilder name - deskapp name - deskdev name - date/clock)
    QSignalMapper *colorMapper = new QSignalMapper(this);
    colorMapper->setMapping(header_col_but, header_col_lab);
    colorMapper->setMapping(dockicon_col_but, dockicon_col_lab);
    colorMapper->setMapping(deskfolder_col_but, deskfolder_col_lab);
    colorMapper->setMapping(deskfile_col_but, deskfile_col_lab);
    colorMapper->setMapping(deskdev_col_but, deskdev_col_lab);
    colorMapper->setMapping(deskapp_col_but, deskapp_col_lab);
    colorMapper->setMapping(date_col_but, date_col_lab);
    colorMapper->setMapping(clock_col_but, clock_col_lab);
    connect(header_col_but, SIGNAL(clicked()), colorMapper, SLOT(map()));
    connect(dockicon_col_but, SIGNAL(clicked()), colorMapper, SLOT (map()));
    connect(deskfolder_col_but, SIGNAL(clicked()), colorMapper, SLOT (map()));
    connect(deskfile_col_but, SIGNAL(clicked()), colorMapper, SLOT (map()));
    connect(deskdev_col_but, SIGNAL(clicked()), colorMapper, SLOT (map()));
    connect(deskapp_col_but, SIGNAL(clicked()), colorMapper, SLOT (map()));
    connect(date_col_but, SIGNAL(clicked()), colorMapper, SLOT (map()));
    connect(clock_col_but, SIGNAL(clicked()), colorMapper, SLOT (map()));
    connect(colorMapper, SIGNAL(mapped(QWidget *)), this, SLOT(select_color(QWidget *)));
}

void Manager::select_style()
{
    Filedialog *sel_style = new Filedialog("Select the style", "OK_Cancel", this);
    QStringList filters;
    filters << "*.stl";
    sel_style->set_name_filters(filters); // show only style file (.stl)
    sel_style->set_filter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    sel_style->set_path(QCoreApplication::applicationDirPath() + "/theme/");
    sel_style->set_read_only(true);

    if (sel_style->exec() == QDialog::Accepted)
    {
        QString path = sel_style->get_selected_path();
        QString name = sel_style->get_selected_name();
        stl_name = name; // update path and name style
        stl_path = path;
        qDebug() << "Style path:" << stl_path;
        qDebug() << "Style name:" << stl_name;
        style_sel_tx->setText(name); // update style name on box
        style = new QSettings(stl_path + stl_name, QSettings::IniFormat);
        read_settings();
    }
}

void Manager::select_pixmap(QWidget *pix)
{
    QLabel *pixmap = (QLabel *)pix;
    Filedialog *sel_pix = new Filedialog("Select the pixmap", "OK_Cancel", this);
    sel_pix->set_path(stl_path);
    sel_pix->set_filter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    sel_pix->set_read_only(true);

    if (sel_pix->exec())
    {
        QString pix_path = sel_pix->get_selected_path();
        QString pix_name = sel_pix->get_selected_name();
        pixmap->setPixmap(QPixmap(pix_path + pix_name)); // set pixmap
        pixmap->setToolTip(pix_name); // set tooltip (see ok_frame_pressed)
    }
}

void Manager::select_color(QWidget *col)
{
    QLabel *color_lab = (QLabel *)col;
    Colordialog *diag = new Colordialog(color_lab->palette().color(QPalette::Window)); // update colordialog with actual color
    if (diag->exec())
    {
        QColor col = diag->get_color();
        color_lab->setPalette(QPalette(col));
        color_lab->setAutoFillBackground(true);
    }
}

void Manager::ok_frame_pressed()
{
    //////////////////////////////////////////////////////////////////
    antico->beginGroup("Style"); // save the style
    antico->setValue("name", stl_name);
    antico->setValue("path", stl_path);
    antico->endGroup(); //Style
    //////////////////////////////////////////////////////////////////
    style->beginGroup("Frame");
    style->beginGroup("Border");
    style->setValue("top_bdr_height", top_bdr_spinBox->value());
    style->setValue("lateral_bdr_width", lateral_bdr_spinBox->value());
    style->setValue("bottom_bdr_height", bottom_bdr_spinBox->value());
    style->endGroup(); //Border
    //////////////////////////////////////////////////////////////////
    style->beginGroup("Header");
    style->setValue("header_active_pix", header_active_pix->toolTip());
    style->setValue("header_inactive_pix", header_inactive_pix->toolTip());
    style->setValue("title_color", header_col_lab->palette().color(QPalette::Window));
    style->setValue("minmax_pix", minmax_pix->toolTip());
    style->setValue("close_pix", close_pix->toolTip());
    style->endGroup(); //Header
    style->endGroup(); //Frame
    //////////////////////////////////////////////////////////////////
    style->beginGroup("Dockbar");
    style->setValue("dock_pix", dockbar_pix->toolTip());
    style->setValue("dock_height", dockbar_height_spinBox->value());
    style->endGroup(); //Dockbar
    //////////////////////////////////////////////////////////////////
    style->beginGroup("Dockicon");
    style->setValue("d_icon_pix", dockicon_pix->toolTip());
    style->setValue("title_color", dockicon_col_lab->palette().color(QPalette::Window));
    style->endGroup(); //Dockicon
    //////////////////////////////////////////////////////////////////
    style->beginGroup("Sysicon");
    style->setValue("s_icon_pix", sysicon_pix->toolTip());
    style->endGroup(); //Sysicon
    //////////////////////////////////////////////////////////////////
    style->beginGroup("Deskapp");
    style->setValue("name_color", deskapp_col_lab->palette().color(QPalette::Window));
    style->endGroup(); //Deskapp
    //////////////////////////////////////////////////////////////////
    style->beginGroup("Desktop");
    style->beginGroup("Wallpaper");
    style->setValue("wall_pix", desktop_pix->toolTip());
    style->endGroup(); //Wallpaper
    style->endGroup(); //Desktop
    //////////////////////////////////////////////////////////////////
    style->beginGroup("Deskfolder");
    style->setValue("d_folder_pix", deskfolder_pix->toolTip());
    style->setValue("name_color", deskfolder_col_lab->palette().color(QPalette::Window));
    style->endGroup(); //Deskfolder
    //////////////////////////////////////////////////////////////////
    style->beginGroup("Deskfile");
    style->setValue("name_color", deskfile_col_lab->palette().color(QPalette::Window));
    style->endGroup(); //Deskfile
    //////////////////////////////////////////////////////////////////
    style->beginGroup("Deskdev");
    style->setValue("d_disk_pix", deskdev_disk_pix->toolTip());
    style->setValue("d_cdrom_pix", deskdev_cdrom_pix->toolTip());
    style->setValue("name_color", deskdev_col_lab->palette().color(QPalette::Window));
    style->endGroup(); //Deskdev
    //////////////////////////////////////////////////////////////////
    style->beginGroup("Dateclock");
    style->setValue("date_color", date_col_lab->palette().color(QPalette::Window));
    style->setValue("clock_color", clock_col_lab->palette().color(QPalette::Window));
    style->endGroup(); //Dateclock
    //////////////////////////////////////////////////////////////////
    style->beginGroup("Launcher");
    style->beginGroup("Icon");
    style->setValue("launcher_pix", launcher_pix->toolTip());
    style->setValue("application_pix", application_pix->toolTip());
    style->setValue("quit_pix", quit_pix->toolTip());
    style->setValue("shutdown_pix", shutdown_pix->toolTip());
    style->setValue("restart_pix", restart_pix->toolTip());
    style->setValue("refresh_pix", refresh_pix->toolTip());
    style->setValue("run_pix", run_pix->toolTip());
    style->setValue("show_pix", show_pix->toolTip());
    style->setValue("manager_pix", manager_pix->toolTip());
    style->setValue("utility_pix", utility_pix->toolTip());
    style->setValue("office_pix", office_pix->toolTip());
    style->setValue("internet_pix", internet_pix->toolTip());
    style->setValue("graphic_pix", graphic_pix->toolTip());
    style->setValue("development_pix", development_pix->toolTip());
    style->setValue("system_pix", system_pix->toolTip());
    style->setValue("audio_pix", audio_pix->toolTip());
    style->setValue("video_pix", video_pix->toolTip());
    style->endGroup(); //Icon
    style->endGroup(); //Launcher
    //////////////////////////////////////////////////////////////////
    style->beginGroup("Other");
    style->setValue("folder_link_pix", folder_link_pix->toolTip());
    style->setValue("file_link_pix", file_link_pix->toolTip());
    style->setValue("app_link_pix", app_link_pix->toolTip());
    style->setValue("delete_link_pix", delete_link_pix->toolTip());
    style->setValue("delete_file_pix", delete_file_pix->toolTip());
    style->setValue("close_dock_pix", close_dock_pix->toolTip());
    style->setValue("add_to_sys_pix", add_to_sys_pix->toolTip());
    style->setValue("open_with_pix", open_with_pix->toolTip());
    style->endGroup(); //Other
    //////////////////////////////////////////////////////////////////

    msg = new Msgbox(this);
    msg->setText("<b>Antico style settings updated</b>");
    msg->setInformativeText("To apply the modify, select <b>Refresh WM</b> on Launcher menu");
    msg->setIcon(QMessageBox::Information);
    msg->exec();
}

void Manager::add_app_pressed() // add selected app on lancher menu (in the select Category)
{
    if (tree_view->currentIndex().isValid())
    {
        QString path = dir_model->filePath(tree_view->currentIndex());
        QString app = dir_model->fileName(tree_view->currentIndex());
        QFileInfo info = dir_model->fileInfo(tree_view->currentIndex());
        Appicon *app_ico = new Appicon(this); // get application icon
        QString icon = app_ico->get_app_icon(app);

        if ((app != "") && (info.isDir() == false))
        {
            qDebug() << "app:" << app << "path:" << path;
            antico->beginGroup("Launcher");
            antico->beginGroup("Category");
            antico->beginGroup(category_combo->currentText()); // the select Category type from combobox
            antico->beginGroup(app);
            antico->setValue("name", app);
            antico->setValue("path", path);
            antico->setValue("icon", icon);
            antico->endGroup(); // App name
            antico->endGroup(); // Category type
            antico->endGroup(); // Category group
            antico->endGroup(); // Launcher
            update_remove_list();
            msg = new Msgbox(this);
            msg->setText("<b>" + app.toUpper() + "</b>" + " added to " + "<b>" + category_combo->currentText() + "</b>" + " menu");
            msg->setIcon(QMessageBox::Information);
        }
    }
}

void Manager::add_run_app_pressed() // add selected app on "Run at startup" list
{
    Filedialog *sel_dir = new Filedialog("Add application to run at startup:", "OK_Cancel", this);

    if (sel_dir->exec() == QDialog::Accepted)
    {
        QString path = sel_dir->get_selected_path();
        QString name = sel_dir->get_selected_name();

        if (name != "")
        {
            qDebug() << "App:" << name << "Path:" << path;
            run_list->addItem(name); // add app on run list
            antico->beginGroup("Startup");
            antico->beginGroup(name);
            antico->setValue("name", name);
            antico->setValue("path", path);
            antico->endGroup(); // Name
            antico->endGroup(); // Startup
            msg = new Msgbox(this);
            msg->setText("<b>" + name.toUpper() + "</b>" + " added to Run list");
            msg->setIcon(QMessageBox::Information);
        }
    }
}

void Manager::remove_run_app_pressed() // remove selected app from "Run at startup" list
{
    if (run_list->currentItem() != NULL)
    {
        QString app_name = run_list->currentItem()->text();
        run_list->removeItemWidget(run_list->currentItem());
        antico->beginGroup("Startup");
        antico->remove(app_name); // remove the application
        antico->endGroup(); // Startup
        msg = new Msgbox(this);
        msg->setText("<b>" + app_name.toUpper() + "</b>" + " removed from list");
        msg->setIcon(QMessageBox::Information);
        update_run_list();
    }
}

void Manager::remove_app_pressed() // remove selected app from Launcher menu
{
    if (app_tree->currentItem() != NULL)
    {
        QString app = app_tree->currentItem()->text(0);
        QString cat_group = app_tree->currentItem()->parent()->text(0);
        qDebug() << "Application:" << app;
        antico->beginGroup("Launcher");
        antico->beginGroup("Category");
        antico->beginGroup(cat_group); // Category group (parent of application)
        qDebug() << "Parent Application:" << cat_group;
        antico->remove(app); // remove the application
        antico->endGroup(); // Category group
        antico->endGroup(); // Category
        antico->endGroup(); // Launcher
        update_remove_list();
        msg = new Msgbox(this);
        msg->setText("<b>" + app.toUpper() + "</b>" + " removed from " + "<b>" + cat_group + "</b>" + " menu");
        msg->setIcon(QMessageBox::Information);
    }
}

void Manager::update_remove_list() // update the Category/Apps list on remove tab
{
    app_tree->clear();
    antico->beginGroup("Launcher");
    antico->beginGroup("Category");

    for (int c = 0; c < antico->childGroups().size(); c++)
    {
        QString cat_name = antico->childGroups().value(c); //Category group (Utility, Office ...)
        antico->beginGroup(cat_name); // Category group
        QTreeWidgetItem *category = new QTreeWidgetItem(app_tree); // add Category group on TreeWidget
        category->setText(0, cat_name); // set Category group name (top-level item)

        for (int i = 0; i < antico->childGroups().size(); i++)
        {
            QString app_name = antico->childGroups().value(i); //Application name
            QTreeWidgetItem *app = new QTreeWidgetItem(category);
            app->setText(0, app_name);
        }
        antico->endGroup(); // Category group
    }
    antico->endGroup();// Category
    antico->endGroup();// Launcher
}

void Manager::update_run_list() // update the "Run at startup" list
{
    run_list->clear();
    antico->beginGroup("Startup");

    for (int i = 0; i < antico->childGroups().size(); i++)
    {
        QString app_name = antico->childGroups().value(i);
        antico->beginGroup(app_name); // App group
        run_list->addItem(app_name); // add app on run list
        antico->endGroup(); // Name
    }
    antico->endGroup(); // Startup
}

void Manager::quit_pressed()
{
    close();
}

void Manager::show_path(const QModelIndex &index)
{
    app_path->setText(dir_model->filePath(index));
}

void Manager::mousePressEvent(QMouseEvent *event)
{
    mousepos = event->pos();
    grabMouse(QCursor(Qt::SizeAllCursor));
}

void Manager::mouseMoveEvent(QMouseEvent *event)
{
    QPoint p = event->globalPos()-mousepos;
    move(p.x(), p.y());
}

void Manager::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    releaseMouse();
}


