CC = gcc
CC_FLAGS = -g -pedantic -Wall -Wextra -Wstrict-prototypes -Wmissing-prototypes -Wshadow -Wconversion -fPIC -lm
SHARED_FLAGS = -shared
SUBDIRS = deps/plot/src


PLOT = plot
PLOT_OBJ = $(PLOT).o
PLOT_SRC = $(wildcard $(CURDIR)/$(SUBDIRS)/*.c)
PLOT_OBJ = $(PLOT_SRC:.c=.o)

%.o: %.c
	$(CC) $(CC_FLAGS) $(CC_FLAGS) -c $< -o $@

plot: $(PLOT_OBJ)
	$(CC) $(CC_FLAGS) $(PLOT_OBJ) -o $(PLOT)

clean:
		-rm -f $(PLOT) $(PLOT_OBJ) $(SHARED)

check:
		valgrind --leak-check=yes --track-origins=yes ./$(PLOT)

gdb:
		gdb $(PLOT)

