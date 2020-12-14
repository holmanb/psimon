CC = gcc
CC_FLAGS = -g -pedantic -Wall -Wextra -Wstrict-prototypes -Wmissing-prototypes -Wshadow -Wconversion -fPIC -lm
SHARED_FLAGS = -shared
SUBDIRS = deps/plot/src


PLOT = plot
PLOT_SRC = $(wildcard $(CURDIR)/$(SUBDIRS)/*.c)
PLOT_OBJ_ALL = $(PLOT_SRC:.c=.o)
PLOT_OBJ:= $(filter-out main.o,$(PSI_OBJ_ALL))

PSI = psimon
PSI_SRC = $(wildcard *.c)
PSI_OBJ_ALL = $(PSI_SRC:.c=.o)
PSI_OBJ := $(filter-out main.o,$(PSI_OBJ_ALL))


%.o: %.c
	$(CC) -I$(CURDIR)/$(SUBDIRS) $(CC_FLAGS) $(CC_FLAGS) -c $< -o $@


plot: $(PLOT_OBJ_ALL)
	$(CC) $(CC_FLAGS) $(PLOT_OBJ_ALL) -o $(PLOT)

psimon: $(PSI_OBJ)
	$(CC) -I$(CURDIR)/$(SUBDIRS) $(CC_FLAGS) $(PLOT_OBJ) $(PSI_OBJ_ALL) -o $(PSI)

clean:
		-rm -f $(PLOT) $(PLOT_OBJ_ALL) $(SHARED) $(PSI_OBJ_ALL) $(PSI)

check:
		valgrind --leak-check=yes --track-origins=yes ./$(PSI)

gdb:
		gdb $(PSI)

