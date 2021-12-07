CC = gcc
CC_FLAGS = -g -pedantic -Wall -Wextra -Wstrict-prototypes -Wmissing-prototypes -Wshadow -fPIC -fstack-protector-strong -Wformat -Werror=format-security -fPIE
LD_FLAGS = -lm
SHARED_FLAGS = -shared
SUBDIRS = deps/plot/src


PLOT = plot
PLOT_SRC = $(wildcard $(CURDIR)/$(SUBDIRS)/*.c)
PLOT_OBJ_ALL = $(PLOT_SRC:.c=.o)
PLOT_OBJ := $(filter-out $($(CURDIR)/$(SUBDIRS)/main.o),$(PSI_OBJ_ALL))

PSI = psimon
PSI_SRC = $(wildcard *.c)
PSI_OBJ_ALL = $(PSI_SRC:.c=.o)
PSI_OBJ := $(filter-out main.o,$(PSI_OBJ_ALL))

.PHONY: all
all: $(PLOT_OBJ_ALL) $(PSI_OBJ_ALL) psimon

.PHONY: $(PLOT_OBJ_ALL)
$(PLOT_OBJ_ALL): %.o: %.c
	$(CC) $(CC_FLAGS) $(SHARED_FLAGS) -c $< -o $@ $(LD_FLAGS)

.PHONY: $(PSI_OBJ_ALL)
$(PSI_OBJ_ALL): %.o: %.c
	$(CC) -I$(CURDIR)/$(SUBDIRS) $(CC_FLAGS) $(SHARED_FLAGS) -c $< -o $@ $(LD_FLAGS)

.PHONY: plot
plot: $(PLOT_OBJ_ALL)
	$(CC) $(CC_FLAGS) $(PLOT_OBJ_ALL) -o $(PLOT) $(LD_FLAGS)

psimon:
	-rm -f $(CURDIR)/$(SUBDIRS)/main.o
	$(CC) -I$(CURDIR)/$(SUBDIRS) $(CC_FLAGS) $(shell echo '$(CURDIR)/$(SUBDIRS)/*.o') $(PSI_OBJ_ALL) -o $(PSI) $(LD_FLAGS)

.PHONY: clean
clean:
		-rm -f $(PLOT) $(PLOT_OBJ_ALL) $(SHARED) $(PSI_OBJ_ALL) $(PSI)

.PHONY: check
check:
		valgrind --leak-check=yes --track-origins=yes ./$(PSI)

.PHONY: check
gdb:
		gdb $(PSI)

