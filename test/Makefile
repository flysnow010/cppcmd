PROJECT_NAME := test

PWD := $(shell pwd)
TOP := $(PWD)/..
EXEMKFILE := $(TOP)/mkfiles/exe.mk
INCS := -I$(TOP)/inc -I$(PWD)/inc
SRCDIR := $(PWD)
OBJDIR := $(PWD)/.obj
BINDIR := $(TOP)/bin
INCFILES := $(SRCDIR)/cmdtest.h 
LIBS :=  $(TOP)/lib/libcppcmd.a $(PWD)/lib/libcpptest.a
APPNAME := $(BINDIR)/$(PROJECT_NAME)

CFLAGS := 
C++FLAGS := -std=c++11
LINKFLAGS :=

include $(EXEMKFILE)