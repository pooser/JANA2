
#ifndef _JFACTORYGENERATOR_TEST_H_
#define _JFACTORYGENERATOR_TEST_H_

#include <JANA/JFactoryGenerator.h>
#include <JANA/JFactoryT.h>
#include "MyHit.h"

class JFactoryGenerator_test:public JFactoryGenerator{
	public:

	void GenerateFactories(JFactorySet *factory_set){
		
	  factory_set->Add(new JFactoryT<MyHit>());

	}
};

#endif   // _JFACTORYGENERATOR_TEST_H_
