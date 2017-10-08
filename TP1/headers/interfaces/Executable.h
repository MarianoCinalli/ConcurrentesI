#ifndef EXECUTABLE_H
#define EXECUTABLE_H

class Executable {
    public:

    	Executable(){}
	    virtual ~Executable(){}
        virtual void execute()=0;
        
};
#endif // EXECUTABLE_H
