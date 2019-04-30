//
//    File: test/test.cc
// Created: Tue Apr 30 08:37:05 EDT 2019
// Creator: pooser (on Darwin Erics-MacBook-Pro.local 18.5.0 i386)
//
// ------ Last repository commit info -----
// [ Date ]
// [ Author ]
// [ Source ]
// [ Revision ]

#include "JEventProcessor_test.h"
#include "JEventSource_test.h"
#include "JFactoryGenerator_test.h"

extern "C"{
void InitPlugin(JApplication *app){
  
	InitJANAPlugin(app);
	app->Add(new JEventProcessor_test());
	app->Add(new JEventSourceGeneratorT<JEventSource_test>());
	app->Add(new JFactoryGenerator_test());

}
} // "C"
