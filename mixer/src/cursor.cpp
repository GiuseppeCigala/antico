////////////////////////////////////////
//  File      : cursor.cpp            //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "cursor.h"

////////////////////////////////////////

Cursor::Cursor(QString ty, QWidget *parent) : QWidget(parent)
{
    colBack = QColor(60, 60, 60);
    colScale = Qt::yellow;
    colValue = Qt::green;
    dimVal = 18;
    dimScale = 9;
    cy = 473;
    min = 0;
    max = 100;
    act = 0;
    type = ty;
}

Cursor::~Cursor()
{}

void Cursor::paintEvent(QPaintEvent *)
{
    paintBorder();
    paintCursor();
    paintScale();
    paintValue();
}

void Cursor::paintBorder()
{
    QPainter painter(this);
    painter.setWindow(0, 0, 100, 550);
    painter.setRenderHint(QPainter::Antialiasing);
    QColor light = Qt::white;
    QColor dark = colBack.darker(140);
    painter.setPen(QPen(colBack, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QLinearGradient linGrad(5, 250, 10, 250);
    linGrad.setColorAt(0, light);
    linGrad.setColorAt(1, colBack);
    linGrad.setSpread(QGradient::PadSpread);
    painter.setBrush(linGrad);
    QRectF border(5, 5, 90, 540);
    painter.drawRoundRect(border, 30, 5);
    QLinearGradient linGrad1(85, 250, 95, 250);
    linGrad1.setColorAt(0, colBack);
    linGrad1.setColorAt(1, dark);
    linGrad1.setSpread(QGradient::PadSpread);
    painter.setBrush(linGrad1);
    QRectF border1(50, 5, 45, 540);
    painter.drawRoundRect(border1, 50, 5);
}

void Cursor::paintCursor()
{
    QPainter painter(this);
    painter.setWindow(0, 0, 100, 550);
    painter.setRenderHint(QPainter::Antialiasing);

    if (type == "right")
    {
        //draw slot
        painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.setBrush(Qt::black);
        QRectF slot(27, 20, 7, 460);
        painter.drawRect(slot);
        //draw metal support
        QLinearGradient linGrad(33, 250, 36, 250);
        linGrad.setColorAt(0, Qt::black);
        linGrad.setColorAt(1, Qt::white);
        linGrad.setSpread(QGradient::ReflectSpread);
        painter.setBrush(linGrad);
        painter.setPen(QPen(linGrad, 6));
        painter.drawLine(30, 470, 30, 30);
        // draw cursor rect and bar
        QLinearGradient linGrad1(10, 470, 20, 470);
        linGrad1.setColorAt(0, Qt::gray);
        linGrad1.setColorAt(1, Qt::black);
        linGrad1.setSpread(QGradient::PadSpread);
        painter.setBrush(linGrad1);
        painter.setPen(QPen(linGrad1, 2));
        QRectF cursor(14, cy-16, 32, 31);
        painter.drawRoundRect(cursor);
        painter.setPen(QPen(colScale, 4));
        painter.drawLine(16, cy, 44, cy);
    }

    if (type == "left")
    {
        //draw slot
        painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.setBrush(Qt::black);
        QRectF slot(67, 20, 7, 460);
        painter.drawRect(slot);
        //draw metal support
        QLinearGradient linGrad(67, 250, 70, 250);
        linGrad.setColorAt(0, Qt::black);
        linGrad.setColorAt(1, Qt::white);
        linGrad.setSpread(QGradient::ReflectSpread);
        painter.setBrush(linGrad);
        painter.setPen(QPen(linGrad, 6));
        painter.drawLine(70, 470, 70, 30);
        // draw cursor rect and bar
        QLinearGradient linGrad1(50, 470, 60, 470);
        linGrad1.setColorAt(0, Qt::gray);
        linGrad1.setColorAt(1, Qt::black);
        linGrad1.setSpread(QGradient::PadSpread);
        painter.setBrush(linGrad1);
        painter.setPen(QPen(linGrad1, 2));
        QRectF cursor(54, cy-16, 32, 31);
        painter.drawRoundRect(cursor);
        painter.setPen(QPen(colScale, 4));
        painter.drawLine(56, cy, 84, cy);
    }
    if (type == "mono")
    {
        //draw slot
        painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.setBrush(Qt::black);
        QRectF slot(47, 20, 7, 460);
        painter.drawRect(slot);
        //draw metal support
        QLinearGradient linGrad(47, 250, 51, 250);
        linGrad.setColorAt(0, Qt::black);
        linGrad.setColorAt(1, Qt::white);
        linGrad.setSpread(QGradient::ReflectSpread);
        painter.setBrush(linGrad);
        painter.setPen(QPen(linGrad, 6));
        painter.drawLine(50, 470, 50, 30);
        // draw cursor rect and bar
        QLinearGradient linGrad1(30, 470, 40, 470);
        linGrad1.setColorAt(0, Qt::gray);
        linGrad1.setColorAt(1, Qt::black);
        linGrad1.setSpread(QGradient::PadSpread);
        painter.setBrush(linGrad1);
        painter.setPen(QPen(linGrad1, 2));
        QRectF cursor(34, cy-16, 32, 31);
        painter.drawRoundRect(cursor);
        painter.setPen(QPen(colScale, 4));
        painter.drawLine(36, cy, 64, cy);
    }
}

void Cursor::paintScale()
{
    QPainter painter(this);
    painter.setWindow(0, 0, 100, 550);
    painter.setRenderHint(QPainter::Antialiasing);
    QFont valFont("Arial", dimScale, QFont::Normal);
    painter.setFont(valFont);
    painter.setPen(QPen(colScale, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    double numLineL = 10;
    double numLineS = numLineL*5;
    double size = 445;
    double deltaVal = abs(max-min)/10.0;
    double deltaY = 44.5;

    if (type == "left")
    {
        painter.drawLine(50, 473, 50, 28);

        for (int i = 0; i<=numLineL; i++)
        {
            painter.drawLine(45, 473-(i*size/numLineL), 50, 473-(i*size/numLineL));
        }
        for (int i = 0; i<=numLineS; i++)
        {
            painter.drawLine(47, 473-(i*size/numLineS), 50, 473-(i*size/numLineS));
        }
        for (int i = 0; i<=numLineL; i++)
        {
            QString val = QString("%1").arg(min+(deltaVal*i), 0, 'f', 0);
            QRectF valL(15, 465-deltaY*i, 25, 20);
            painter.drawText(valL, Qt::AlignRight, val);
        }
    }
    if (type == "right")
    {
        painter.drawLine(50, 473, 50, 28);

        for (int i = 0; i<=numLineL; i++)
        {
            painter.drawLine(50, 473-(i*size/numLineL), 55, 473-(i*size/numLineL));
        }
        for (int i = 0; i<=numLineS; i++)
        {
            painter.drawLine(50, 473-(i*size/numLineS), 53, 473-(i*size/numLineS));
        }
        for (int i = 0; i<=numLineL; i++)
        {
            QString val = QString("%1").arg(min+(deltaVal*i), 0, 'f', 0);
            QRectF valR(60, 465-deltaY*i, 25, 20);
            painter.drawText(valR, Qt::AlignLeft, val);
        }
    }
    if (type == "mono")
    {
        QFont valFont("Arial", dimScale-2, QFont::Normal);
        painter.setFont(valFont);

        painter.drawLine(30, 473, 30, 28);
        painter.drawLine(70, 473, 70, 28);

        for (int i = 0; i<=numLineL; i++)
        {
            painter.drawLine(25, 473-(i*size/numLineL), 30, 473-(i*size/numLineL));
            painter.drawLine(70, 473-(i*size/numLineL), 75, 473-(i*size/numLineL));
        }
        for (int i = 0; i<=numLineS; i++)
        {
            painter.drawLine(27, 473-(i*size/numLineS), 30, 473-(i*size/numLineS));
            painter.drawLine(70, 473-(i*size/numLineS), 73, 473-(i*size/numLineS));
        }
        for (int i = 0; i<=numLineL; i++)
        {
            QString val = QString("%1").arg(min+(deltaVal*i), 0, 'f', 0);
            QRectF valL(75, 465-deltaY*i, 25, 20);
            QRectF valR(0, 465-deltaY*i, 25, 20);
            painter.drawText(valR, Qt::AlignRight, val);
            painter.drawText(valL, Qt::AlignLeft, val);
        }
    }
}

void Cursor::paintValue()
{
    QPainter painter(this);
    painter.setWindow(0, 0, 100, 550);
    painter.setRenderHint(QPainter::Antialiasing);
    QLinearGradient linGrad(20, 500, 25, 500);
    linGrad.setColorAt(0, Qt::white);
    linGrad.setColorAt(1, Qt::black);
    linGrad.setSpread(QGradient::PadSpread);
    painter.setBrush(linGrad);
    painter.drawRect(20, 495, 60, 35);

    // set actual value
    double act = min+(max-min)*((473-cy)/445);
    QString val = QString("%1").arg(act, 0, 'f', 0);
    QRectF valR(20, 495, 60, 35);
    painter.setPen(QPen(colValue, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QFont valFont("Arial", dimVal, QFont::Bold);
    painter.setFont(valFont);
    painter.drawText(valR, Qt::AlignCenter, val);

    emit valueChanged(val.toInt());
}

void Cursor::mousePressEvent(QMouseEvent *e)
{
    double x = e->x();
    double y = e->y();

    double wrun = width();
    double hrun = height();

    double wstart = 100;
    double hstart = 550;

    double dx = wrun/wstart;
    double dy = hrun/hstart;

    if (type == "left")
    {
        if (x>=(50*dx) && x<=(90*dx) && y >=(26*dy) && y <=(474*dy))
        {
            cy = (y/dy);
            update();
        }
    }
    if (type == "right")
    {
        if (x>=(10*dx) && x<=(50*dx) && y >=(26*dy) && y <=(474*dy))
        {
            cy = (y/dy);
            update();
        }
    }
    if (type == "mono")
    {
        if (x>=(30*dx) && x<=(70*dx) && y >=(26*dy) && y <=(474*dy))
        {
            cy = (y/dy);
            update();
        }
    }
}

void Cursor::mouseMoveEvent(QMouseEvent *e)
{
    double x = e->x();
    double y = e->y();

    double wrun = width();
    double hrun = height();

    double wstart = 100;
    double hstart = 550;

    double dx = wrun/wstart;
    double dy = hrun/hstart;

    if (type == "left")
    {
        if (x>=(50*dx) && x<=(90*dx) && y >=(26*dy) && y <=(474*dy))
        {
            cy = (y/dy);
            update();
        }
    }
    if (type == "right")
    {
        if (x>=(10*dx) && x<=(50*dx) && y >=(26*dy) && y <=(474*dy))
        {
            cy = (y/dy);
            update();
        }
    }
    if (type == "mono")
    {
        if (x>=(30*dx) && x<=(70*dx) && y >=(26*dy) && y <=(474*dy))
        {
            cy = (y/dy);
            update();
        }
    }
}

void Cursor::setScaleDim(int scaleDim)
{
    dimScale = scaleDim;
    update();
}

void Cursor::setValueDim(int valueDim)
{
    dimVal = valueDim;
    update();
}

void Cursor::setColorBg(QColor colorBg)
{
    colBack = colorBg;
    update();
}

void Cursor::setColorValue(QColor colorValue)
{
    colValue = colorValue;
    update();
}

void Cursor::setColorScale(QColor colorScale)
{
    colScale = colorScale;
    update();
}

void Cursor::setMinValue(int minValue)
{
    if (minValue > max)
    {
        min = max;
        max = minValue;
    }
    else
    {
        min = minValue;
    }
    update();
}

void Cursor::setMaxValue(int maxValue)
{
    if (maxValue < min)
    {
        max = min;
        min = maxValue;
    }
    else
    {
        max = maxValue;
    }
    update();
}

void Cursor::setActValue(int val)
{
    if (val > max)
        val = max;

    double wrun = width();
    double hrun = height();

    double wstart = 100;
    double hstart = 550;

    double dx = wrun/wstart;
    double dy = hrun/hstart;

    int y = (473 - 445 * val/(min+(max-min)))*dy;
    int x = 50*dx;

    QMouseEvent *e = new QMouseEvent(QEvent::MouseButtonPress, QPoint(x, y), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    mousePressEvent(e);
}

QSize Cursor::sizeHint() const
{
    return QSize(50, 200);
}

QSize Cursor::minimumSizeHint() const
{
    return QSize(50, 200);
}



