////////////////////////////////////////
//  File      : push.cpp              //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "push.h"

////////////////////////////////////////

Push::Push(QWidget *parent) : QWidget(parent)
{
    init();
}

Push::~Push()
{}

void Push::init()
{
    pushed = false;
    fontText = QFont("Arial", 28, QFont::Normal);
    textBut = "Push Button";
    colText = Qt::black;
}


void Push::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setWindow(0, 0, 200, 100);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QColor(60, 60, 60), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    paintBorder(painter);
    paintButton(painter);
    paintText(painter);
}

void Push::paintBorder(QPainter &painter)
{
    QLinearGradient linGrad(5, 100, 100, 80);
    linGrad.setColorAt(0, Qt::gray);
    linGrad.setColorAt(1, QColor(200, 200, 200));
    linGrad.setSpread(QGradient::ReflectSpread);
    painter.setBrush(linGrad);
    QRectF border(5, 5, 190, 90);
    painter.drawRoundRect(border, 10, 3);
}

void Push::paintButton(QPainter &painter)
{
    if (pushed == false)
    {
        painter.setBrush(QColor(100, 100, 100));
        QRectF border1(15, 15, 170, 70);
        painter.drawRect(border1);
        painter.setPen(QPen(QColor(10, 10, 10), 3));
        painter.drawLine(186, 84, 186, 16);
        painter.drawLine(16, 84, 186, 84);
        return;
    }
    if (pushed == true)
    {
        painter.setBrush(QColor(110, 110, 110));
        QRectF border1(15, 15, 170, 70);
        painter.drawRect(border1);
        painter.setPen(QPen(QColor(10, 10, 10), 3));
        painter.drawLine(16, 16, 16, 84);
        painter.drawLine(16, 16, 184, 16);
        return;
    }
}

void Push::paintText(QPainter &painter)
{
    if (pushed == false)
    {
        QRectF text(5, 5, 190, 90);
        painter.setFont(fontText);
        painter.setPen(QPen(colText));
        painter.drawText(text, Qt::AlignCenter, textBut);
    }
    if (pushed == true)
    {
        QRectF text(7, 7, 190, 90);
        painter.setFont(fontText);
        painter.setPen(QPen(colText));
        painter.drawText(text, Qt::AlignCenter, textBut);
    }
}

void Push::setText(QString text)
{
    textBut = text;
    update();
}

void Push::setFont(QFont font)
{
    fontText = font;
    update();
}

void Push::setColorText(QColor colorTx)
{
    colText = colorTx;
    update();
}

void Push::mousePressEvent(QMouseEvent *e)
{
    double x = e->x();
    double y = e->y();

    double wrun = width();
    double hrun = height();

    double wstart = 200;
    double hstart = 100;

    double dx = wrun/wstart;
    double dy = hrun/hstart;

    if (y>=(5*dy) && y<=(95*dy) && x>=(5*dx) && x<=(195*dx))
    {
        pushed = true;
        update();
        emit pressed();
    }
}

void Push::mouseReleaseEvent(QMouseEvent *e)
{
    double x = e->x();
    double y = e->y();

    double wrun = width();
    double hrun = height();

    double wstart = 200;
    double hstart = 100;

    double dx = wrun/wstart;
    double dy = hrun/hstart;

    if (y>=(5*dy) && y<=(95*dy) && x>=(5*dx) && x<=(195*dx))
    {
        pushed = false;
        update();
        emit released();
    }
}

QSize Push::sizeHint() const
{
    return QSize(60, 30);
}

QSize Push::minimumSizeHint() const
{
    return QSize(60, 30);
}
