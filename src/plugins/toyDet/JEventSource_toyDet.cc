//
//    File: toyDet/JEventSource_toyDet.cc
// Created: Wed Apr 24 16:04:14 EDT 2019
// Creator: pooser (on Linux rudy.jlab.org 3.10.0-957.10.1.el7.x86_64 x86_64)
//
// ------ Last repository commit info -----
// [ Date ]
// [ Author ]
// [ Source ]
// [ Revision ]

#include "JEventSource_toyDet.h"

//---------------------------------
// JEventSource_toyDet    (Constructor)
//---------------------------------
JEventSource_toyDet::JEventSource_toyDet(std::string source_name, JApplication *app):JEventSource(source_name, app)
{
  // Don't open the file/stream here. Do it in the Open() method below.

  // Uncomment the line below and modify it to specify all of the data types this source
  // provides. The "MyHit" and "MyTrigger" are just examples of JObject types you might
  // define (they don't actually exist). The purpose of this is to create empty JFactory
  // objects that will serve as containers for the data the source produces.
  // Make sure that all types are listed as template arguments here!!
  // mFactoryGenerator = new JSourceFactoryGenerator<MyHit, MyTrigger>();
}

//---------------------------------
// ~JEventSource_toyDet    (Destructor)
//---------------------------------
JEventSource_toyDet::~JEventSource_toyDet()
{
  // Delete JFactoryGenerator if we created one
  if( mFactoryGenerator != nullptr ) delete mFactoryGenerator;

  // Close the file/stream here.
  ifs.close();
}

//------------------
// Open
//------------------
void JEventSource_toyDet::Open(void)
{
  // Open the file/stream here. The name of the source will be in mName
  ifs.open(mName);
  if (!ifs) {
    cout << "!!! Unable to open in JEventSource_toyDet::Open !!!" << endl;
    exit(1);  // terminate with error
  }
}

//------------------
// GetEvent
//------------------
// std::shared_ptr<const JEvent> JEventSource_toyDet::GetEvent(void)
void JEventSource_toyDet::GetEvent(std::shared_ptr<JEvent> event)
{

  // Read an event (or possibly block of events) from the source return it.
	
  // If an event was successfully read in, return kSUCCESS. If there
  // are no more events in the source to read, return kNO_MORE_EVENTS.
  // If the source has no events at the moment, but may later (e.g.
  // a live stream) then return kTRY_AGAIN;

  // Throw exception if we have exhausted the source of events
  static size_t nevents = 0; // by way of example, just count 1000000 events
  if( ++nevents > 10 ) throw JEventSource::RETURN_STATUS::kNO_MORE_EVENTS;

  // if( true ) return JEventSource::RETURN_STATUS::kSUCCESS;

  // This should read an event from the input stream and make a
  // JEventtoyDet out of it.
  // auto jevent = new JEvent_toyDet();
  // ... add event data to jevent ...
  if (ifs.is_open()) {

    int lineCntr = 0; int chanCntr = 0; int eventCntr = 0;
    while (getline(ifs, line)) {

      size_t eventDelim = line.find('@');
      if (eventDelim != string::npos) {

	eventCntr++; chanCntr = 0;
	tdcData.clear(); adcData.clear();

      }

      cout << "Level 0: event = " << eventCntr << endl;
      cout << "Level 0: line = " << line << endl;
      cout << "Level 0: event tdc vec size = "   << tdcData.size() << endl;
      cout << "Level 0: event adc vec size = "   << adcData.size() << endl;

      size_t chanDelim = line.find('#');
      if (chanDelim != string::npos) {
     
        lineCntr = 0; chanCntr++;

	cout << "chanCntr = " << chanCntr << endl;

        if (chanCntr > 1) {  
	  
	  cout << "Pushing back rawSamples data : " << "chanCntr - 1  = " << chanCntr - 1 << endl;
	  data.push_back(new rawSamples(eventCntr, chanCntr - 1, tdcData, adcData));
	  tdcData.clear(); adcData.clear();
	  cout << "data size = " << data.size() << endl;

        }

	cout << "Level 1: event = " << eventCntr << endl;
        cout << "Level 1: line = " << line << endl;
        cout << "Level 1: event tdc vec size = "   << tdcData.size() << endl;
        cout << "Level 1: event adc vec size = "   << adcData.size() << endl;

      }

      lineCntr++;

      if (lineCntr > 0 && lineCntr % 2 == 0 && 
	  eventDelim == string::npos && chanDelim == string::npos) { // tdc data
	istringstream istr(line);
	double tdcSample;
	while (!istr.eof()) {
	  istr >> tdcSample; if (!istr) break;
	  tdcData.push_back(tdcSample);
	}
        cout << "tdc data" << endl;
        for (auto data : tdcData) cout << data << endl;
      }
      if (lineCntr > 0 && lineCntr % 2 != 0 && 
	  eventDelim == string::npos && chanDelim == string::npos) { // adc data
	istringstream istr(line);
	double adcSample;
	while (!istr.eof()) {
	  istr >> adcSample; if (!istr) break;
	  adcData.push_back(adcSample);
	}
        cout << "adc data" << endl;	   
        for (auto data : adcData) cout << data << endl;
      }

      cout << "Level 2: event = " << eventCntr << endl;
      cout << "Level 2: line = " << line << endl;
      cout << "Level 2: event tdc vec size = "   << tdcData.size() << endl;
      cout << "Level 2: event adc vec size = "   << adcData.size() << endl;

      line.clear();

    }
    // grab last channel data
    data.push_back(new rawSamples(eventCntr, chanCntr - 1, tdcData, adcData));
    cout << "data size = " << data.size() << endl;

    event->Insert(new rawSamples(eventCntr, chanCntr - 1, tdcData, adcData));

  }
  else {
    cout << "!!! Unable to open in JEventSource_toyDet::GetEvent !!!" << endl;
    exit(1); // terminate with error
  }
	
  // return std::shared_ptr<JEvent>( (JEvent*)jevent );

}

//------------------
// GetObjects
//------------------
bool JEventSource_toyDet::GetObjects(const std::shared_ptr<const JEvent>& aEvent, JFactory* aFactory)
{
  // This should create objects that are of the type aFactory holds using
  // data from aEvent. The objects should be transferred to aFactory which
  // serves simply as a container. Two possible paradigms are shown below
  // as OPTION 1 and OPTION 2

  // lock_guard<mutex> lck(vsamples_mutex);

  // auto vsamples = vsamples_deque.pop();
  // (*aEvent).Insert( vrawsamples );

  return true;

}

// The following method is optional in case you need to implement a more 
// complicated scheme. If not defined, a default JTask will be created for
// each event that simply runs all event processors on it. (This is
// probably what you want so feel free to delete this section.)
//
// An example of when you would implement this method is if you wanted
// to split the parsing of the input data  through multiple stages.
// For example, the first stage might simply split the larger buffer
// which contains many events into smaller buffers containing single
// events (e.g. disentangling). The JEventProcessor tasks should only
// be attached to single events rather than blocks so it makes sense
// to make the parsing of the blocks a different flavor of JTask.
// See the documentation for more details.
//------------------
// GetProcessEventTask
//------------------
// std::shared_ptr<JTaskBase> JEventSource_toyDet::GetProcessEventTask(std::shared_ptr<const JEvent>&& aEvent)
//{
// 
//}
