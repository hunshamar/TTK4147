SOURCE_FILES = main.c copyable-code.c

#program executable name
TARGET = time

#compiler flags
CFLAGS = -g -Wall -std=gnu11 

#linker flags
LDFLAGS = -g 

#end flags
ENDFLAGS = -lm

#list of sources
#SOURCES = $( "*.c")

#default rule, to compile everything
all: $(TARGET)

#define object files
OBJECTS = $(SOURCE_FILES:.c=.o)

#link programs
$(TARGET): $(OBJECTS)
	gcc $(LDFLAGS) -o $@ $^ $(ENDFLAGS)

#compile 
%.o : %.c
	gcc $(CFLAGS) -c -o $@ $<

#cleaning 
clean:
	rm -f $(TARGET) $(OBJECTS)