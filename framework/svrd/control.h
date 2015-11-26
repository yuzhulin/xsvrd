#ifndef __CONTROL_H__
#define __CONTROL_H__

class Control {
public:
	Control() {}
	virtual~Control() {} 

	virtual int PrepareToRun() = 0;
	virtual int Run() = 0;
};

#endif
