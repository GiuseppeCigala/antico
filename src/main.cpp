////////////////////////////////////////
//  File      : main.cpp              //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "defs.h"
#include "antico.h"

////////////////////////////////////////


int main(int argc, char **argv)
{
    Antico a(argc, argv);
    XSelectInput(QX11Info::display(), QX11Info::appRootWindow(QX11Info::appScreen()), KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
                 KeymapStateMask | ButtonMotionMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask | FocusChangeMask |
                 ExposureMask | StructureNotifyMask | SubstructureRedirectMask | SubstructureNotifyMask);
    XClearWindow(QX11Info::display(), QX11Info::appRootWindow(QX11Info::appScreen()));
    XSync(QX11Info::display(), FALSE);
    return a.exec();
}

