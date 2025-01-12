#Setting a default configuration if invoked with just "make":
CFG ?= debug

# Config specific settings
ifeq ($(CFG),debug)
CFLAGS += -DDEBUG
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

BUILD_DIR = build/$(CFG)
OBJS_DIR = $(BUILD_DIR)/objs
#--------------------------------------------------


BINARY=$(BUILD_DIR)/fsm
CODEDIRS=. ./src
INCDIRS=. ./include/ # can be list

CC=gcc
OPT=-O0
#generate files that encode make rules for the .h dependencies
DEPFLAGS=-MP -MD
#automatically add the -I onto each include directory
CFLAGS=-Wall -Wextra -g $(foreach D,$(INCDIRS),-I$(D)) $(OPT) $(DEPFLAGS)

# for-style iteration (foreach) and regular expression completions (wildcard)
CFILES=$(foreach D, $(CODEDIRS),$(wildcard $(D)/*.c))
# regular expression replacement
OBJECTS=$(patsubst %.c,$(OBJS_DIR)/%.o,$(CFILES))
DEPFILES=$(patsubst %.c,$(OBJS_DIR)/%.d,$(CFILES))

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^

# only want the .c file dependency here, thus $< instead of $^.
#
%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BINARY) $(OBJECTS) $(DEPFILES)

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