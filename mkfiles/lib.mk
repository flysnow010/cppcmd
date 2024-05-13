CC ?= gcc
CXX ?= g++
AR ?= ar
ECHO ?= echo
MAKE ?= make

LIBFLAGS := -rcD

CSRC := $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(CSRC)))

CPPS := $(wildcard $(SRCDIR)/*.cpp)
CPPOBJS := $(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(CPPS)))

all: $(LIBNAME)
	@make -ts --no-print-directory

$(LIBNAME): $(OBJS) $(CPPOBJS) $(LIBDIR)
	@$(ECHO) Ar $(LIBNAME)
	@$(AR) $(LIBFLAGS) $(LIBNAME) $(OBJS) $(CPPOBJS) 

$(OBJS): $(OBJDIR)/%.o:$(SRCDIR)/%.c $(INCFILES) $(OBJDIR)
	@$(ECHO) Cc $@
	@$(CC) -c $(CFLAGS) $(INCS) $< -o $@

$(CPPOBJS): $(OBJDIR)/%.o:$(SRCDIR)/%.cpp $(INCFILES) $(OBJDIR) 
	@$(ECHO) C++ $@
	@$(CXX) -c $(C++FLAGS) $(INCS) $< -o $@

$(LIBDIR):
	@mkdir -p $(LIBDIR)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

.PHNOY: clean
clean:
ifeq ($(wildcard $(OBJDIR)), $(OBJDIR))
	@rm -rf $(OBJDIR)
endif
	@rm -f $(LIBNAME)