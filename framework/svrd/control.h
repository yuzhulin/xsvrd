#ifndef __CONTROL_H__
#define __CONTROL_H__

class Control {
public:
	Control() {}
	virtual~Control() {} 

	virtual int ReadConfigFile() = 0;
	virtual int PrepareToRun() = 0;
	virtual int Init() {
		return ReadConfigFile();
	}
	virtual int Run() = 0;

	void set_config_file(std::string& config_file) {
		config_file_ = config_file;
	}
	std::string get_config_file() {
		return config_file_;
	}

private:
	std::string config_file_;
};

#endif
