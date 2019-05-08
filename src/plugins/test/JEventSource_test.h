//
//    File: test/JEventSource_test.h
// Created: Tue Apr 30 08:37:05 EDT 2019
// Creator: pooser (on Darwin Erics-MacBook-Pro.local 18.5.0 i386)
//
// ------ Last repository commit info -----
// [ Date ]
// [ Author ]
// [ Source ]
// [ Revision ]


#ifndef _JEventSource_test_h_
#define _JEventSource_test_h_

#include <JANA/JEvent.h>
#include <JANA/JEventSource.h>
#include <JANA/JEventSourceGeneratorT.h>

#include "JEvent_test.h"

#include "MyHit.h"

// This first bit is optional. It allows you to specify how likely this class
// is to read from a given source. Uncomment this if you want to return something
// other than the default of 0.01 which is OK for most implementations.
// template<> double JEventSourceGeneratorT<JEventSource_test>::CheckOpenable(std::string source) {
// 	if( source.find(".evio") != source.npos ) return 1.0;
// }

//////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief class description.
///
/// Detailed class description.
//////////////////////////////////////////////////////////////////////////////////////////////////
class JEventSource_test : public JEventSource{
	public:
	        JEventSource_test(std::string source_name, JApplication *app);
	        virtual ~JEventSource_test();
		
	        static std::string GetDescription(void){ return "My Event source"; }
                void Open(void);
		void GetEvent(std::shared_ptr<JEvent>);
	        bool GetObjects(const std::shared_ptr<const JEvent>& aEvent, JFactory* aFactory);
		// std::shared_ptr<JTaskBase> GetProcessEventTask(std::shared_ptr<const JEvent>&& aEvent);

};

#endif // _JEventSource_test_h_

