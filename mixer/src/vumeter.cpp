////////////////////////////////////////
//  File      : vumeter.cpp           //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "vumeter.h"

////////////////////////////////////////

VUMeter::VUMeter(QWidget *parent) : QWidget(parent)
{
    colBack = QColor(60, 60, 60);
    colValue = Qt::white;
    colHigh = Qt::red;
    colLow = Qt::green;
    dimVal = 14;
    min = 0;
    max = 100;
    val = 0;
    label = "M";
}

VUMeter::~VUMeter()
{}

void VUMeter::paintEvent(QPaintEvent *)
{
    paintBorder();
    paintValue();
    paintBar();
}

void VUMeter::paintBorder()
{
    QPainter painter(this);
    painter.setWindow(0, 0, 50, 500);
    painter.setRenderHint(QPainter::Antialiasing);
    QColor light = Qt::gray;
    QColor dark = colBack.darker(180);
    painter.setPen(QPen(colBack, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QLinearGradient linGrad(2, 250, 10, 250);
    linGrad.setColorAt(0, light);
    linGrad.setColorAt(1, colBack);
    linGrad.setSpread(QGradient::PadSpread);
    painter.setBrush(linGrad);
    QRectF border(2, 5, 48, 490);
    painter.drawRoundRect(border, 10, 3);
    QLinearGradient linGrad1(40, 250, 48, 250);
    linGrad1.setColorAt(0, colBack);
    linGrad1.setColorAt(1, dark);
    linGrad1.setSpread(QGradient::PadSpread);
    painter.setBrush(linGrad1);
    QRectF border1(25, 5, 23, 490);
    painter.drawRoundRect(border1, 10, 3);
    // paint label
    painter.setPen(QPen(colValue, 2));
    QRectF channel(5, 450, 40, 40);
    QFont valFont("Arial", 20, QFont::Bold);
    painter.setFont(valFont);
    painter.drawText(channel, Qt::AlignCenter, label);
}

void VUMeter::paintBar()
{
    QPainter painter(this);
    painter.setWindow(0, 0, 50, 500);
    painter.setRenderHint(QPainter::Antialiasing);
    QLinearGradient linGrad(25, 0, 25, 500);
    linGrad.setColorAt(0, colHigh);
    linGrad.setColorAt(1, colLow);
    linGrad.setSpread(QGradient::PadSpread);
    painter.setBrush(linGrad);
    // draw color bar
    QRectF col_bar(10, 50, 30, 400);
    painter.drawRect(col_bar);
    // draw background bar
    painter.setBrush(QColor(40, 40, 40));
    int length = 400.0;
    int bar = abs(length*(min-val)/(max-min));
    QRectF back_bar(10, 50, 30, 400-bar);
    painter.drawRect(back_bar);
}

void VUMeter::paintValue()
{
    QPainter painter(this);
    painter.setWindow(0, 0, 50, 500);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::black);
    painter.drawRect(5, 10, 40, 35);
    painter.setPen(Qt::gray);
    painter.drawLine(5, 45, 45, 45);
    painter.drawLine(45, 10, 45, 45);

    painter.setPen(QPen(colValue, 1));
    QFont valFont("Arial", dimVal, QFont::Bold);
    painter.setFont(valFont);

    QRectF valR(5, 10, 40, 40);
    QString value = QString("%1").arg(val, 0);
    painter.drawText(valR, Qt::AlignCenter, value);
}

void VUMeter::setValueDim(int dim)
{
    dimVal = dim;
    update();
}

void VUMeter::setColorBg(QColor color)
{
    colBack = color;
    update();
}

void VUMeter::setColorValue(QColor color)
{
    colValue = color;
    update();
}

void VUMeter::setColorHigh(QColor color)
{
    colHigh = color;
    update();
}

void VUMeter::setColorLow(QColor color)
{
    colLow = color;
    update();
}

void VUMeter::setActValue(int value)
{
    if (value > max)
    {
        val = value;
        update();
    }
    else if (value < min)
    {
        val = min;
        update();
    }
    else
    {
        val = value;
        update();
    }
}

void VUMeter::setMinValue(int minValue)
{
    if (minValue > max)
    {
        min = max;
        max = minValue;
        update();
    }
    else
    {
        min = minValue;
        update();
    }
}

void VUMeter::setMaxValue(int maxValue)
{
    if (maxValue < min)
    {
        max = min;
        min = maxValue;
        update();
    }
    else
    {
        max = maxValue;
        update();
    }
}

void VUMeter::setLabel(QString lab)
{
    label = lab;
    update();
}

QSize VUMeter::sizeHint() const
{
    return QSize(25, 200);
}

QSize VUMeter::minimumSizeHint() const
{
    return QSize(25, 200);
}

