////////////////////////////////////////
//  File      : extension.cpp         //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "extension.h"

////////////////////////////////////////

Extension::Extension(int mixer_dev, int mixer_fd, oss_mixext ex, QWidget * parent) : QWidget(parent)
{
    mix_fd = mixer_fd;
    dev = mixer_dev;
    ext = ex;
    check_type();
}

Extension::~Extension()
{}

void Extension::check_type()
{
    switch (ext.type)
    {
    case MIXT_DEVROOT:
        set_type("MIXT_DEVROOT");
        break;

    case MIXT_SLIDER:
        set_shift(0);
        set_value_mask(~0);
        set_type("MIXT_SLIDER");
        break;

    case MIXT_STEREOSLIDER:
        set_shift(8);
        set_value_mask(0xff);
        set_type("MIXT_STEREOSLIDER");
        break;

    case MIXT_STEREOSLIDER16:
        set_shift(16);
        set_value_mask(0xffff);
        set_type("MIXT_STEREOSLIDER16");
        break;

    case MIXT_MONOSLIDER:
        set_shift(0);
        set_value_mask(0xff);
        set_type("MIXT_MONOSLIDER");
        break;

    case MIXT_MONOSLIDER16:
        set_shift(0);
        set_value_mask(0xffff);
        set_type("MIXT_MONOSLIDER16");
        break;

    case MIXT_MONOPEAK:
        set_shift(0);
        set_value_mask(0xff);
        set_type("MIXT_MONOPEAK");
        break;

    case MIXT_STEREOPEAK:
        set_shift(8);
        set_value_mask(0xff);
        set_type("MIXT_STEREOPEAK");
        break;

    case MIXT_GROUP:
        set_type("MIXT_GROUP");
        break;

    case MIXT_RADIOGROUP:
        set_type("MIXT_RADIOGROUP");
        break;

    case MIXT_ONOFF:
        set_type("MIXT_ONOFF");
        break;

    case MIXT_MUTE:
        set_type("MIXT_MUTE");
        break;

    case MIXT_ENUM:
        set_type("MIXT_ENUM");
        break;

    case MIXT_HEXVALUE:
        set_type("MIXT_HEXVALUE");
        break;

    case MIXT_MESSAGE:
        set_type("MIXT_MESSAGE");
        break;

    default:
        break;
    }
}

int Extension::get_act_value()
{
    oss_mixer_value val;
    val.dev = dev;
    val.timestamp = ext.timestamp;
    val.ctrl = ext.ctrl;
    val.value = 0;
    int act_val = 0;

    if (ioctl (mix_fd, SNDCTL_MIX_READ, &val) != -1)
    {
        act_val = val.value;
    }
    return act_val;
}

int Extension::get_mono_value()
{
    oss_mixer_value val;
    val.dev = dev;
    val.timestamp = ext.timestamp;
    val.ctrl = ext.ctrl;
    val.value = 0;
    int act_val = 0;

    if (ioctl (mix_fd, SNDCTL_MIX_READ, &val) != -1)
    {
        act_val = val.value & value_mask;
    }
    return act_val;
}

int Extension::get_left_value()
{
    oss_mixer_value val;
    val.dev = dev;
    val.timestamp = ext.timestamp;
    val.ctrl = ext.ctrl;
    val.value = 0;
    int act_val = 0;

    if (ioctl (mix_fd, SNDCTL_MIX_READ, &val) != -1)
    {
        act_val = val.value & value_mask;
    }
    return act_val;
}

int Extension::get_right_value()
{
    oss_mixer_value val;
    val.dev = dev;
    val.timestamp = ext.timestamp;
    val.ctrl = ext.ctrl;
    val.value = 0;
    int act_val = 0;

    if (ioctl (mix_fd, SNDCTL_MIX_READ, &val) != -1)
    {
        act_val = (val.value >> shift) & value_mask;
    }
    return act_val;
}

int Extension::get_min_value()
{
    return ext.minvalue;
}

int Extension::get_max_value()
{
    return ext.maxvalue;
}

QStringList Extension::get_entry_list() // for MIXT_ENUM
{
    QStringList entry_list;

    oss_mixer_enuminfo ei;
    ei.dev = dev;
    ei.ctrl = ext.ctrl;

    if (ioctl (mix_fd, SNDCTL_MIX_ENUMINFO, &ei) != -1)
    {
        int n = ei.nvalues;
        if (n > ext.maxvalue)
            n = ext.maxvalue;

        for (int i = 0; i < ext.maxvalue; i++)
        {
            if (ext.enum_present[i / 8] & (1 << (i % 8)))
            {
                QString entry = ei.strings + ei.strindex[i];
                entry_list << entry;
            }
        }
    }
    return entry_list;
}

int Extension::get_ctrl()
{
    return ext.ctrl;
}

QString Extension::get_extname()
{
    return ext.extname;
}

QString Extension::get_id()
{
    return ext.id;
}

int Extension::get_parent()
{
    return ext.parent;
}

QStringList Extension::get_flags()
{
    QStringList flags;

    if (ext.flags & MIXF_CENTIBEL)
        flags << "MIXF_CENTIBEL";
    if (ext.flags & MIXF_DECIBEL)
        flags << "MIXF_DECIBEL";
    if (ext.flags & MIXF_DESCR)
        flags << "MIXF_DESCR";
    if (ext.flags & MIXF_DYNAMIC)
        flags << "MIXF_DYNAMIC";
    if (ext.flags & MIXF_FLAT)
        flags << "MIXF_FLAT";
    if (ext.flags & MIXF_HZ)
        flags << "MIXF_HZ";
    if (ext.flags & MIXF_LEGACY)
        flags << "MIXF_LEGACY";
    if (ext.flags & MIXF_MAINVOL)
        flags << "MIXF_MAINVOL";
    if (ext.flags & MIXF_MONVOL)
        flags << "MIXF_MONVOL";
    if (ext.flags & MIXF_OKFAIL)
        flags << "MIXF_OKFAIL";
    if (ext.flags & MIXF_PCMVOL)
        flags << "MIXF_PCMVOL";
    if (ext.flags & MIXF_POLL)
        flags << "MIXF_POLL";
    if (ext.flags & MIXF_READABLE)
        flags << "MIXF_READABLE";
    if (ext.flags & MIXF_RECVOL)
        flags << "MIXF_RECVOL";
    if (ext.flags & MIXF_STRING)
        flags << "MIXF_STRING";
    if (ext.flags & MIXF_WIDE)
        flags << "MIXF_WIDE";
    if (ext.flags & MIXF_WRITEABLE)
        flags << "MIXF_WRITEABLE";

    return flags;
}

QString Extension::get_type()
{
    return type;
}

QList <int> Extension::get_childs()
{
    return childs;
}

void Extension::set_type(QString t)
{
    type = t;
}

void Extension::set_shift(int shf)
{
    shift = shf;
}

void Extension::set_value_mask(int mask)
{
    value_mask = mask;
}

void Extension::set_act_value(int act)
{
    oss_mixer_value val;
    val.dev = dev;
    val.ctrl = ext.ctrl;
    val.timestamp = ext.timestamp;
    val.value = act;
    ioctl(mix_fd, SNDCTL_MIX_WRITE, &val);
}

void Extension::set_mono_value(int mono)
{
    oss_mixer_value val;
    val.dev = dev;
    val.ctrl = ext.ctrl;
    val.timestamp = ext.timestamp;
    val.value = mono;
    ioctl(mix_fd, SNDCTL_MIX_WRITE, &val);
}

void Extension::set_left_value(int left)
{
    oss_mixer_value val;
    val.dev = dev;
    val.ctrl = ext.ctrl;
    val.timestamp = ext.timestamp;
    int right = (get_act_value() >> shift) & value_mask;
    val.value = left | (right << shift);
    ioctl(mix_fd, SNDCTL_MIX_WRITE, &val);
}

void Extension::set_right_value(int right)
{
    oss_mixer_value val;
    val.dev = dev;
    val.ctrl = ext.ctrl;
    val.timestamp = ext.timestamp;
    int left = get_act_value() & value_mask;
    val.value = left | (right << shift);
    ioctl(mix_fd, SNDCTL_MIX_WRITE, &val);
}

void Extension::set_child(int ch)
{
    childs << ch;
}

