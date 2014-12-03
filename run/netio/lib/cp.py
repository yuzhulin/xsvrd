import os
import platform
import shutil

cursystem = platform.system()
if "Windows" == cursystem:
	postfix = ".dll"
elif "Linux" == cursystem:
	postfix = ".so"
elif "Darwin" == cursystem:
	postfix = ".dylib"

libcnf = "../../../libs/libcnf" + postfix
liblog = "../../../libs/liblog" + postfix

# check file exist
if os.path.isfile(libcnf):
	shutil.copy2(libcnf, "./")

if os.path.isfile(liblog):
	shutil.copy2(liblog, "./")