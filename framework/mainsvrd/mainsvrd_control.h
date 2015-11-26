#ifndef __MAINSVRD_CONTROL_H__
#define __MAINSVRD_CONTROL_H__
#include <string>
#include "../../framework/svrd/control.h"

class MainsvrdControl : public Control {
public:
	MainsvrdControl();
	virtual~MainsvrdControl();
	virtual int Init(); 
	virtual int ReadConfigFile();
	virtual int PrepareToRun();
	virtual int Run();
};

#endif /* __MAINSVRD_CONTROL_H__ */
