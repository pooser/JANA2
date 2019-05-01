//
//    File: test/JEventSource_test.cc
// Created: Tue Apr 30 08:37:05 EDT 2019
// Creator: pooser (on Darwin Erics-MacBook-Pro.local 18.5.0 i386)
//
// ------ Last repository commit info -----
// [ Date ]
// [ Author ]
// [ Source ]
// [ Revision ]

#include "JEventSource_test.h"
#include "MyEvent.h"

//---------------------------------
// JEventSource_test    (Constructor)
//---------------------------------
JEventSource_test::JEventSource_test(std::string source_name, JApplication *app):JEventSource(source_name, app)
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
// ~JEventSource_test    (Destructor)
//---------------------------------
JEventSource_test::~JEventSource_test()
{
	// Delete JFactoryGenerator if we created one
	if( mFactoryGenerator != nullptr ) delete mFactoryGenerator;

	// Close the file/stream here.
}

//------------------
// Open
//------------------
void JEventSource_test::Open(void)
{
	// Open the file/stream here. The name of the source will be in mName.
	// e.g.  auto ifs = new ifstream(mName);
}

//------------------
// GetEvent
//------------------
std::shared_ptr<const JEvent> JEventSource_test::GetEvent(void)
{

  	// Throw exception if we have exhausted the source of events
	static size_t Nevents = 0; // by way of example, just count 1000000 events
	if( ++Nevents > 100 ) throw JEventSource::RETURN_STATUS::kNO_MORE_EVENTS;
	
	// This should read an event from the input stream and make a
	// JEventtest out of it.

	auto jevent = new MyEvent();
	// ... add event data to jevent ...
	return std::shared_ptr<JEvent>( (JEvent*)jevent );
}

//------------------
// GetObjects
//------------------
bool JEventSource_test::GetObjects(const std::shared_ptr<const JEvent>& aEvent, JFactory* aFactory)
{
	// This should create objects that are of the type aFactory holds using
	// data from aEvent. The objects should be transferred to aFactory which
	// serves simply as a container. Two possible paradigms are shown below
	// as OPTION 1 and OPTION 2
	
	auto name = aFactory->GetName();   // name of class aFactory holds
	auto tag  = aFactory->GetTag();    // aFactory tag

	if( aFactory->GetName() != "MyHit" ) return false;
	
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
	  double x = 10.; double E = 5.; double t = 1.;
	  // hits.push_back(new MyHit(x, E, t));
	  // aFactory->Set(hits);
	  // ((JFactoryT<MyHit>*) aFactory)->Insert(new MyHit(x, E, t));
	  static_cast <JFactoryT<MyHit>*> (aFactory)->Insert(new MyHit(x, E, t));
	}
	else if(name == "MyTrigger"){
		// ... similarly for additional data types this source provides
	}
	else{
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
// std::shared_ptr<JTaskBase> JEventSource_test::GetProcessEventTask(std::shared_ptr<const JEvent>&& aEvent)
//{
// 
//}
