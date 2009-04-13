////////////////////////////////////////
//  File      : slider.cpp            //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "slider.h"

////////////////////////////////////////

Slider::Slider(Extension *ex, QString ty, QWidget * parent) : QFrame(parent)
{
    ext = ex;
    type = ty;
    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    setToolTip(ext->get_id());
    init();
}

Slider::~Slider()
{}

void Slider::init()
{
    QGridLayout *layout = new QGridLayout();
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(5);
    setLayout(layout);
    QLabel *id_lab = new QLabel(this);
    QString name = QApplication::fontMetrics().elidedText(ext->get_id(), Qt::ElideRight, width()-width()/3);
    id_lab->setText(name);
    id_lab->setAlignment(Qt::AlignCenter);
    
    if (type == "mono")
    {
        Cursor *mono_cursor = new Cursor("mono", this);
        mono_cursor->setMinValue(ext->get_min_value());
        mono_cursor->setMaxValue(ext->get_max_value());
        mono_cursor->setActValue(ext->get_mono_value()); 
        layout->addWidget(id_lab, 0, 0, Qt::AlignCenter);
        layout->addWidget(mono_cursor, 1, 0, Qt::AlignCenter);
        connect(mono_cursor, SIGNAL(valueChanged(int)), this, SLOT(set_mono_volume(int)));
    }
    if (type == "stereo")
    {
        Cursor *left_cursor = new Cursor("left", this);
        Cursor *right_cursor = new Cursor("right", this);
        left_cursor->setMinValue(ext->get_min_value());
        left_cursor->setMaxValue(ext->get_max_value());
        left_cursor->setActValue(ext->get_left_value()); 
        right_cursor->setMinValue(ext->get_min_value());
        right_cursor->setMaxValue(ext->get_max_value());
        right_cursor->setActValue(ext->get_right_value());
        layout->addWidget(id_lab, 0, 0, 1, 0, Qt::AlignCenter);
        layout->addWidget(left_cursor, 1, 0, Qt::AlignCenter);
        layout->addWidget(right_cursor, 1, 1, Qt::AlignCenter);
        connect(left_cursor, SIGNAL(valueChanged(int)), this, SLOT(set_left_volume(int)));
        connect(right_cursor, SIGNAL(valueChanged(int)), this, SLOT(set_right_volume(int)));
    }
}

void Slider::set_mono_volume(int volume)
{
    ext->set_mono_value(volume);
}

void Slider::set_left_volume(int volume)
{
    ext->set_left_value(volume);
}

void Slider::set_right_volume(int volume)
{
    ext->set_right_value(volume);
}
