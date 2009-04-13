////////////////////////////////////////
//  File      : mixer.cpp             //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "mixer.h"
#include "info.h"

////////////////////////////////////////

Mixer::Mixer(QWidget * parent) : QDialog(parent)
{
    setWindowTitle(tr("Mixer"));
    init();
}

Mixer::~Mixer()
{}

void Mixer::init()
{
    main_layout = new QGridLayout();
    setLayout(main_layout);

    // open the mixer device and get the extensions
    dev = new Device(this);
    extension_list = dev->get_extensions();
    // to get system information
    Info *info_dlg = new Info(dev);

    /// check childs for respective parents ///
    foreach(Extension *child, extension_list)
    {
        Extension *ex;

        if ((ex = extension_list.value(child->get_parent())) != NULL)
        {
            qDebug() << "Extension:" << ex->get_ctrl();
            qDebug() << "Child:" << child->get_ctrl();
            ex->set_child(child->get_ctrl());
        }
    }
    foreach(Extension *ex, extension_list)
    {
        qDebug() << "Extension:" << ex->get_ctrl() << "Parent:" << ex->get_parent() << "Childs:" << ex->get_childs() <<
        "Id:" << ex->get_id() << "Extname:" << ex->get_extname() << "Min:" << ex->get_min_value() << "Max:" << ex->get_max_value() <<
        "Act:" << ex->get_act_value() << "Flags:" << ex->get_flags() << "Type:" << ex->get_type();
    }

    /// create the mixer controls ///
    foreach(Extension *ex, extension_list)
    {
        if (ex->get_type() == "MIXT_MONOSLIDER16")
        {
            Slider *sld = new Slider(ex, "mono");
            control_list.insert(ex->get_ctrl(), sld);
            qDebug() << "Add ctrl:" << ex->get_ctrl() << "Childs:" << ex->get_childs();
        }

        if (ex->get_type() == "MIXT_STEREOSLIDER16")
        {
            Slider *sld = new Slider(ex, "stereo");
            control_list.insert(ex->get_ctrl(), sld);
            qDebug() << "Add ctrl:" << ex->get_ctrl() << "Childs:" << ex->get_childs();
        }

        if (ex->get_type() == "MIXT_GROUP")
        {
            Group *grp = new Group(ex);
            control_list.insert(ex->get_ctrl(), grp);
            qDebug() << "Add ctrl:" << ex->get_ctrl() << "Childs:" << ex->get_childs();
        }

        if (ex->get_type() == "MIXT_MONOPEAK")
        {
            Peak *mono_vu = new Peak(ex, "mono");
            control_list.insert(ex->get_ctrl(), mono_vu);
            qDebug() << "Add ctrl:" << ex->get_ctrl() << "Childs:" << ex->get_childs();
        }

        if (ex->get_type() == "MIXT_STEREOPEAK")
        {
            Peak *stereo_vu = new Peak(ex, "stereo");
            control_list.insert(ex->get_ctrl(), stereo_vu);
            qDebug() << "Add ctrl:" << ex->get_ctrl() << "Childs:" << ex->get_childs();
        }

        if (ex->get_type() == "MIXT_MUTE")
        {
            Mute *mute = new Mute(ex);
            control_list.insert(ex->get_ctrl(), mute);
            qDebug() << "Add ctrl:" << ex->get_ctrl() << "Childs:" << ex->get_childs();
        }

        if (ex->get_type() == "MIXT_ONOFF")
        {
            Onoff *onoff = new Onoff(ex);
            control_list.insert(ex->get_ctrl(), onoff);
            qDebug() << "Add ctrl:" << ex->get_ctrl() << "Childs:" << ex->get_childs();
        }

        if (ex->get_type() == "MIXT_ENUM")
        {
            Enum *en = new Enum(ex);
            control_list.insert(ex->get_ctrl(), en);
            qDebug() << "Add ctrl:" << ex->get_ctrl() << "Childs:" << ex->get_childs();
        }
    }

    /// create the main containers ///
    master_group = new QGroupBox(tr("Master"), this);
    master_group_layout = new QHBoxLayout();
    master_group->setLayout(master_group_layout);
    
    sections_group = new QGroupBox(tr("Sections"), this);
    sections_group_layout = new QVBoxLayout();
    sections_group->setLayout(sections_group_layout);

    info_group = new QGroupBox(tr("Info"), this);
    info_group_layout = new QVBoxLayout();
    info_group->setLayout(info_group_layout);
    QPushButton *info_but = new QPushButton(this);
    info_but->setText(tr("Info"));
    connect(info_but, SIGNAL(clicked()), info_dlg, SLOT(show()));
    info_group_layout->addWidget(info_but);

    main_layout->addWidget(master_group, 0, 0, 0, 1);
    main_layout->addWidget(sections_group, 0, 1);
    main_layout->addWidget(info_group, 1, 1);

    /// merge the childs to respective parent ///
    QHashIterator<int, QWidget *> iter(control_list);

    while (iter.hasNext())
    {
        iter.next();

        QList <int> childs = extension_list.value(iter.key())->get_childs();
        int parent = extension_list.value(iter.key())->get_parent();

        if (childs.size() != 0 && parent == 0 && extension_list.value(iter.key())->get_type() == "MIXT_GROUP") // is top level group box
        {
            qDebug() << "Top Level Group:" << iter.key();

            Group *parent = dynamic_cast <Group *>(iter.value());
            QBoxLayout *group_layout = parent->get_layout();
            QPushButton *section_but = new QPushButton(this);
            section_but->setText(extension_list.value(iter.key())->get_id());
            connect(section_but, SIGNAL(clicked()), parent, SLOT(show()));
            sections_group_layout->addWidget(section_but);

            for (int i = 0; i < childs.size(); ++i)
            {
                group_layout->addWidget(control_list.value(childs.at(i)), Qt::AlignCenter);
                qDebug() << "Child Control:" << childs.at(i) << "add to Group:" << iter.key();
            }
        }
        if (childs.size() != 0 && parent != 0 && extension_list.value(iter.key())->get_type() == "MIXT_GROUP") // is a nested group box
        {
            qDebug() << "Child Group:" << iter.key();

            Group *parent = dynamic_cast <Group *>(iter.value());
            QBoxLayout *group_layout = parent->get_layout();

            if (parent->size().width() >= 400)
                group_layout->setDirection(QBoxLayout::TopToBottom);

            if (parent->get_flags().contains("MIXF_FLAT"))
                group_layout->setDirection(QBoxLayout::LeftToRight);

            for (int i = 0; i < childs.size(); ++i)
            {
                group_layout->addWidget(control_list.value(childs.at(i)), Qt::AlignCenter);
                qDebug() << "Child Control:" << childs.at(i) << "add to Group:" << iter.key();
            }
        }
        if (childs.size() == 0 && parent == 0
                && extension_list.value(iter.key())->get_type() != "MIXT_GROUP") // is a top level control (bypass the empty group)
        {
            qDebug() << "Top Level Control:" << iter.key();
            master_group_layout->addWidget(iter.value(), Qt::AlignCenter);
        }
    }
}

