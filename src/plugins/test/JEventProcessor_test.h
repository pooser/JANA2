//
//    File: test/JEventProcessor_test.h
// Created: Tue Apr 30 08:37:05 EDT 2019
// Creator: pooser (on Darwin Erics-MacBook-Pro.local 18.5.0 i386)
//
// ------ Last repository commit info -----
// [ Date ]
// [ Author ]
// [ Source ]
// [ Revision ]


#ifndef _JEventProcessor_test_h_
#define _JEventProcessor_test_h_

#include <JANA/JEventProcessor.h>

#include "MyHit.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief class description.
///
/// Detailed class description.
//////////////////////////////////////////////////////////////////////////////////////////////////
class JEventProcessor_test : public JEventProcessor{
	public:
		JEventProcessor_test();
		virtual ~JEventProcessor_test();

		virtual void Init(void);
                virtual void Process(const std::shared_ptr<const JEvent>& aEvent);// {
                // auto hits = aEvent->Get<MyHit>();
		// }
		virtual void Finish(void);

	protected:


	private:

};

#endif // _JEventProcessor_test_h_

