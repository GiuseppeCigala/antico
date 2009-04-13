////////////////////////////////////////
//  File      : main.cpp              //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "mixer.h"

////////////////////////////////////////

int main(int argc, char ** argv)
{
	QApplication app( argc, argv );
	Mixer mix;
	mix.show(); 
	app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
	return app.exec();
}
