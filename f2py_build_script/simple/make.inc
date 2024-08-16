this_makefile := $(lastword $(MAKEFILE_LIST))
this_dir := $(dir $(this_makefile))

#==========================================#
# settings
#==========================================#
TARGET_LIBNAME ?= myproj

BUILD_SRC_DIR  ?= $(BUILD_DIR)/src
MOD_DIR        ?= $(BUILD_DIR)/mod
DEP_DIR        ?= $(BUILD_SRC_DIR)/dep

SHARED         = -shared -fpic
F2PY_FLAGS     ?= --f2cmap $(F2PY_F2CMAP)
F2PY_F2CMAP    ?= $(this_dir)/../f2py_f2cmap
F2PY_LOG       ?=

SRC_INC_DEP    ?= $(DEP_DIR)/dep-src-inc.make.inc
BUILD_SRC_DEP  ?= $(DEP_DIR)/dep-build-src.make.inc

include $(this_dir)/../make.inc
