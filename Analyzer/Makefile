-include $(ANALYZERPATH)/Makefile.common

.PHONY: lib all

lib: libLQDM.so
all: lib

libLQDM.so: libLQDMClassDictionaries.so $(OBJECTS)
	@echo "--> Creating custom shared library."
	@$(CC) $(CFLAGSDICT) -shared $(OBJECTS) $(LFLAGS) -o $(ANALYZERPATH)/$(LIBDIR)/$@ $(ROOTLIBS) -lLQDMClassDictionaries

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cc
	@echo "--> Creating object $@"
	@$(CC) $(CFLAGS) -fPIC -c $< -o $@ $(LIBSMAIN)


libLQDMClassDictionaries.so: $(LIBDIR)/ClassDictionaries.cxx
	@echo "--> Creating shared library with custom class dictionaries."
	@$(CC) $(CFLAGSDICT) $(LFLAGS) -shared -o $(ANALYZERPATH)/$(LIBDIR)/$@ $(ROOTLIBS) $(CMSSWLIBS) $^

$(LIBDIR)/ClassDictionaries.cxx: $(INCLUDES) include/Linkdef.hpp
	@echo "--> Creating custom class dictionaries."
	@rootcling -f $@ -c -p -I${LEAFPATH} $^

clean:
	@echo "--> cleaning folders $(OBJDIR)/, $(LIBDIR)/"
	@rm -f $(wildcard $(OBJDIR)/*.o) $(LIBOBJS)
