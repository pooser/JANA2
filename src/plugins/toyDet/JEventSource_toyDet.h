//
//    File: toyDet/JEventSource_toyDet.h
// Created: Wed Apr 24 16:04:14 EDT 2019
// Creator: pooser (on Linux rudy.jlab.org 3.10.0-957.10.1.el7.x86_64 x86_64)
//
// ------ Last repository commit info -----
// [ Date ]
// [ Author ]
// [ Source ]
// [ Revision ]

#ifndef _JEventSource_toyDet_h_
#define _JEventSource_toyDet_h_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <JANA/JEventSource.h>
#include <JANA/JEventSourceGeneratorT.h>

#include "JEvent_toyDet.h"

#include "detHit.h"
#include "rawSamples.h"

using namespace std;

// This first bit is optional. It allows you to specify how likely this class
// is to read from a given source. Uncomment this if you want to return something
// other than the default of 0.01 which is OK for most implementations.
// template<> double JEventSourceGeneratorT<JEventSource_toyDet>::CheckOpenable(std::string source) {
// 	if( source.find(".evio") != source.npos ) return 1.0;
// }

//////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief class description.
///
/// Detailed class description.
//////////////////////////////////////////////////////////////////////////////////////////////////
class JEventSource_toyDet : public JEventSource{
  
 public:
                
  JEventSource_toyDet(std::string source_name, JApplication *app);
  virtual ~JEventSource_toyDet();
		
  static std::string GetDescription(void){ return "My Event source"; }
  void Open(void);
  std::shared_ptr<const JEvent> GetEvent(void);
  bool GetObjects(const std::shared_ptr<const JEvent>& aEvent, JFactory* aFactory);
  // std::shared_ptr<JTaskBase> GetProcessEventTask(std::shared_ptr<const JEvent>&& aEvent);

 private:
  
  // User defined variables
  ifstream ifs;
  string line;
  vector <double> tdcData, adcData;
  vector <rawSamples*> data;

};

#endif // _JEventSource_toyDet_h_

