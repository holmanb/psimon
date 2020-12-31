CC = gcc
CC_FLAGS = -g -pedantic -Wall -Wextra -Wstrict-prototypes -Wmissing-prototypes -Wshadow -Wconversion -fPIC -lm -fstack-protector-strong -Wformat -Werror=format-security -fPIE#-fstack-usage  -Wstack-protector #-Wstack-usage

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


all: $(PLOT_OBJ_ALL) $(PSI_OBJ_ALL) 
.PHONY : all

.PHONY: $(PLOT_OBJ_ALL)
$(PLOT_OBJ_ALL): %.o: %.c
	$(CC) $(CC_FLAGS) $(SHARED_FLAGS) -c $< -o $@ 

.PHONY: $(PSI_OBJ_ALL)
$(PSI_OBJ_ALL): %.o: %.c
	$(CC) -I$(CURDIR)/$(SUBDIRS) $(CC_FLAGS) $(SHARED_FLAGS) -c $< -o $@ 

.PHONY: plot
plot: $(PLOT_OBJ_ALL) 
	$(CC) $(CC_FLAGS) $(PLOT_OBJ_ALL) -o $(PLOT)

.PHONY: psimon
psimon: 
	-rm -f $(CURDIR)/$(SUBDIRS)/main.o
	$(CC) -I$(CURDIR)/$(SUBDIRS) $(CC_FLAGS) $(shell echo '$(CURDIR)/$(SUBDIRS)/*.o') $(PSI_OBJ_ALL) -o $(PSI)

.PHONY: clean 
clean:
		-rm -f $(PLOT) $(PLOT_OBJ_ALL) $(SHARED) $(PSI_OBJ_ALL) $(PSI)

.PHONY: check
check:
		valgrind --leak-check=yes --track-origins=yes ./$(PSI)

.PHONY: check
gdb:
		gdb $(PSI)

