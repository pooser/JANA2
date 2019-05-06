//
//    File: toyDet/JEventProcessor_toyDet.cc
// Created: Wed Apr 24 16:04:14 EDT 2019
// Creator: pooser (on Linux rudy.jlab.org 3.10.0-957.10.1.el7.x86_64 x86_64)
//
// ------ Last repository commit info -----
// [ Date ]
// [ Author ]
// [ Source ]
// [ Revision ]

#include "JEventProcessor_toyDet.h"

//---------------------------------
// JEventProcessor_toyDet    (Constructor)
//---------------------------------
JEventProcessor_toyDet::JEventProcessor_toyDet()
{

}

//---------------------------------
// ~JEventProcessor_toyDet    (Destructor)
//---------------------------------
JEventProcessor_toyDet::~JEventProcessor_toyDet()
{
  testFile->Close();
}

//------------------
// Init
//------------------
void JEventProcessor_toyDet::Init(void)
{
  // This is called once at program startup.
  testFile = new TFile("test.root", "RECREATE");
  aHisto   = new TH1I("aHisto", "a test; a test; NOE", 100, 0, 100);
  bHisto   = new TH1I("bHisto", "b test; b test; NOE", 100, 0, 100);
  cHisto   = new TH1I("cHisto", "c test; c test; NOE", 100, 0, 100);
  dHisto   = new TH1I("dHisto", "d test; d test; NOE", 100, 0, 100);
}

//------------------
// Process
//------------------
void JEventProcessor_toyDet::Process(const std::shared_ptr<const JEvent>& aEvent)
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

  auto hit = aEvent->Get<detHit>();

  for(auto h : hit) {
    double a = h->A; double b = h->B;
    cHisto->Fill(a); dHisto->Fill(b);
  }

}

//------------------
// Finish
//------------------
void JEventProcessor_toyDet::Finish(void)
{
  // This is called when at the end of event processing
  testFile->Write();
}