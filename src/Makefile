PROJECT_NAME ?= cppcmd

PWD := $(shell pwd)
TOP := $(PWD)/..
LIBMKFILE := $(TOP)/mkfiles/lib.mk
INCS := -I$(TOP)/inc
SRCDIR := $(TOP)/src
LIBDIR := $(TOP)/lib
OBJDIR := $(PWD)/.obj
INCFILES := $(SRCDIR)/cmdhelper.h 
LIBNAME := $(LIBDIR)/lib$(PROJECT_NAME).a

CFLAGS := 
C++FLAGS := -std=c++11

include $(LIBMKFILE)