////////////////////////////////////////
//  File      : mute.cpp              //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "mute.h"

////////////////////////////////////////

Mute::Mute(Extension *ex, QWidget * parent) : QFrame(parent)
{
    ext = ex;
    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    setToolTip(ext->get_id());
    init();
    state = 0;
}

Mute::~Mute()
{}

void Mute::init()
{
    QGridLayout *layout = new QGridLayout();
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(5);
    setLayout(layout);
    QLabel *id_lab = new QLabel(this);
    id_lab->setText(ext->get_id());
    id_lab->setAlignment(Qt::AlignCenter);
    mute = new Push(this);
    mute->setText("OFF");
    mute->setColorText(Qt::green);
    connect(mute, SIGNAL(pressed()), this, SLOT(set_state()));
    layout->addWidget(id_lab, 0, 0, Qt::AlignCenter);
    layout->addWidget(mute, 1, 0, Qt::AlignCenter);
}

void Mute::set_state()
{
    if(state == 1) // 0 = off, 1 = on
    {
        mute->setText("OFF");
        mute->setColorText(Qt::green);
        state = 0;
        ext->set_act_value(0);
        return;
    }
    if(state == 0) 
    {
        mute->setText("ON");
        mute->setColorText(Qt::red);
        state = 1;
        ext->set_act_value(1);
        return;
    }
}
