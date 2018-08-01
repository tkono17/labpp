PROJ_NAME=$(shell basename `pwd`)
INCDIR = .
SRCDIR = src
OBJDIR = obj
LIBDIR = lib
EXEDIR = exe
PROJINCDIR=$(PROJ_NAME)

# Expected suffix for various types of files
CppSrcSuf = cxx cpp C cc
CppIncSuf = hxx hpp H hh
ObjSuf    = o
LibSuf    = so
CSrcSuf   = c
CIncSuf   = h
FSrcSuf   = f fpp F

vpath %.hxx $(INCDIR)
vpath %.hpp $(INCDIR)
vpath %.hh $(INCDIR)
vpath %.H $(INCDIR)
vpath %.h $(INCDIR)
vpath %.inc $(INCDIR)

vpath %.cpp $(SRCDIR)
vpath %.cxx $(SRCDIR) $(SRCDIR)/main $(EXTRA_SRCDIRS)
vpath %.cc $(SRCDIR)
vpath %.C $(SRCDIR)
vpath %.c $(SRCDIR)
vpath %.f $(SRCDIR)
vpath %.for $(SRCDIR)
vpath %.fpp $(SRCDIR)
vpath %.java $(SRCDIR)
vpath %Dict.h $(SRCDIR)

vpath %.$(ObjSuf) $(OBJDIR)
vpath %.$(LibSuf) $(LIBDIR)
vpath %.exe $(EXEDIR)

CC            = gcc
CFLAGS        = -O
CXX = g++
CXXFLAGS = -Wall -O -I$(INCDIR)
CFLAGS = -O -I$(INCDIR)
FFLAGS = -O -I$(INCDIR)
INCFLAGS = -I$(INCDIR)
LD = g++
LDFLAGS = -O
SOFLAGS = -shared
LIBS = -L$(TK_ROOT)/lib
SWIG = swig
F77 = g77

OBJS    = $(C_OBJS) $(CPP_OBJS) $(FOR_OBJS)


#----------------------------------------------------------------------
# ROOT
#----------------------------------------------------------------------
ROOTCFLAGS := $(shell root-config --cflags)
ROOTLIBS   := $(shell root-config --libs)
ROOTGLIBS  := $(shell root-config --glibs)
CXXFLAGS += $(ROOTCFLAGS)
LIBS     += $(ROOTLIBS)
GLIBS    += $(ROOTGLIBS)

#----------------------------------------------------------------------
# FastJet+LHEF
#----------------------------------------------------------------------
SW_DIR=$(PH_SW_DIR)
fastjet_dir = $(SW_DIR)
CXXFLAGS += -I$(fastjet_dir)/include 
LIBS     += -L$(fastjet_dir)/lib -lfastjet
CXXFLAGS += -I$(SW_DIR)/MG5_aMC_v2_4_0/ExRootAnalysis
LIBS     += -L$(SW_DIR)/MG5_aMC_v2_4_0/ExRootAnalysis -lExRootAnalysis
#----------------------------------------------------------------------
# HepMC
#----------------------------------------------------------------------
hepmc_dir = $(SW_DIR)
CXXFLAGS += -I$(hepmc_dir)/include
LIBS     += -L$(hepmc_dir)/lib -lHepMC

#----------------------------------------------------------------------
# Pythia8
#----------------------------------------------------------------------
LIBS += -lpythia8 -lMcData

.PHONY: cleanall cleanobj cleanlib cleanbak cleandict
.PHONY: all install

#----------------------------------------------------------------------
# Pattern matching
#----------------------------------------------------------------------
srcdirs = $(SRCDIR) # Add other directories if they exist
#CPP_EXES    := 
#C_EXES      := 
#F_EXES    := 
C_MAIN_OBJS = $(patsubst %.exe,%.$(ObjSuf), $(C_EXES))
CPP_MAIN_OBJS = $(patsubst %.exe,%.$(ObjSuf), $(CPP_EXES))
F_MAIN_OBJS = $(patsubst %.exe,%.$(ObjSuf), $(F_EXES))
MAIN_OBJS   := $(CPP_MAIN_OBJS) $(C_MAIN_OBJS) $(F_MAIN_OBJS)

C_OBJS      = $(foreach dir, $(srcdirs), \
                $(patsubst $(dir)/%.$(CSrcSuf),%.$(ObjSuf), \
                $(foreach suf, $(CSrcSuf), \
                $(wildcard $(dir)/*.$(suf)))))
C_LIB_OBJS      = $(filter-out $(C_MAIN_OBJS), $(C_OBJS))
C_LIB_OBJS2     = $(addprefix $(OBJDIR)/, $(C_LIB_OBJS))

CPP_OBJS  = $(foreach dir, $(srcdirs), \
                $(filter-out $(DICTOBJS),\
                $(addsuffix .$(ObjSuf), $(basename $(patsubst $(dir)/%, %, \
                $(foreach suf, $(CppSrcSuf), $(wildcard $(dir)/*.$(suf)))) \
                ))))
CPP_LIB_OBJS     = $(filter-out $(CPP_MAIN_OBJS), $(CPP_OBJS))
CPP_LIB_OBJS2    = $(addprefix $(OBJDIR)/, $(CPP_LIB_OBJS))

# ROOT dictionary
ROOTDICTHDRS = $(foreach suf, h hxx, \
        $(patsubst %.$(suf),$(PROJINCDIR)/%.$(suf), \
        $(filter %.$(suf), $(ROOTDICT_HEADERS))))
ROOTDICTSRCS = $(patsubst %LinkDef.hxx,$(SRCDIR)/%Dict.cxx, \
                $(ROOTDICT_LINKDEF))
DICTOBJS  += $(patsubst $(SRCDIR)/%.cxx,%.$(ObjSuf), $(ROOTDICTSRCS))
DICTOBJS2 += $(addprefix $(OBJDIR)/, $(DICTOBJS))

LIB_OBJS      = $(CPP_LIB_OBJS) $(DICTOBJS) $(C_LIB_OBJS) $(F_LIB_OBJS)
LIB_OBJS2     = $(addprefix $(OBJDIR)/, $(LIB_OBJS))
