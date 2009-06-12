# User variables - can be set as environment variables or as make arguments
#
# PYTHON_VERSION	version of Python used for compilation, e.g. "2.3"
# PYTHON_INCDIR         python header files
# NUMPY_INCDIR		numpy header files
# DEBUG			debugging flags (-gstabs+), turns off optimizations

# MODULE
ifndef MODULE
MODULE = atom
endif

# figure out user variables
# PYTHON_VERSION
ifndef PYTHON_VERSION
PYTHON_VERSION = $(shell python -c 'import sys; print sys.version[:3]')
endif

# PYTHON_INCDIR
ifndef PYTHON_INCDIR
PYTHON_INCDIR = /usr/include/python$(PYTHON_VERSION)
endif

# NUMPY_INCDIR
ifndef NUMPY_INCDIR
NUMPY_INCDIR = $(shell python -c "import numpy; import os.path; print os.path.split(numpy.__file__)[0] + '/core/include'")
endif

# Objcryst headers
ifndef OBJCRYST_HEADERS
OBJCRYST_HEADERS=/home/chris/local/src/ObjCryst/ObjCryst
endif 

# Objcryst libraries - the first 4 should be compiled as shared libraries
OBJCRYSTLIBS = -lcryst -lRefinableObj -lCrystVector -lQuirks -lcctbx -lnewmat

# CFLAGS
ifdef DEBUG
CFLAGS = $(DEBUG) -fPIC -Wall
else
CFLAGS = -fPIC -O3 -w -ffast-math -pipe -funroll-loops
endif

# Shared libary for so files
ifndef LIB
LIB = /home/chris/local/lib
endif

# Shared library for python files
ifndef PYLIB
PYLIB = .
endif

# end of setup

CC        = g++
LINK      = g++
LINKFLAGS = -L$(LIB) -L.


# Where to search for dependencies
VPATH = $(LIB)

# Needed for python wrapper
MODOBJ = $(MODULE).o
MODSO = _$(MODULE).so

## Directives

.PHONY : $(MODULE)
$(MODULE): $(MODSO)


### Rules

## Shared library object

## Boost 
 
$(MODSO) : $(MODOBJ) _registerconverters.so
	$(LINK) -shared $(LINKFLAGS) $< -o $@ -lm $(OBJCRYSTLIBS) -lboost_python -lpython$(PYTHON_VERSION) 

$(MODOBJ) : $(MODULE)_ext.cpp
	$(CC) -c $< -o $@ -DHAVE_CONFIG_H -I$(PYTHON_INCDIR) -I$(OBJCRYST_HEADERS)

_registerconverters.so : registerconverters.o
	$(LINK) -shared $(LINKFLAGS) $< -o $@ -lm $(OBJCRYSTLIBS) -lboost_python -lpython$(PYTHON_VERSION) 

## Object Rules
#
registerconverters.o : registerconverters.cpp converters.hpp
 
%.o:%.cpp
	$(CC) -c $< -DHAVE_CONFIG_H -I$(PYTHON_INCDIR) -I$(NUMPY_INCDIR) -I$(OBJCRYST_HEADERS)

## In case you don't have a shared libobjcryst

# The libraries are inter-dependent, so this should cover all the bases. It
# links in the order given in the ObjCryst, then reverses to pick up undefined
# symbols, and then does the original order to close the loops. It seems to
# work.
.PHONY : clean
clean: 
	-rm -f *.pyc *.so *.o *.a