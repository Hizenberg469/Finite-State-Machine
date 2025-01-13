#Setting a default configuration if invoked with just "make":
CFG ?= debug

# Config specific settings
ifeq ($(CFG),debug)
CFLAGS += -g -DDEBUG
else
CFLAGS += -O3 -DNDEBUG
endif

inform:
ifneq ($(CFG),release)
ifneq ($(CFG),debug)
	@echo "Invalid configuration "$(CFG)" specified."
	@echo
	@echo "Possible choices for configuration are 'CFG=release' and 'CFG=debug'"
	@exit 1
endif
endif
	@echo "Configuration "$(CFG)
	@echo "--------------------------------------------------"

rule : dep | inform

BUILD_DIR=bin/$(CFG)
OBJS_DIR=objs/$(CFG)
LIBDIR=lib/${CFG}
SRCDIR=src
#--------------------------------------------------


APP=$(BUILD_DIR)/app
APP_OBJECT=$(OBJS_DIR)/app.o
FSMLIB_NAME=fsm
FSMLIB=$(LIBDIR)/lib$(FSMLIB_NAME)
CODEDIRS=./src
INCDIRS=. ./include/ # can be list

CC=gcc
OPT=-O0
#generate files that encode make rules for the .h dependencies
DEPFLAGS=-MP -MD
#automatically add the -I onto each include directory
CFLAGS+=-Wall -Wextra $(foreach D,$(INCDIRS),-I$(D)) $(OPT) $(DEPFLAGS)

# for-style iteration (foreach) and regular expression completions (wildcard)
CFILES=$(foreach D, $(CODEDIRS),$(wildcard $(D)/*.c))
# regular expression replacement
OBJECTS=$(patsubst %.c,$(OBJS_DIR)/%.o,$(CFILES))
DEPFILES=$(patsubst %.c,$(OBJS_DIR)/%.d,$(CFILES))

all: $(FSMLIB) $(APP) | create_dir

create_dir:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(OBJS_DIR)
	mkdir -p $(LIBDIR)


$(APP): $(APP_OBJECT) | create_dir
	$(CC) -o $@ $^ -L $(LIBDIR) -l$(FSMLIB_NAME)

$(FSMLIB): $(OBJECTS) | create_dir
	ar cr $(LIBDIR)/${FSMLIB}.a ${OBJECTS}

# only want the .c file dependency here, thus $< instead of $^.
#
$(OBJS_DIR)/%.o:$(SRCDIR)/%.c | create_dir
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJS_DIR)/%.o:%.c | create_dir
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(APP) $(APP_OBJECT) $(OBJECTS) $(DEPFILES)
	rm -rf $(BUILD_DIR) $(OBJS_DIR) $(LIBDIR)

# shell commands are a set of keystrokes away
distribute: clean
	tar zcvf dist.tgz *

# @ silences the printing of the command
# $(info ...) prints output
diff:
	$(info The status of the repository, and the volume of per-file changes:)
	@git status
	@git diff --stat

# include the dependencies
-include $(DEPFILES)

# add .PHONY so that the non-targetfile - rules work even if a file with the same name exists.
.PHONY: all clean distribute diff