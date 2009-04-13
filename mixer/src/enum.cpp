////////////////////////////////////////
//  File      : enum.cpp              //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "enum.h"

////////////////////////////////////////

Enum::Enum(Extension *ex, QWidget * parent) : QFrame(parent)
{
    ext = ex;
    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    setToolTip(ext->get_id());
    init();
    read_entry();
}

Enum::~Enum()
{}

void Enum::init()
{
    QGridLayout *layout = new QGridLayout();
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(5);
    setLayout(layout);
    QLabel *id_lab = new QLabel(this);
    id_lab->setText(ext->get_id());
    id_lab->setAlignment(Qt::AlignCenter);
    combo = new QComboBox(this);
    layout->addWidget(id_lab, 0, 0, Qt::AlignCenter);
    layout->addWidget(combo, 1, 0, Qt::AlignCenter);
    connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(change_entry(int)));
}

void Enum::read_entry()
{
    QStringList entry_list = ext->get_entry_list();
    
    for (int i = 0; i < entry_list.size(); ++i)
    {
        combo->addItem(entry_list.at(i));
    }
}

void Enum::change_entry(int num) // change ComboBox entry
{
    ext->set_act_value(num);
}