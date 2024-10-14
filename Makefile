# I'm using minGW (min-GNU for Windows): https://www.mingw-w64.org/
# It support make (originally called mingw32-make.exe under \bin) and g++


# Detect all .cpp, turn into .o format and define .exe name
SOURCES := $(wildcard */*.cpp */*/*.cpp */*/*/*.cpp)
OUTS := $(addsuffix .o, $(basename $(SOURCES)))
DEPS := $(addsuffix .d, $(basename $(SOURCES)))
EXE := hw1

# compiler flags
FLAGS := -O -Wall # optimize a little 
DEBUG_FLAGS := -O0 -g -Wall # flags for GDB debug and turn on all warnings
RELEASE_FLAGS := -O2 # optimize lots! 

# cool beeping noise (ASCII: \007) worked on Windows cmd
# typed by Notepad++: Plugins > Converter > HEX -> ASCII, convert 07 to 
BEEP := 


# =============== GLUT parameters ==================
USING_GLUT := freeglut
# directory that contain `GL/glut.h` stuffs:
GLUT_INCLUDE_ARGU := -I"./GLUT_env"
# directory that contain `freeglut.lib.a`:
GLUT_LIB_ARGU := -L"./GLUT_env"
# ==================================================


# In Windows(Windows_NT: Windows New Technology architecture), command will differ. i.e. `del` and must replace `/` with `\` in paths
ifeq ($(OS), Windows_NT)
	EXE := $(addsuffix .exe, $(EXE))
	OUTS := $(subst /,\,$(OUTS))
	DEPS := $(subst /,\,$(DEPS))
	CLEAR_COMMAND := del
else
	CLEAR_COMMAND := rm
endif


# First off, make all .o be depended(generated) and linked to form .EXE
# $@: the target, $^: "all" dependencies, @ in front of a line surpress printing the line
$(EXE): $(OUTS)
	g++ $(FLAGS) -o $@ $^ $(GLUT_LIB_ARGU) -l$(USING_GLUT) -lopengl32
	@echo $(BEEP)  



# Next, each .o will find its .cpp and be compiled seperately(not linked) if .cpp or HEADERS changed
# $<: the first of the dependecy list, -MMD generate dependency list(local included .h and .cpp) of .o files into .d files 
-include $(DEPS) # include dependeny list of `.d`s. `-`: ignore error of ".d files not found" in the first build
%.o: %.cpp
	g++ $(FLAGS) -MMD -o $@ -c $(GLUT_INCLUDE_ARGU) $<


# debug build, make flags become DEBUG_FLAGS and trigger $(EXE) dependency
.PHONY: debug
debug: FLAGS := $(DEBUG_FLAGS)
debug: $(EXE)
	
# release build
.PHONY: release
release: FLAGS := $(RELEASE_FLAGS)
release: $(EXE)

# remove all .d, .o and .exe by `make clean`
.PHONY: clean
clean:
	$(CLEAR_COMMAND) $(DEPS) $(OUTS) $(EXE)
	@echo $(BEEP) 


# P.S.
#	why not -MP?: it solves "deleting .h, .d file not update, .o still depends .h" issue by adding PHONY target, not elegant. So just rebuild the project!
# 	why not "-f"?: it solves "not found $(DEPS) $(OUTS) $(EXE) when deleting" issue. But no one want/would delete those manually, right?
#	those .o .d files kinda annoying? All makefiles I googled let it be. Maybe that's one of the reason using CMake or Scon