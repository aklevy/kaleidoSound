# Attempt to load a config.make file.
# If none is found, project defaults in config.project.make will be used.
ifneq ($(wildcard config.make),)
	include config.make
endif

# make sure the the OF_ROOT location is defined
ifndef OF_ROOT
    OF_ROOT=$(realpath ../../..)
endif


# call the project makefile!
include $(OF_ROOT)/libs/openFrameworksCompiled/project/makefileCommon/compile.project.mk

#for osx
copy:
	cp bin/libossia.dylib bin/kaleidoSound.app/Contents/MacOS
	install_name_tool -change "libossia.dylib" "@executable_path/libossia.dylib" bin/kaleidoSound.app/Contents/MacOS/kaleidoSound

#ifeq($(shell uname -s),Darwin)
#	cp bin/libossia.dylib bin/kaleidoSound.app/Contents/MacOS
#	install_name_tool -change "libossia.dylib" "@executable_path/libossia.dylib" bin/kaleidoSound.app/Contents/MacOS/kaleidoSound
#endif

