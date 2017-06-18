CC := arm-xilinx-linux-gnueabi-gcc
CC_FLAGS := -I include -w
CC_FILES := $(wildcard src/*.c)
OBJ_FILES := $(addprefix obj/,$(notdir $(CC_FILES:.c=.o)))

all: main

main: $(OBJ_FILES)
	$(CC) $(CC_FLAGS) -o $@ $^
    
obj/%.o: src/%.c
	$(CC) $(CC_FLAGS) -c -o $@ $<
    
clean:
	rm  -rf main obj/*.o *.log

.PHONY: all main clean