////////////////////////////////////////
//  File      : runner.cpp            //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "runner.h"

////////////////////////////////////////

Runner::Runner(QWidget *parent) : QDialog(parent)
{
    setSizeGripEnabled(true);
    setContentsMargins(0, 10, 0, 0);
    init();
    show();
}

Runner::~Runner()
{}

void Runner::init()
{
    QGridLayout *layout = new QGridLayout(this);
    setLayout(layout);
    QLabel *win_lab = new QLabel(tr("<b>RUNNER</b>"), this);
    win_lab->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    QLabel *text = new QLabel(tr("Type command line:"), this);
    command = new QLineEdit("", this);
    QPushButton *run = new QPushButton(tr("Run"), this);
    QPushButton *quit = new QPushButton(tr("Quit"), this);
    layout->addWidget(win_lab, 0, 0, 1, 0);
    layout->addWidget(text, 1, 0, 1, 0);
    layout->addWidget(command, 2, 0, 1, 0);
    layout->addWidget(run, 3, 0);
    layout->addWidget(quit, 3, 1);

    connect(run, SIGNAL(clicked()), this, SLOT(run()));
    connect(quit, SIGNAL(clicked()), this, SLOT(quit()));
}

void Runner::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::darkGray, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawRect(0, 0, width(), height());
}

void Runner::mousePressEvent(QMouseEvent *event)
{
    mousepos = event->pos();
    grabMouse(QCursor(Qt::SizeAllCursor));
}

void Runner::mouseMoveEvent(QMouseEvent *event)
{
    QPoint p = event->globalPos()-mousepos;
    move(p.x(), p.y());
}

void Runner::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    releaseMouse();
}

void Runner::run()
{
    QString cmd = command->text();

    if ((cmd != ""))
    {
        if (QProcess::startDetached(cmd))
            close();
        else
        {
            Msgbox *msg = new Msgbox(this);
            msg->setText(tr("<b>COMMAND INCORRECT</b>"));
            msg->setInformativeText(tr("Check the command syntax. If the app is not in your $PATH, type the absolute app path."));
            msg->setIcon(QMessageBox::Critical);
        }
    }
}

void Runner::quit()
{
    close();
}
