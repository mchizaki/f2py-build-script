MAKEFLAGS += --no-builtin-rules --no-builtin-variables

#==========================================#
# targets and srcs
#==========================================#
# f2py targets
TARGET_MODNAME = myproj4py
F2PY_MODS += module1_name.mod
F2PY_MODS += module2_name.mod

# f90 prog targets
F90_PROGS += program1_name.prog
F90_PROGS += program2_name.prog

# f90 srcs
SRCS += $(wildcard $(SRC_DIR)/*.f90)
SRCS += $(wildcard ./srctest/*.f90)

# libname
TARGET_LIBNAME = myproj


#==========================================#
# build settings
#==========================================#
SRC_DIR        = ./src
BUILD_DIR      = ./build
BUILD_SRC_DIR  = $(BUILD_DIR)/src
MOD_DIR        = $(BUILD_DIR)/mod
LIB_DIR        = $(BUILD_DIR)/lib
BIN_DIR        = $(BUILD_DIR)/bin
F2PY_OUT_DIR   = $(LIB_DIR)
DEP_DIR        = $(BUILD_SRC_DIR)/dep

FC             = gfortran
F2PY           = f2py
FPP            = cpp -P -traditional $(DFLAGS)
# FPP            = cp -f
AR             = ar cr

F2PY_FLAGS     = --f2cmap $(F2PY_F2CMAP)
F2PY_F2CMAP    = ${F2PY_BUILD_SCRIPT}/f2py_f2cmap
F2PY_LOG       = $(BUILD_SRC_DIR)/f2py.log

LIB            =
INC            = $(addprefix -I, $(MOD_DIR))
FFLAGS         = -ffree-line-length-0 -O3
FMOD_OUTPUT    = $(addprefix -J, $(MOD_DIR))
DFLAGS         = -D_MYFLAG
SHARED         = -shared -fpic
OPENMP         = -fopenmp
F2PY_EXTRA_LIB = -lgomp -lgfortran

SRC_INC_DEP    = $(DEP_DIR)/dep-src-inc.make.inc
BUILD_SRC_DEP  = $(DEP_DIR)/dep-build-src.make.inc


#==========================================#
# main
#==========================================#
all: f2py f90

F2PY_BUILD_SCRIPT ?= .
include ${F2PY_BUILD_SCRIPT}/make.inc

.PHONY: all clean
.PHONY: f2py f90
