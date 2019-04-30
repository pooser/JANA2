//
//    File: test/JEvent_test.h
// Created: Tue Apr 30 08:37:05 EDT 2019
// Creator: pooser (on Darwin Erics-MacBook-Pro.local 18.5.0 i386)
//
// ------ Last repository commit info -----
// [ Date ]
// [ Author ]
// [ Source ]
// [ Revision ]


#ifndef _JEvent_test_h_
#define _JEvent_test_h_

#include <JANA/JEvent.h>

//////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief class description.
///
/// Detailed class description.
//////////////////////////////////////////////////////////////////////////////////////////////////
class JEvent_test : public JEvent{
	public:
		        JEvent_test(){}
		virtual ~JEvent_test(){}

		// This represents an event read from a JEventSource_test object
		// Add members that can hold the data in whatever form is easy to access
		// in the JEventSource_test::GetObjects method
};

#endif // _JEvent_test_h_

