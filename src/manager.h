////////////////////////////////////////
//  File      : manager.h             //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

// manage the frame border, color, pixmap, desk wallpaper
// add/remove apps from the launcher menu

////////////////////////////////////////

#ifndef MANAGER_H
#define MANAGER_H

#include "defs.h"
#include "filedialog.h"
#include "colordialog.h"
#include "msgbox.h"
#include "utils.h"

////////////////////////////////////////

class Manager : public QDialog
{
    Q_OBJECT

public:
    Manager(QWidget *parent=0);
    ~Manager();
    void init();
    void read_settings();
    void style_tab();
    void add_app_tab();
    void run_app_tab();
    void remove_app_tab();
    void update_remove_list();
    void update_run_list(); 

public slots:
    void ok_frame_pressed();
    void add_app_pressed();
    void add_run_app_pressed();
    void remove_app_pressed();
    void remove_run_app_pressed(); 
    void quit_pressed();
    void select_pixmap(QWidget *);
    void select_color(QWidget *);
    void show_path(const QModelIndex &);
    void update_add_tree(const QModelIndex &);
    void path_completer();
    void select_style();

protected:
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

private:
    QSpinBox *top_bdr_spinBox;
    QSpinBox *lateral_bdr_spinBox;
    QSpinBox *bottom_bdr_spinBox;
    QSpinBox *dockbar_height_spinBox;
    QLabel *dockbar_pix;
    QLabel *deskfolder_pix;
    QLabel *deskdev_disk_pix;
    QLabel *deskdev_cdrom_pix;
    QLabel *dockicon_pix;
    QLabel *sysicon_pix;
    QLabel *desktop_pix;
    QLabel *header_active_pix;
    QLabel *header_inactive_pix;
    QLabel *minmax_pix;
    QLabel *close_pix;
    /// Launcher ///
    QLabel *launcher_pix;
    QLabel *application_pix;
    QLabel *quit_pix;
    QLabel *refresh_pix;
    QLabel *run_pix;
    QLabel *show_pix;
    QLabel *manager_pix;
    QLabel *utility_pix;
    QLabel *office_pix;
    QLabel *internet_pix;
    QLabel *graphic_pix;
    QLabel *development_pix;
    QLabel *system_pix;
    QLabel *audio_pix;
    QLabel *video_pix;
    /// Other ///
    QLabel *folder_link_pix;
    QLabel *file_link_pix;
    QLabel *app_link_pix;
    QLabel *delete_link_pix;
    QLabel *delete_file_pix;
    QLabel *close_dock_pix;
    QLabel *add_to_sys_pix;
    QLabel *open_with_pix;

    QString dockbar_pix_path;
    QString deskfolder_pix_path;
    QString deskdev_disk_pix_path;
    QString deskdev_cdrom_pix_path;
    QString dockicon_pix_path;
    QString sysicon_pix_path;
    QString desktop_pix_path;
    QString header_active_pix_path;
    QString header_inactive_pix_path;
    QString minmax_pix_path;
    QString close_pix_path;
    /// Launcher ///
    QString launcher_pix_path;
    QString application_pix_path;
    QString quit_pix_path;
    QString refresh_pix_path;
    QString run_pix_path;
    QString show_pix_path;
    QString manager_pix_path;
    QString utility_pix_path;
    QString office_pix_path;
    QString internet_pix_path;
    QString graphic_pix_path;
    QString development_pix_path;
    QString system_pix_path;
    QString audio_pix_path;
    QString video_pix_path;
    /// Other ///
    QString folder_link_pix_path;
    QString file_link_pix_path;
    QString app_link_pix_path;
    QString delete_link_pix_path;
    QString delete_file_pix_path;
    QString close_dock_pix_path;
    QString add_to_sys_pix_path;
    QString open_with_pix_path;
   
    QString stl_name;
    QString stl_path;
    QLabel *header_col_lab;
    QLabel *dockicon_col_lab;
    QLabel *deskfolder_col_lab;
    QLabel *deskfile_col_lab;
    QLabel *deskdev_col_lab;
    QLabel *deskapp_col_lab;
    QLabel *date_col_lab;
    QLabel *clock_col_lab;
    QLineEdit *style_sel_tx;
    QSettings *style;
    QSettings *antico;
    QTabWidget *tab;
    QTreeWidget *app_tree;
    QTreeView *tree_view;
    QListWidget *run_list;
    QString man_ico;
    QDirModel *dir_model;
    QPoint mousepos;
    QLineEdit *app_path;
    QComboBox *category_combo;
    Msgbox *msg;
    
};
#endif
