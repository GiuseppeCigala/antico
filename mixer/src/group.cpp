////////////////////////////////////////
//  File      : group.cpp             //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "group.h"

////////////////////////////////////////

Group::Group(Extension *ex, QWidget * parent) : QGroupBox(parent)
{
    ext = ex;
    setWindowTitle(ext->get_id());
    init();
}

Group::~Group()
{
}

void Group::init()
{
    layout = new QBoxLayout(QBoxLayout::LeftToRight);
    setLayout(layout);
    layout->setAlignment(Qt::AlignCenter);
    setTitle(ext->get_id());
}

QBoxLayout * Group::get_layout()
{
    return layout;
}

QStringList Group::get_flags()
{
    return ext->get_flags();
}