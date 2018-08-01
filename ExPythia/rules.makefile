#----------------------------------------------------------------------
# Rules
#----------------------------------------------------------------------
# Rules to make .o files (C, C++, Fortran)
$(C_LIB_OBJS): %.$(ObjSuf): %.c
	mkdir -p $(OBJDIR)
	$(CC) -fPIC $(CFLAGS) -c $< -o $(OBJDIR)/$(@F)
%.$(ObjSuf): %.cxx
	mkdir -p $(OBJDIR)
	$(CXX) -fPIC $(CXXFLAGS) -c $< -o $(OBJDIR)/$(@F)
%.$(ObjSuf): %.cpp
	mkdir -p $(OBJDIR)
	$(CXX) -fPIC $(CXXFLAGS) -c $< -o $(OBJDIR)/$(@F)
%.$(ObjSuf): %.C
	mkdir -p $(OBJDIR)
	$(CXX) -fPIC $(CXXFLAGS) -c $< -o $(OBJDIR)/$(@F)
%.$(ObjSuf): %.cc
	mkdir -p $(OBJDIR)
	$(CXX) -fPIC $(CXXFLAGS) -c $< -o $(OBJDIR)/$(@F)
%.$(ObjSuf): %.fpp
	mkdir -p $(OBJDIR)
	$(F77) $(FFLAGS) -x f77-cpp-input -c $< -o $(OBJDIR)/$(@F)
%.$(ObjSuf): %.F
	mkdir -p $(OBJDIR)
	$(F77) $(FFLAGS) -c $< -o $(OBJDIR)/$(@F)
%.$(ObjSuf): %.f
	mkdir -p $(OBJDIR)
	$(F77) $(FFLAGS) -c $< -o $(OBJDIR)/$(@F)
# Libraries
$(TARGET_SHARED_LIB): %.so: $(LIB_OBJS)
	mkdir -p $(LIBDIR)
	$(LD) $(SOFLAGS) $(LIB_OBJS2) $(LIBS) -o $(LIBDIR)/$(@F)
$(TARGET_STATIC_LIB): %.a: $(LIB_OBJS)
	mkdir -p $(LIBDIR)
	ar r $(LIBDIR)/$(@F) $(LIB_OBJS2) $(LIBS)
	ranlib $(LIBDIR)/$(@F)
# Executables
THIS_LIB2 := -L$(LIBDIR) $(patsubst lib%.so,-l%, $(THIS_LIB))
$(C_EXES): %.exe: %.o $(TARGET_SHARED_LIBS)
	mkdir -p $(EXEDIR)
	$(CC) $(LDFLAGS) -o $(EXEDIR)/$(@F) $(OBJDIR)/$(<F) \
		$(THIS_LIB2) $(LIBS)
$(CPP_EXES): %.exe: %.o $(TARGET_SHARED_LIBS)
	mkdir -p $(EXEDIR)
	$(CXX) $(LDFLAGS) -o $(EXEDIR)/$(@F) $(OBJDIR)/$(<F) \
		$(THIS_LIB2) $(LIBS)
$(F_EXES): %.exe: %.o $(TARGET_SHARED_LIBS)
	mkdir -p $(EXEDIR)
	$(F77) $(FFLAGS) -o $(EXEDIR)/$(@F) $(OBJDIR)/$(<F) \
		$(THIS_LIB2) $(LIBS)

# ROOT 5.0 dictionary generation
$(DICTOBJS): %Dict.$(ObjSuf): $(SRCDIR)/%Dict.cxx
	$(CXX) -fPIC $(CXXFLAGS) -c $< -o $(OBJDIR)/$(@F)

$(ROOTDICTSRCS):$(SRCDIR)/%Dict.cxx: $(PROJINCDIR)/%LinkDef.hxx $(ROOTDICTHDRS)
	(cd $(PROJINCDIR); \
	rootcint $(CPPPARSER) -f ../$(SRCDIR)/$(@F) -c -I.. $(INCFLAGS) \
	$(ROOTDICTHDRS) ../$(PROJINCDIR)/$*LinkDef.hxx )


cleanall: cleanobj cleanlib cleandict cleanbak cleanexe
	@touch coredummy; rm -f core*

cleandict: 
	@touch $(SRCDIR)/dummyDictdummy; rm -f $(SRCDIR)/*Dict*

cleanobj:
	@if [[ -d $(OBJDIR) ]]; then touch $(OBJDIR)/dummy.o; rm -f $(OBJDIR)/*.o; fi
	@if [[ -d swig ]]; then (touch swig/dummy.o; rm -f swig/*.o); fi

cleanlib:
	@if [[ -d $(LIBDIR) ]]; then \
	touch $(LIBDIR)/libdummy.so; touch $(LIBDIR)/libdummy.a; \
		rm -f $(LIBDIR)/lib*.so $(LIBDIR)/lib*.a; \
	fi

cleanexe:
	@mkdir -p $(EXEDIR)
	@touch $(EXEDIR)/dummy.exe
	@rm -f $(EXEDIR)/*.exe

cleanbak:
	@touch $(INCDIR)/dummy~;
	@touch $(SRCDIR)/dummy~;
	@if [[ -d $(OBJDIR) ]]; then touch $(OBJDIR)/dummy~; rm $(OBJDIR)/*~; fi
	@touch dummy~;
	@rm -f $(PROJINCDIR)/*~ $(SRCDIR)/*~ *~ 

