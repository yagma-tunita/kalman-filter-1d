CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2

LDFLAGS  := -lgdi32 -mwindows

TARGET   := kalman_plot.exe

SRCS := main.cpp \
        graph.cpp \
        curve.cpp \
        kalman.c \
        noise.cpp

OBJS := $(SRCS:.cpp=.o)
OBJS := $(OBJS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	-del /Q $(OBJS) $(TARGET)

.PHONY: all clean