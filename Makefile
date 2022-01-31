MAKE?=mingw32-make
AR?=ar
ARFLAGS?=rcs
PATHSEP?=/
CC=gcc
BUILDROOT?=build

ifeq ($(CLANG),1)
	export CC=clang
endif

BUILDDIR?=$(BUILDROOT)$(PATHSEP)$(CC)
BUILDPATH?=$(BUILDDIR)$(PATHSEP)

INSTALL_ROOT?=$(BUILDPATH)

ifeq ($(DEBUG),1)
	export debug=-ggdb -Ddebug=1
	export isdebug=1
endif

ifeq ($(ANALYSIS),1)
	export analysis=-Danalysis=1
	export isanalysis=1
endif

ifeq ($(DEBUG),2)
	export debug=-ggdb -Ddebug=2
	export isdebug=1
endif

ifeq ($(DEBUG),3)
	export debug=-ggdb -Ddebug=3
	export isdebug=1
endif

ifeq ($(OUTPUT),1)
	export outimg= -Doutput=1
endif

BIT_SUFFIX=

ifeq ($(M32),1)
	CFLAGS+=-m32
	BIT_SUFFIX+=32
endif

CFLAGS+=-std=c11 -Wpedantic -pedantic-errors -Wall -Wextra
#-ggdb -O1
#-pg for profiling 

NAME=data

SRC=$(NAME).c

LIBNAME=lib$(NAME).a
LIB=$(BUILDPATH)$(LIBNAME)
OBJS=$(BUILDPATH)$(NAME).o

TESTSRC=test_$(NAME).c
TESTBIN=$(BUILDPATH)test_$(NAME).exe
TESTLIB=-l$(NAME)
TESTLIBDIR=-L$(BUILDPATH)


all: createdir $(LIB) $(TESTBIN)

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $(LIB) $(OBJS)

$(OBJS): data.h
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJS) $(debug)
	
$(TESTBIN):
	$(CC) $(CFLAGS) $(TESTSRC) -o $(TESTBIN) $(TESTLIBDIR) $(TESTLIB) $(debug)

.PHONY: clean createdir test

test:
	./$(TESTBIN)

createdir:
	mkdir -p $(BUILDDIR)

clean:
	-rm -dr $(BUILDROOT)

install:
	mkdir -p $(INSTALL_ROOT)include
	mkdir -p $(INSTALL_ROOT)lib$(BIT_SUFFIX)
	cp ./data.h $(INSTALL_ROOT)include/data.h
	cp $(LIB) $(INSTALL_ROOT)lib$(BIT_SUFFIX)/$(LIBNAME)
