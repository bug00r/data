_CFLAGS:=$(CFLAGS)
CFLAGS:=$(_CFLAGS)
_LDFLAGS:=$(LDFLAGS)
LDFLAGS:=$(_LDFLAGS)

ARFLAGS?=rcs
PATHSEP?=/
BUILDROOT?=build

BUILDDIR?=$(BUILDROOT)$(PATHSEP)$(CC)
BUILDPATH?=$(BUILDDIR)$(PATHSEP)

ifndef PREFIX
	INSTALL_ROOT=$(BUILDPATH)
else
	INSTALL_ROOT=$(PREFIX)$(PATHSEP)
	ifeq ($(INSTALL_ROOT),/)
	INSTALL_ROOT=$(BUILDPATH)
	endif
endif

ifdef DEBUG
	CFLAGS+=-ggdb
	ifeq ($(DEBUG),)
	CFLAGS+=-Ddebug=1
	else 
	CFLAGS+=-Ddebug=$(DEBUG)
	endif
endif

ifeq ($(M32),1)
	CFLAGS+=-m32
	BIT_SUFFIX+=32
endif

override CFLAGS+=-std=c11 -Wpedantic -pedantic-errors -Wall -Wextra
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

override LDFLAGS+=$(TESTLIBDIR) $(TESTLIB)

all: createdir $(LIB)

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(BUILDDIR)/%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<
	
$(TESTBIN): $(LIB)
	$(CC) $(CFLAGS) $(TESTSRC) -o $@ $(LDFLAGS)

.PHONY: clean createdir test

test: createdir $(TESTBIN)
	./$(TESTBIN)

createdir:
	mkdir -p $(BUILDDIR)

clean:
	-rm -dr $(BUILDROOT)

install:
	$(info install at: $(INSTALL_ROOT))
	mkdir -p $(INSTALL_ROOT)include
	mkdir -p $(INSTALL_ROOT)lib$(BIT_SUFFIX)
	cp ./$(NAME).h $(INSTALL_ROOT)include/$(NAME).h
	cp $(LIB) $(INSTALL_ROOT)lib$(BIT_SUFFIX)/$(LIBNAME)
