import platform
import shutil

cursystem = platform.system()
if "Windows" == cursystem:
	postfix = ".dll"
elif "Linux" == cursystem:
	postfix = ".so"
elif "Darwin" == cursystem:
	postfix = ".dylib"

shutil.copy2("../../../libs/libcnf" + postfix, "./")
shutil.copy2("../../../libs/liblog" + postfix, "./")