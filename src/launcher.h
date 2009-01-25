////////////////////////////////////////
//  File      : launcher.h            //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

// the "Start" menu in the bottom-left of the dockbar
// manage the menu entry

////////////////////////////////////////

#ifndef LAUNCHER_H
#define LAUNCHER_H

#include "defs.h"
#include "antico.h"
#include "manager.h"
#include "runner.h"
#include "utils.h"

class Antico;

////////////////////////////////////////

class Launcher : public QLabel
{
    Q_OBJECT

public:
    Launcher(Antico *a, QWidget *parent=0);
    ~Launcher();
    void read_settings();
    void init();
    void update_style();

public slots:
    void run_command(QAction *);

protected:
    void mousePressEvent(QMouseEvent *);

private:
 
    QAction *quit;
    QAction *refresh;
    QAction *run;
    QAction *show_desk;
    QAction *manag;
	QSettings *style;
	QMenu *main_menu;
	QString launcher_pix;
 	QString app_pix;
    QString quit_pix;
    QString show_pix;
    QString refresh_pix;
    QString run_pix;
    QString manager_pix;
    Categorymenu *cat_menu;
    Antico *app;
};

#endif
