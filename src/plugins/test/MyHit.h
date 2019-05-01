
#ifndef _MyHit_h_
#define _MyHit_h_

#include <JANA/JObject.h>

class MyHit:public JObject{
	public:

                MyHit(double x, double E, double t) : x(x), E(E), t(t) {}
		double x, E, t;
		
};

#endif // _MyHit_h_XS
