#include "../../include/libcommon/x_thread.h"

namespace xsvrd {

class HandlerThread : public Thread {
public:
	HandlerThread();
	virtual~HandlerThread();

	virtual Routine();
	virtual PrepareToRun();
	virtual IsToBeBlocked();
};

}
