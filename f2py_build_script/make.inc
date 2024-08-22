this_makefile := $(lastword $(MAKEFILE_LIST))
this_dir := $(dir $(this_makefile))

#==========================================#
# settings
#==========================================#
ifdef F2PY_MODS
TARGET_MODNAME     ?= fmod4py
TARGET_F2PY        ?= $(F2PY_OUT_DIR)/$(TARGET_MODNAME)$(F2PY_EXT_SUFFIX)
endif
TARGET_F90PROGS    ?= $(call progname2target,$(F90_PROGS))
TARGET_LIB         ?= $(LIB_DIR)/lib$(TARGET_LIBNAME).a

F2PY_EXT_SUFFIX    ?= $(shell python3-config --extension-suffix)
F90PROG_EXT_SUFFIX ?= .x

GD     ?= $(this_dir)gd/gen-deps.awk
GD_INC ?= $(this_dir)gd/gen-inc-deps.awk

MOD_DIR          := $(abspath $(MOD_DIR))
LIB_DIR          := $(abspath $(LIB_DIR))
BUILD_OUTSRC_DIR ?= $(BUILD_SRC_DIR)/other/


#==========================================#
# procedures
#==========================================#
_get_exist_pathlist    = $(eval _plist:=$(foreach path, ${1}, $(if $(wildcard ${path}),${path})))
_get_notexist_pathlist = $(eval _plist:=$(foreach path, ${1}, $(if $(wildcard ${path}),,${path})))
rmrf_if_exist      = $(call _get_exist_pathlist,${1})$(if ${_plist},-rm -rf ${_plist})
rm_if_exist        = $(call _get_exist_pathlist,${1})$(if ${_plist},-rm ${_plist})
mkdir_if_not_exist = $(call _get_notexist_pathlist,${1})$(if ${_plist},mkdir -p ${1})

progname2target = $(addprefix $(BIN_DIR)/, $(patsubst %.prog, %$(F90PROG_EXT_SUFFIX), ${1}))


#==========================================#
# flags
#==========================================#
fflags     += $(OPENMP) $(FFLAGS) $(INC) $(LIB) $(SHARED) $(FMOD_OUTPUT)
ld_flags   += $(OPENMP) $(FFLAGS) $(INC) $(LIB) -L$(LIB_DIR) -l$(TARGET_LIBNAME)

f2py_flags += $(F2PY_FLAGS) -L$(LIB_DIR) -l$(TARGET_LIBNAME) $(F2PY_EXTRA_LIB) $(LIB) $(INC)
ifdef F2PY_LOG
f2py_flags += > $(F2PY_LOG) 2>&1
endif

# backend of f2py (version >= 1.26) is meson
# => "--fcompiler" & "--f90flags" options are not used
f2py = FC="$(FC)" FFLAGS="$(OPENMP) $(FFLAGS)" $(F2PY)

# backend is distutils [not recommended]
# f2py = $(F2PY) --fcompiler='$(FC)' --f90flags='$(OPENMP) $(FFLAGS)'




##################################################################
#                                                                #
# main                                                           #
#                                                                #
##################################################################
f2py: $(TARGET_F2PY)
f90: $(TARGET_F90PROGS)


#==========================================#
# f90 -> object file
#==========================================#
vpath % .: $(BUILD_SRC_DIR)

%.o: % | $(MOD_DIR) $(LIB_DIR)
	$(FC) -c $< -o $@ $(fflags)
	$(if $($@.is_prog),,\
		$(AR) $(TARGET_LIB) $@\
	)


#==========================================#
# preprocessing and copy
#==========================================#
define fpp_rule
${1}: ${2} | $(SRC_INC_DEP)
	$(call mkdir_if_not_exist,$(dir ${1}))
	$(FPP) ${2} ${1}

endef


#==========================================#
# directory
#==========================================#
$(BIN_DIR) $(LIB_DIR) $(MOD_DIR) $(DEP_DIR):
	mkdir -p $@




##################################################################
# start - generate & import dependency -                         #
##################################################################
ifeq ($(findstring clean, $(MAKECMDGOALS)),)


#==========================================#
# generate dependencies - include files -
#==========================================#
$(SRC_INC_DEP): $(SRCS) | $(DEP_DIR)
	$(GD_INC) $(SRCS) > $(SRC_INC_DEP)
# 	$(GD_INC) $^ > $@

include $(SRC_INC_DEP)


#==========================================#
# make FPP rules, inc dependencies, and $(all_build_srcs)
#==========================================#
append_slash     = $(if $(filter %/, ${1}),${1},$(addsuffix /, ${1}))
src_dir          = $(call append_slash,$(SRC_DIR))
build_src_dir    = $(call append_slash,$(BUILD_SRC_DIR))
build_outsrc_dir = $(call append_slash,$(BUILD_OUTSRC_DIR))

# constant
_srcs_in_srcdir  = $(filter $(src_dir)%, $(SRCS))
_srcs_out_srcdir = $(filter-out $(src_dir)%, $(SRCS))
build_srcs += $(patsubst $(src_dir)%,$(build_src_dir)%,$(_srcs_in_srcdir))
build_srcs += $(addprefix $(build_outsrc_dir), $(notdir $(_srcs_out_srcdir)))
outsrcdirs = $(sort $(dir $(filter-out $(src_dir)%, $(SRCS))))

# temporary variable and macro depending on $(src)
_src_dir = $(firstword $(foreach _dir, $(src_dir) $(outsrcdirs), \
	$(if $(filter ${_dir}%, ${src}),${_dir}))\
)
_build_src_dir = $(if $(filter $(src_dir), $(_src_dir)),$(build_src_dir),$(build_outsrc_dir))
src2buildsrc = $(patsubst $(_src_dir)%, $(_build_src_dir)%, ${1})

inc_srcs       = $($(src).inc)
build_inc_src  = $(call src2buildsrc,$(inc_src))
build_inc_srcs = $(call src2buildsrc,$(inc_srcs))
build_src      = $(call src2buildsrc,$(src))

# FPP rules and inc dependencies
$(foreach src, $(SRCS),\
	$(eval $(call fpp_rule,$(build_src),$(src)))\
	$(eval $(build_src): $(build_inc_srcs))\
	$(foreach inc_src, $(inc_srcs),\
		$(eval $(call fpp_rule,$(build_inc_src),$(inc_src)))\
	)\
)

# get all_build_srcs (including inc files)
all_build_srcs = $(build_srcs) $(foreach src, $(SRCS), $(build_inc_srcs))


#==========================================#
# generate dependencies of build srcs
#==========================================#
$(BUILD_SRC_DEP): $(DEP_DIR) $(SRC_INC_DEP) | $(all_build_srcs)
	$(GD) $(build_srcs) > $(BUILD_SRC_DEP)
# 	$(GD) $^ > $@

# define build src variables
obj = $(src).o
$(foreach src,$(build_srcs),$(eval $(src) := $(obj)))

# include build src dependencies
include $(BUILD_SRC_DEP)


#==========================================#
# dependencies
#==========================================#
# dependencies to make object files in $(BUILD_SRC_DIR)
obj = $($(src))
dep = $($(obj))
$(foreach src, $(build_srcs), $(eval $(obj): $(dep)))

# dependencies for f2py
src = $(patsubst %.o, %, $(obj))
obj = $($(mod))
dep = $($(obj))
$(foreach mod, $(F2PY_MODS), $(eval $(TARGET_F2PY): $(src) $(dep)))

# dependencies for f90 programs
target = $(call progname2target,$(prog))
obj = $($(prog))
$(foreach prog, $(F90_PROGS), $(eval $(target): $(obj)))

endif
##################################################################
# end - generate & import dependency -                           #
##################################################################




##################################################################
# targets                                                        #
##################################################################
$(TARGET_F2PY):
	$(f2py) -m $(TARGET_MODNAME) -c $^ $(f2py_flags)
	$(call mkdir_if_not_exist,$(dir $@))
	$(if $(patsubst ./%,,$(dir $@)),mv $(notdir $@) $@)

$(TARGET_F90PROGS): | $(BIN_DIR)
	$(FC) $^ $(ld_flags) -o $@




##################################################################
#                                                                #
# clean                                                          #
#                                                                #
##################################################################
clean:
	$(call rm_if_exist,$(TARGET_F90PROGS))
	$(call rm_if_exist,$(TARGET_F2PY))
	$(call rm_if_exist,$(TARGET_LIB))
	$(call rmrf_if_exist,$(BUILD_DIR))
