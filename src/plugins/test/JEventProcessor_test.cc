//
//    File: test/JEventProcessor_test.cc
// Created: Tue Apr 30 08:37:05 EDT 2019
// Creator: pooser (on Darwin Erics-MacBook-Pro.local 18.5.0 i386)
//
// ------ Last repository commit info -----
// [ Date ]
// [ Author ]
// [ Source ]
// [ Revision ]

#include "JEventProcessor_test.h"
#include "MyHit.h"

//---------------------------------
// JEventProcessor_test    (Constructor)
//---------------------------------
JEventProcessor_test::JEventProcessor_test()
{

}

//---------------------------------
// ~JEventProcessor_test    (Destructor)
//---------------------------------
JEventProcessor_test::~JEventProcessor_test()
{

}

//------------------
// Init
//------------------
void JEventProcessor_test::Init(void)
{

  // This is called once at program startup.
  testFile = new TFile("test.root", "RECREATE");
  xHisto   = new TH1I("xHisto", "x test; x test; NOE", 100, 0, 100);
  EHisto   = new TH1I("EHisto", "E test; E test; NOE", 100, 0, 100);
  tHisto   = new TH1I("tHisto", "t test; t test; NOE", 100, 0, 100);
  
}

//------------------
// Process
//------------------
void JEventProcessor_test::Process(const std::shared_ptr<const JEvent>& aEvent)
{
	// This is called for every event. Use of common resources like writing
	// to a file or filling a histogram should be mutex protected. Using
	// aEvent->Get<type>() to get reconstructed objects (and thereby activating the
	// reconstruction algorithm) should be done outside of any mutex lock
	// since multiple threads may call this method at the same time.
	// Here's an example:
	//
	// auto myTracks = aEvent->Get*<MyTrack>();
	//
	// lock_guard<mutex> lck( mymutex );
	// for( auto t : myTracks ){
	//  ... fill histograms or trees ...
	// }

        lock_guard<mutex> lck( mymutex );

	auto myHit = aEvent->Get<MyHit>();

	for(auto h : myHit) {
	  double x = h->x; double E = h->E; double t = h->t;
	  xHisto->Fill(x); EHisto->Fill(E); tHisto->Fill(t);
	}

}

//------------------
// Finish
//------------------
void JEventProcessor_test::Finish(void)
{
	// This is called when at the end of event processing
	testFile->Write(); testFile->Close();
}
