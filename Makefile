PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

CXX = gcc

OBJS = project970.o

CPPFLAGS := ${CPPFLAGS} $(shell pkg-config --cflags --libs gtk+-3.0)
CPPFLAGS := ${CPPFLAGS} 

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
else ifeq ($(BUILD_MODE),run)
	CFLAGS += -O2
else
	$(error Build mode $(BUILD_MODE) not supported by this Makefile)
endif

all:	project970

project970:	$(OBJS)
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^

%.o:	$(PROJECT_ROOT)%.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

%.o:	$(PROJECT_ROOT)%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	rm -fr project970 $(OBJS)
