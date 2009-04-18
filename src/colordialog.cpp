////////////////////////////////////////
//  File      : colordialog.cpp       //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "colordialog.h"

////////////////////////////////////////

Colordialog::Colordialog(const QColor &color, QWidget *parent) : QDialog(parent)
{
    setWindowModality(Qt::WindowModal);
    actual_col = color;
    setSizeGripEnabled(true);
    setMinimumSize(250, 250);
    init();
    show();
}

Colordialog::~Colordialog()
{}

void Colordialog::init()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
    col_sel = new Colorsel(actual_col, this); // update colorsel with actual color
    layout->addWidget(col_sel);

    buttonBox = new QDialogButtonBox(this);
    buttonBox->addButton(tr("Ok"), QDialogButtonBox::AcceptRole);
    buttonBox->addButton(tr("Cancel"), QDialogButtonBox::RejectRole);
    layout->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void Colordialog::paintEvent(QPaintEvent *)
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

QColor Colordialog::get_color()
{
    return col_sel->get_color();
}

void Colordialog::mousePressEvent(QMouseEvent *event)
{
    mousepos = event->pos();
    grabMouse(QCursor(Qt::SizeAllCursor));
    raise();
}

void Colordialog::mouseMoveEvent(QMouseEvent *event)
{
    QPoint p = event->globalPos()-mousepos;
    move(p.x(), p.y());
}

void Colordialog::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    releaseMouse();
}
