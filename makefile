DIRS := text_util

SRC  := $(addprefix /,$(wildcard *.cpp))  $(foreach dir,$(DIRS), $(addprefix /,$(wildcard $(dir)/*.cpp)))          #$(shell find . -name '*.cpp') #$(addprefix /,$(wildcard *.cpp))
HDR := $(wildcard *.h)  $(foreach dir,$(DIRS), $(wildcard $(dir)/*.h))                              #$(addprefix /,$(wildcard *.h)) #$(shell find . -name '*.h')
SRC_NAME := $(subst ./,/,$(SRC))
OBJS  := $(addprefix obj, $(SRC_NAME:.cpp=.o)) 
DEPS := $(addprefix obj, $(SRC_NAME:.cpp=.d)) 
LIB := -lgdiplus -lgdi32
FLAGS := -g -fpermissive -Wextra -MMD -MP -w -DUNICODE -std=c++20 -Wl,--subsystem,windows -static-libgcc -static-libstdc++#-fcompare-debug-second
ARGS := $(FLAGS)
TARGET := Graph
.PHONY: all clear clear_all

all: build/$(TARGET).exe

rebuild: clear all

build/$(TARGET).exe:  $(OBJS)
	@mkdir -p build
	g++ $(ARGS) $(OBJS) -o build/$(TARGET).exe $(LIB)

obj/%.co: %.c
	gcc $(ARGS) $(INCLUDE) -c $< -o $@

obj/%.o:  %.cpp
	@mkdir -p $(dir $@)
	g++ $(ARGS)$(INCLUDE) -c $< -o $@

-include $(DEPS)

clear: 
	rm -f $(OBJS)
	rm -f $(DEPS)

clear_all: clear
	rm -f build/$(TARGET).exe