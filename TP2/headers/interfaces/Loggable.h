#include <string>

#ifndef LOGGABLE_H
#define LOGGABLE_H

class Loggable {
    public:
    	Loggable(){}
	    virtual ~Loggable(){}
	    virtual std::string logMemberVariables()=0;
};
#endif // LOGGABLE_H
