#=============================================================================
# Makefile to build a C++ library linked to ROOT
#=============================================================================

TARGET_SHARED_LIB := libMcData.so
TARGET_STATIC_LIB := 
EXTRA_SRCDIRS     := 
C_EXES            := 
CPP_EXES          := runAnalysis.exe ExRootLHEFJetConverter.exe \
	ExRootHepMCJetConverter.exe
F_EXES            := 
# Scripts and python modules
SCRIPT_FILES      := 
PYTHON_MODULES    := 

TARGET_EXES       := $(C_EXES) $(CPP_EXES) $(F_EXES)

# Specify this library for linking to the executables. 
# Other libraries will be linked to the ones in the install area.
THIS_LIB          := $(TARGET_SHARED_LIB) $(TARGET_STATIC_LIB)

# ROOT 5.0 dictionary generation with Reflex
# Need to include root.makefile from the main Makefile
ROOTDICT_HEADERS = McEvent.hxx McParticle.hxx McVertex.hxx Jet.hxx IndexPair.hxx PJEvent.hxx
ROOTDICT_LINKDEF = LinkDef.hxx

all: $(TARGET_SHARED_LIB) $(TARGET_STATIC_LIB) $(TARGET_EXES) $(SWIG_LIB)

include basic.makefile

INSTALL_DIR=$(SWDIR)
#install: all#
#	cp -r McData $(INSTALL_DIR)/include
#	cp lib/lib*.so $(INSTALL_DIR)/lib

include rules.makefile

headers=$(shell ls -1 McData/*.* |grep -v *~)

install: all
	@echo " ** Copy header files to $(INSTALL_DIR)/include"
	@mkdir -p $(INSTALL_DIR)/include/McData	
	@cp $(headers) $(INSTALL_DIR)/include/McData
	@echo " ** Copy libraries to $(INSTALL_DIR)/lib"
	@cp lib/lib*.so $(INSTALL_DIR)/lib/
	@echo " ** Copy executables to $(INSTALL_DIR)/bin"
	@cp exe/*.exe $(INSTALL_DIR)/bin/
