////////////////////////////////////////
//  File      : onoff.cpp             //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "onoff.h"

////////////////////////////////////////

Onoff::Onoff(Extension *ex, QWidget * parent) : QFrame(parent)
{
    ext = ex;
    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    setToolTip(ext->get_id());
    init();
    state = 0;
}

Onoff::~Onoff()
{}

void Onoff::init()
{
    QGridLayout *layout = new QGridLayout();
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(5);
    setLayout(layout);
    QLabel *id_lab = new QLabel(this);
    id_lab->setText(ext->get_id());
    id_lab->setAlignment(Qt::AlignCenter);
    onoff = new Push(this);
    onoff->setText("OFF");
    onoff->setColorText(Qt::red);
    connect(onoff, SIGNAL(pressed()), this, SLOT(set_state()));
    layout->addWidget(id_lab, 0, 0, Qt::AlignCenter);
    layout->addWidget(onoff, 1, 0, Qt::AlignCenter);
}

void Onoff::set_state()
{
    if(state == 1) // 0 = off, 1 = on
    {
        onoff->setText("OFF");
        onoff->setColorText(Qt::red);
        state = 0;
        ext->set_act_value(0);
        return;
    }
    if(state == 0) 
    {
        onoff->setText("ON");
        onoff->setColorText(Qt::green);
        state = 1;
        ext->set_act_value(1);
        return;
    }
}
