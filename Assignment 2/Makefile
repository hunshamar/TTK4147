#sources here
SOURCE_FILES = array.c main.c

#program executable name
TARGET = prog

#compiler flags
CFLAGS = -g -Wall -std=gnu11 

#linker flags
LDFLAGS = -g 

#end flags
ENDFLAGS = -lm -lasan

#list of deps
DEPS = array.h

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
%.o : %.c $(DEPS)
	gcc $(CFLAGS) -fsanitize=address -c -o  $@ $<

#cleaning 
clean:
	rm -f $(TARGET) $(OBJECTS)