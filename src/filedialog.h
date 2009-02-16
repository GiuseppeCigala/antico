////////////////////////////////////////
//  File      : filedialog.h          //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

// dialog for directory selection

////////////////////////////////////////

#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include "defs.h"
#include "msgbox.h"
#include "utils.h"
#include "header.h"

////////////////////////////////////////

class Filedialog : public QDialog
{
    Q_OBJECT

public:
    Filedialog(Categorymenu *, QWidget *parent=0);
    Filedialog(QWidget *parent=0);
    ~Filedialog();
    void init();
    void set_path(const QString &path);
    void set_type(const QString &, const QString &);
    QString get_selected_path() const;
    QString get_selected_name() const;
    QString get_selected_icon() const;
    void set_filter(QDir::Filters);
    void set_name_filters(const QStringList &fltr);
    void set_read_only(bool);
    void clear();
    void read_settings();
    void set_category_menu();
   
protected:
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
 
signals:

public slots:
    void show_path(const QModelIndex &);
    void show_preview(const QModelIndex &);
    void del_file();
    void show_hidden(bool);
    void path_completer();
    void update_tree(const QModelIndex &);
    void paintEvent(QPaintEvent *);
    void contextMenuEvent(QContextMenuEvent *);

private:
    QPoint mousepos;
    QDirModel *dir_model;
    QTreeView *tree_view;
    QLineEdit *line_path;
    QMenu *main_menu;
    QMenu *open_menu;
    QString win_text;
    QString delete_file_pix;
    QString open_with_pix;
    QString ok_button_pix_path;
    QString close_button_pix_path;
    QLabel *win_label;
    QLabel *preview_label;
    QRadioButton *hidden_files;
    QVBoxLayout *layout;
    QHBoxLayout *hor_layout;
    QDialogButtonBox *button_box;
    QPushButton *ok;
    QPushButton *cancel;
    QPushButton *close;
    QSettings *antico;
    QSettings *style;
    QCompleter *completer;
    Categorymenu *cat_menu;
    Fileicon *prov;
    QList <QMenu *> menu_list;
};


#endif
