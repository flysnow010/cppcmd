CC ?= gcc
CXX ?= g++
AR ?= ar
ECHO ?= echo

CSRC := $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(CSRC)))

CPPS := $(wildcard $(SRCDIR)/*.cpp)
CPPOBJS := $(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(CPPS)))

all: $(APPNAME)
	@make -ts --no-print-directory

$(APPNAME): $(OBJS) $(CPPOBJS) $(LIBS) $(BINDIR)
	@$(ECHO) Link $(APPNAME)
	@$(CXX) $(OBJS) $(CPPOBJS) $(LIBS) $(LINKFLAGS) -o $(APPNAME)

$(OBJS): $(OBJDIR)/%.o:$(SRCDIR)/%.c $(INCFILES) $(OBJDIR)
	@$(ECHO) Cc $@
	@$(CC) -c $(CFLAGS) $(INCS) $< -o $@

$(CPPOBJS): $(OBJDIR)/%.o:$(SRCDIR)/%.cpp  $(INCFILES) $(OBJDIR)
	@$(ECHO) C++ $@
	@$(CXX) -c $(C++FLAGS) $(INCS) $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(BINDIR):
	@mkdir -p $(BINDIR)

.PHNOY: clean
clean:
ifeq ($(wildcard $(OBJDIR)), $(OBJDIR))
	@rm -rf $(OBJDIR)
endif
	@rm -f $(APPNAME)