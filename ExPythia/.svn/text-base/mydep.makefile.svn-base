#
# 1. Define the target executables and libraries
#    There could be as many executables in a project, but only one library
#    is allowed. Both shared library and static libraries could be build
#    for a project. 
#    The executables must have the name XXX.exe, where XXX corresponds to
#    the name of the source file with the main functions.
#    C_EXES for C source files, CPP_EXES for C++ source files and 
#    F_EXES for fortran source files.
# 2. Define how to make all and install, The default is given below
#

# should be set by the command line as "make BUILD_CONFIG='some_value'".
ifeq ($(PACKAGE_NAME),ExPythia)

BUILD_CONFIG = 

TARGET_SHARED_LIB :=
TARGET_STATIC_LIB := 
EXTRA_SRCDIRS     := 
C_EXES            := 
CPP_EXES          := 
F_EXES            := 
# SWIG
SWIG_MODULE       := 
SWIG_I            := 
SWIG_HEADERS      := 
# Scripts and python modules
SCRIPT_FILES      := 
PYTHON_MODULES    := 

TARGET_EXES       := $(C_EXES) $(CPP_EXES) $(F_EXES)

# Specify this library for linking to the executables. 
# Other libraries will be linked to the ones in the install area.
THIS_LIB          := $(TARGET_SHARED_LIB) $(TARGET_STATIC_LIB)

# ROOT 5.0 dictionary generation with Reflex
# Need to include root.makefile from the main Makefile
ROOTDICT_HEADERS = 
ROOTDICT_LINKDEF = 

all: $(TARGET_SHARED_LIB) $(TARGET_STATIC_LIB) $(TARGET_EXES) $(SWIG_LIB)

install: all
	make install_lib install_exe install_headers install_python_modules

endif # Local to ExPythia

# Public section
DEPENDS_ON += 
CXXFLAGS += -I$(TK_INCDIR)
INCFLAGS += -I$(TK_INKDIR)
LIBS += -L$(TK_LIBDIR)

