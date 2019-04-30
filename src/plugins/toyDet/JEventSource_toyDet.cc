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
// convStrToVecDbl
//------------------

vector <double> JEventSource_toyDet::convStrToVecDbl(string strToConv) {
  istringstream istr(strToConv); 
  double data; vector <double> dataVec;
  while (!istr.eof()) {
    istr >> data; if(!istr) break;
    dataVec.push_back(data);
  }
  return dataVec;
}

//------------------
// GetEvent
//------------------
std::shared_ptr<const JEvent> JEventSource_toyDet::GetEvent(void)
{

  	// Read an event (or possibly block of events) from the source return it.
	
	// If an event was successfully read in, return kSUCCESS. If there
	// are no more events in the source to read, return kNO_MORE_EVENTS.
	// If the source has no events at the moment, but may later (e.g.
	// a live stream) then return kTRY_AGAIN;

  	// Throw exception if we have exhausted the source of events
	static size_t nevents = 0; // by way of example, just count 1000000 events
	if( ++nevents > 1 ) throw JEventSource::RETURN_STATUS::kNO_MORE_EVENTS;

	// if( true ) return JEventSource::RETURN_STATUS::kSUCCESS;

	// This should read an event from the input stream and make a
	// JEventtoyDet out of it.
	auto jevent = new JEvent_toyDet();
	// ... add event data to jevent ...

	if (ifs.is_open()) {

	  int lineCntr = 0; int chanCntr = 0;
	  while (getline(ifs, line)) {

	    cout << "Level 0: line = " << line << endl;
	    cout << "Level 0: event tdc vec size = "   << eventTdcData.size() << endl;
	    cout << "Level 0: event adc vec size = "   << eventAdcData.size() << endl;
	    cout << "Level 0: channel tdc vec size = " << chanTdcData.size() << endl;
	    cout << "Level 0: channel adc vec size = " << chanAdcData.size() << endl;

	    size_t chanDelim = line.find('#');
	    if (chanDelim != string::npos) {
     
	      lineCntr = 0; chanCntr++;
	      if (chanCntr > 1) {
		chanTdcData.push_back(eventTdcData);
		chanAdcData.push_back(eventAdcData);  
		eventTdcData.clear(); eventAdcData.clear();
	      }

	      cout << "Level 1: line = " << line << endl;
	      cout << "Level 1: event tdc vec size = "   << eventTdcData.size() << endl;
	      cout << "Level 1: event adc vec size = "   << eventAdcData.size() << endl;
	      cout << "Level 1: channel tdc vec size = " << chanTdcData.size() << endl;
	      cout << "Level 1: channel adc vec size = " << chanAdcData.size() << endl;

	    }

	    lineCntr++;

	    if (lineCntr > 0 && lineCntr % 2 == 0 && chanDelim == string::npos) { // tdc data
	      eventTdcData = convStrToVecDbl(line);
	      // cout << "tdc data" << endl;
	      // for (auto data : eventTdcData) cout << data << endl;
	      // jevent->Insert(eventTdcData);
	    }
	    if (lineCntr > 0 && lineCntr % 2 != 0 && chanDelim == string::npos) { // adc data
	      eventAdcData = convStrToVecDbl(line);
	      // cout << "adc data" << endl;	   
	      // for (auto data : eventTdcData) cout << data << endl;
	    }

	    cout << "Level 2: line = " << line << endl;
	    cout << "Level 2: event tdc vec size = "   << eventTdcData.size() << endl;
	    cout << "Level 2: event adc vec size = "   << eventAdcData.size() << endl;
	    cout << "Level 2: channel tdc vec size = " << chanTdcData.size() << endl;
	    cout << "Level 2: channel adc vec size = " << chanAdcData.size() << endl;

	    line.clear();

	  }

	  // TOTAL HACK! MUST FIND VIABLE SOLUTION
	  if (chanCntr == 100) {
	    chanTdcData.push_back(eventTdcData);
	    chanAdcData.push_back(eventAdcData);  
	  }

	  cout << "Level 3: channel tdc vec size = " << chanTdcData.size() << endl;
	  cout << "Level 3: channel adc vec size = " << chanAdcData.size() << endl;

	}
	else {
	  cout << "!!! Unable to open in JEventSource_toyDet::GetEvent !!!" << endl;
	  exit(1); // terminate with error
	}
	
	A->push_back(1.0); B->push_back(2);
	jevent->Insert(A);
	jevent->Insert(B);

	return std::shared_ptr<JEvent>( (JEvent*)jevent );
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
	
	auto name = aFactory->GetName();   // name of class aFactory holds
	auto tag  = aFactory->GetTag();    // aFactory tag
	
	// Most implementations will not fill tagged factories from event sources.
	// Your implementation should verify that the name, tag combo for this
	// aFactory is one it can provide objects for.
	if( !tag.empty() ) return false; // aFactory does not hold a type produced by this source
	
	// OPTION 1 ------------------------------------------------------------------
	// Check the type of data being requested is one we can provide. If so,
	// make the objects and hand them over to aFactory.
	if( name == "MyHit" ){
		// vector<MyHit*> hits;
		// ... create MyHit objects from data in aEvent and add them to hits vector
		// aFactory->Set( hits);
	}else if(name == "MyTrigger"){
		// ... similarly for additional data types this source provides
	}else{
		// we don't provide this type of object
		return false;
	}
	return true; // we do produce this type of object and they are now in aFactory
	
	// OPTION 2 ------------------------------------------------------------------
	// In many cases the data in aEvent may just be a big buffer that must be
	// parsed to extract all objects. In this case, it is more efficient to
	// parse it once and produce all objects from a single pass. These should
	// be placed in their respective JFactory objects, of which aFactory is
	// just one. Pointers to other factories in the set can be obtained via
	// the aEvent pointer. Here's an example:
	//
	// vector<MyHit*> hits;
	// vector<MyTrigger*> trigs;
	// ...
	// ...  parse buffer, filling hits,trigs,... etc. vectors
	// ...
	// JFactory *myHitFac  = aEvent->GetFactory<MyHit>();
	// JFactory *myTrigFac = aEvent->GetFactory<MyTrigger>();
	// ...
	//  myHitFac->Set( hits );
	//  myTrigFac->Set( trigs );
	//
	// This last part is trickier since we need to return "true" only if the
	// type of object in aFactory is one we produced. Even though we may have
	// produced many objects and filled many factories, the return value should
	// refer only to whether aFactory has been filled.
	// return name=="MyHit" || name=="MyTrigger";
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
