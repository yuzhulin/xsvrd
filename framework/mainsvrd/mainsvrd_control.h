#ifndef __MAINSVRD_CONTROL_H__
#define __MAINSVRD_CONTROL_H__
#include <string>
#include "common.h"
#include "../../framework/svrd/control.h"

class MainsvrdControl : public Control {
public:
	MainsvrdControl();
	virtual~MainsvrdControl();
	virtual int Init(); 
	virtual int ReadConfigFile();
	virtual int PrepareToRun();
	virtual int Run();

private:
	Configuration configuration_;	
};

#endif /* __MAINSVRD_CONTROL_H__ */
