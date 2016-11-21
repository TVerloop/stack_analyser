##=============================================================================#
# makefile
#
# author: Tom Verloop
# last change: 16/6/2016
#
# Makefile c/c++ projects
#=============================================================================#


#=============================================================================#
# include configuration
#=============================================================================#

include makefile.cfg

#=============================================================================#
# set the VPATH according to SRCS_DIRS
#=============================================================================#

VPATH = $(SRCS_DIRS)

#=============================================================================#
# when using output folder, append trailing slash to its name
#=============================================================================#

ifeq ($(strip $(OUT_DIR)), )
	OUT_DIR_F =
else
	OUT_DIR_F = $(strip $(OUT_DIR))/
endif

#=============================================================================#
# various compilation flags
#=============================================================================#

# core flags
CORE_FLAGS =

# flags for C++ compiler
CXX_FLAGS = -std=$(CXX_STD) -g3 -Wall

# flags for C compiler
C_FLAGS = -std=$(C_STD) -g3 -Wall

# flags for assembler
AS_FLAGS = 
# flags for linker
LD_FLAGS =

#=============================================================================#
# do some formatting
#=============================================================================#

CXX_OBJS = $(addprefix $(OUT_DIR_F), $(notdir $(CXX_SRCS:.$(CXX_EXT)=.o)))
C_OBJS = $(addprefix $(OUT_DIR_F), $(notdir $(C_SRCS:.$(C_EXT)=.o)))
AS_OBJS = $(addprefix $(OUT_DIR_F), $(notdir $(AS_SRCS:.$(AS_EXT)=.o)))
OBJS = $(AS_OBJS) $(C_OBJS) $(CXX_OBJS) $(USER_OBJS)
DEPS = $(OBJS:.o=.d)
INC_DIRS_F = -I. $(patsubst %, -I%, $(INC_DIRS))
LIB_DIRS_F = $(patsubst %, -L%, $(LIB_DIRS))
ifeq ($(OS),Windows_NT)
EXECUTABLE = $(OUT_DIR_F)$(PROJECT).exe
else
EXECUTABLE = $(OUT_DIR_F)$(PROJECT)
endif
# format final flags for tools, request dependancies for C++, C and asm
CXX_FLAGS_F = $(CORE_FLAGS) $(OPTIMIZATION) $(CXX_WARNINGS) $(CXX_FLAGS)  $(CXX_DEFS) -MD -MP -MF $(OUT_DIR_F)$(@F:.o=.d) $(INC_DIRS_F)
C_FLAGS_F = $(CORE_FLAGS) $(OPTIMIZATION) $(C_WARNINGS) $(C_FLAGS) $(C_DEFS) -MD -MP -MF $(OUT_DIR_F)$(@F:.o=.d) $(INC_DIRS_F)
AS_FLAGS_F = $(CORE_FLAGS) $(AS_FLAGS) $(AS_DEFS) -MD -MP -MF $(OUT_DIR_F)$(@F:.o=.d) $(INC_DIRS_F)
LD_FLAGS_F = $(CORE_FLAGS) $(LD_FLAGS) $(LIB_DIRS_F)

#contents of output directory
GENERATED = $(wildcard $(patsubst %, $(OUT_DIR_F)*.%, bin d dmp elf hex lss lst map o su stk cgraph asm))
#=============================================================================#
# make all
#=============================================================================#

all : make_output_dir $(EXECUTABLE)

# make object files dependent on Makefile
$(OBJS) : makefile makefile.cfg


#-----------------------------------------------------------------------------#
# linking - objects -> Executable
#-----------------------------------------------------------------------------#

$(EXECUTABLE) : $(OBJS)
	@echo 'Linking target: $(PROJECT)'
	$(CXX) $(LD_FLAGS_F) $(OBJS) $(LIBS) -o $@
	@echo ' '

#-----------------------------------------------------------------------------#
# compiling - C++ source -> objects
#-----------------------------------------------------------------------------#

$(OUT_DIR_F)%.o : %.$(CXX_EXT)
	@echo 'Compiling file: $<'
	$(CXX) -c $(CXX_FLAGS_F) $< -o $@
	@echo ' '

#-----------------------------------------------------------------------------#
# compiling - C source -> objects
#-----------------------------------------------------------------------------#

$(OUT_DIR_F)%.o : %.$(C_EXT)
	@echo 'Compiling file: $<'
	$(CC) -c $(C_FLAGS_F) $< -o $@
	@echo ' '

#-----------------------------------------------------------------------------#
# assembling - ASM source -> objects
#-----------------------------------------------------------------------------#

$(OUT_DIR_F)%.o : %.$(AS_EXT)
	@echo 'Assembling file: $<'
	$(AS) -c $(AS_FLAGS_F) $< -o $@
	@echo ' '

#-----------------------------------------------------------------------------#
# create the desired output directory
#-----------------------------------------------------------------------------#

make_output_dir :
	$(shell mkdir $(OUT_DIR_F) 2>/dev/null)

#=============================================================================#
# make clean
#=============================================================================#

clean:
ifeq ($(strip $(OUT_DIR_F)), )
	@echo 'Removing all generated output files'
else
	@echo 'Removing all generated output files from output directory: $(OUT_DIR_F)'
endif
ifneq ($(strip $(GENERATED)), )
	$(RM) $(GENERATED)
else
	@echo 'Nothing to remove...'
endif

#=============================================================================#
# make doxygen
#=============================================================================#

doxygen :
	@echo 'generating doxygen documentation'
	doxygen

#=============================================================================#
# global exports
#=============================================================================#

.PHONY: all clean dependents

.SECONDARY:

# include dependancy files
-include $(DEPS)
