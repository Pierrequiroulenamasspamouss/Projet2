SRCS = main.c simulation.c grid.c animal.c rabbit.c wolf.c constants.c simulation_gif.c gifenc.c
OBJS = $(SRCS:%.c=%.o)
TARGET = simulate
CC = gcc
CFLAGS = -std=c99 --pedantic -Wall -Wextra -Wmissing-prototypes
LDFLAGS = -lm

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

debug: CFLAGS += -g

debug: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

run:
	./simulate

animal.o: animal.c position.h action.h constants.h grid.h animal.h
constants.o: constants.c constants.h
gifenc.o: gifenc.c gifenc.h
grid.o: grid.c position.h animal.h action.h grid.h
main.o: main.c simulation.h position.h action.h grid.h animal.h \
  simulation_gif.h gifenc.h
rabbit.o: rabbit.c animal.h action.h position.h grid.h constants.h \
  rabbit.h
simulation.o: simulation.c grid.h position.h animal.h action.h rabbit.h \
  wolf.h simulation.h
simulation_gif.o: simulation_gif.c simulation_gif.h simulation.h \
  position.h action.h grid.h animal.h gifenc.h constants.h
wolf.o: wolf.c animal.h action.h position.h grid.h constants.h wolf.h
