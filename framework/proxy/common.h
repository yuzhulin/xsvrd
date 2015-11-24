#ifndef __COMMON_H__
#define __COMMON_H__
#include <string>

namespace xsvrd {

struct Configuration {
	int proxy_id;
	unsigned short proxy_port;
	struct SvrdList {
		int svrdnum;
		std::string list_file;
	}svrdlist[10];
};

}

#endif
