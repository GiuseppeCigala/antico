////////////////////////////////////////
//  File      : cursor.h              //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#ifndef CURSOR_H
#define CURSOR_H

#include "defs.h"

///////////////////////////////////////

class Cursor : public QWidget
{
    Q_OBJECT
    
    QColor colorBg() const
    {
        return colBack;
    }
    QColor colorValue() const
    {
        return colValue;
    }
    QColor colorScale() const
    {
        return colScale;
    }
    int valueDim() const
    {
        return dimVal;
    }
    int scaleDim() const
    {
        return dimScale;
    }
    int minValue() const
    {
        return min;
    }
    int maxValue() const
    {
        return max;
    }

public:
    Cursor(QString, QWidget *parent = 0);
    ~Cursor();
    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    void set_type(QString);

signals:
    void valueChanged(int);

public slots:
    void setColorBg(QColor);
    void setColorScale(QColor);
    void setColorValue(QColor);
    void setValueDim(int);
    void setScaleDim(int);
    void setMinValue(int);
    void setMaxValue(int);
    void setActValue(int);
    
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void paintBorder();
    void paintCursor();
    void paintScale();
    void paintValue();

private:
    double cy;
    int min;
    int max;
    int act;    
    int dimVal;
    int dimScale;
    QColor colBack;
    QColor colScale;
    QColor colValue;
    QString type;
};

#endif
