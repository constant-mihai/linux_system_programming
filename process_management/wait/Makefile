##############################################################################################
################## Functions, Oneliners & SoupCans*  #########################################
##############################################################################################
# *Does not contain soup!
##############################################################################################

# ---------------------------- Check subdirs for libraries ---------------------------- #
#                                                                                       #
# ------------------------------------------------------------------------------------- #
f_check_subdirs_for_libs = $(if $(library_dirs),$(compile_subdirectories),)


# ---------------------------- Compile subirs ------------------------------------------#
#                                                                                       #
# ------------------------------------------------------------------------------------- #
define f_compile_subdir
cd $(1); make; cd $(CURR_DIR); 
endef

compile_subdirectories = $(foreach dir,$(library_dirs),$(call f_compile_subdir,$(dir)))
clean_subdirectories = $(foreach dir,$(library_dirs),$(call f_clean_subdir,$(dir)))


# ---------------------------- User defined functions ----------------------------------#
#                                                                                       #
# ------------------------------------------------------------------------------------- #
# Look into each directory from subdirs and search for *.(arg).
# Where arg can be:
# A header file
#  - h
#  - hpp
#  - H
# Or a source file
#  - c
#  - cpp
#  - C
f_deep_source_search = $(foreach dir,$(subdirs),$(wildcard $(dir)/*.$(1)))
f_deep_includes_search = $(foreach dir,$(include_dirs),$(wildcard $(dir)/*.$(1)))

# ---------------------------- Compile shared lib --------------------------------------#
#                                                                                       #
# ------------------------------------------------------------------------------------- #
# Compilation command for a shared lib. One liner:
#compile_shared_lib = $(compiler) $(shared_flags) $(objs_without_main) -o $(shl_fullname) $(LDFLAGS); ln -sf $(shl_fullname) $(shl_soname); ln -sf $(shl_fullname) $(shl_linker_name)
# Two liner, define:
define compile_shared_lib
$(compiler) $(shared_flags) $(objs_without_main) -o $(shl_fullname) $(LDFLAGS)
ln -sf $(shl_fullname) $(shl_soname)
ln -sf $(shl_fullname) $(shl_linker_name)
endef

# ---------------------------- Compile Main --------------------------------------------#
#                                                                                       #
# ------------------------------------------------------------------------------------- #
# Test if this is the root directory of the project.
# If it is then compile this as such.
# It it is NOT then compile this as a lib.
compile = $(if $(findstring yes,$(main)),$(compile_main),$(compile_shared_lib))

# ---------------------------- Clean functions -----------------------------------------#
#                                                                                       #
# ------------------------------------------------------------------------------------- #
define f_clean
rm -f *.o; rm -f *.so*;
endef

define f_clean_main
$(f_clean) if [ -a $(name) ]; then rm $(name); fi;
endef

define f_clean_subdir
cd $(1); $(f_clean) cd $(CURR_DIR);
endef

##############################################################################################
########################################## Variables #########################################
##############################################################################################
##############################################################################################

# --------------------------- GNU --------------------------- #
#                                                             #
# ----------------------------------------------------------- # 
SHELL:= /bin/bash
.RECIPEPREFIX := >

# --------------------------- Filetypes --------------------- #
#                                                             #
# ----------------------------------------------------------- # 
.SUFFIXES:
.SUFFIXES: .c .C .cpp .o

# --------------------------- General ----------------------- #
#                                                             #
# ----------------------------------------------------------- # 
name := wait_call
main := yes 

# ---------------------------- Shared library --------------- #
#                                                             #
# ----------------------------------------------------------- # 
shl_name := $(name)
shl_version := 1
shl_release_number := 0
shl_minor_number := 0
shl_linker_name := lib$(shl_name).so
shl_soname := $(shl_linker_name).$(shl_version)
shl_fullname := $(shl_soname).$(shl_minor_number).$(shl_release_number)


# ---------------------------- Directories ------------------ # 
#                                                             #
# ----------------------------------------------------------- # 
subdirs := 
CURR_DIR := $(PWD)
include_dirs := 
library_dirs := 
libraries := 

# ---------------------------- Compiler --------------------- # 
#                                                             #
# ----------------------------------------------------------- # 
CC := gcc
CXX := g++ 
compiler := gcc
# Compilation command for the main program.
compile_main = $(compiler) $(objs) -o $(name) $(LDFLAGS)

# ---------------------------- Headers  --------------------- # 
#                                                             #
# ----------------------------------------------------------- # 
h := $(wildcard *.h) $(call f_deep_includes_search,h)
hpp := $(wildcard *.hpp) $(call f_deep_includes_search,hpp)
cap_h := $(wildcard *.H) $(call f_deep_includes_search,H)


# ---------------------------- Sources ---------------------- # 
#                                                             #
# ----------------------------------------------------------- # 
c_srcs := $(wildcard *.c) $(call f_deep_source_search,c)
cpp_srcs := $(wildcard *.cpp) $(call f_deep_source_search,cpp) 
cxx_srcs := $(wildcard *.C) $(call f_deep_source_search,C) 

srcs = $(c_srcs) $(cpp_srcs) $(cxx_srcs)

# ---------------------------- Objects ---------------------- # 
#                                                             #
# ----------------------------------------------------------- # 
#cxx_objs := ${cxx_srcs:.C=.o}
#cxx_objs += ${cpp_srcs:.cpp=.o}
#c_objs := ${c_srcs:.c=.o}
basenames := $(basename $(srcs))
objs := $(addsuffix .o,$(basenames))
objs_without_main := $(filter-out $(name).o,$(objs))


# ---------------------------- Includes---------------------- # 
#                                                             #
# ----------------------------------------------------------- # 
incs := $(h) $(hpp) $(cap_h)


# ---------------------------- Flags ------------------------ # 
#                                                             #
# ----------------------------------------------------------- # 
shared_flags := -shared -Wl,-soname,$(shl_soname)
CFLAGS += -Wall -fno-diagnostics-show-caret 
CPPFLAGS += -Wall -O0 -fno-diagnostics-show-caret -fPIC

CPPFLAGS += $(foreach includedir,$(include_dirs),-I$(includedir))
LDFLAGS += $(foreach librarydir,$(library_dirs),-L$(librarydir))
LDFLAGS += $(foreach library,$(libraries),-l$(library))


# ---------------------------- Phony targets (aka targets which are not connected to files) 
.PHONY: all clean cleanall debug


##############################################################################################
########################################## Recipes ###########################################
##############################################################################################
##############################################################################################

main: $(objs)
>   @echo Making Main
>   $(f_check_subdirs_for_libs)
>   $(compile)

$(objs): $(srcs) $(incs)

subdirs:

all: main

shared: $(objs)
>   @echo Making Shared lib 
>   $(f_check_subdirs_for_libs)
>   $(compile_shared_lib)

print-%: ; @echo $* = $($*)

print-all: ;
>    @echo ------------------------------ General
>    @echo SHELL                = $(SHELL)
>    @echo name                 = $(name) 
>    @echo ------------------------------------------ Shared library
>    @echo shl_name           = $(shl_name) 
>    @echo shl_version        = $(shl_version)
>    @echo shl_release_number = $(shl_release_number) 
>    @echo shl_minor_number   = $(shl_minor_number)
>    @echo shl_linker_name    = $(shl_linker_name)
>    @echo shl_soname         = $(shl_soname)
>    @echo shl_fullname       = $(shl_fullname)

>    @echo ------------------------------------------ Directories  
>    @echo subdirs              = $(subdirs) 
>    @echo CURR_DIR             = $($CURR_DIR)
>    @echo include_dirs = $(include_dirs) 
>    @echo library_dirs = $(library_dirs)
>    @echo libraries    = $(libraries)

>    @echo ---------------------------- Compiler 
>    @echo CC                   = $(CC) 
>    @echo CXX                  = $(CXX) 
>    @echo compiler             = $(compiler)

>    @echo ---------------------------- Flags
>    @echo shared               = $(shared)
>    @echo CFLAGS               = $(CFLAGS)
>    @echo CPPFLAGS             = $(CPPFLAGS)
>    @echo LDFLAGS              = $(LDFLAGS)

>    @echo ---------------------------- Sources 
>    @echo c_srcs       = $(c_srcs)
>    @echo c_srcs       = $(c_srcs)
>    @echo cxx_srcs     = $(cxx_srcs)
>    @echo ---------------------------- Objects 
>    @echo cxx_objs     = $(cxx_objs)
>    @echo c_objs       = $(c_objs)
>    @echo ---------------------------- Headers 
>    @echo h            = $(h)
>    @echo hpp          = $(hpp)
>    @echo cap_h        = $(cap_h)

clean:
>   $(f_clean_main)

cleanall: 
>   $(if $(findstring yes,$(recursive)),$(clean_subdirectories),)
>   $(f_clean_main)

debug: CPPFLAGS += -g 
debug: all 

debug-shared: CPPFLAGS += -g
debug-shared: shared
