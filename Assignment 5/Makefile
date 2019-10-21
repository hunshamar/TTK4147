#sources here
SOURCE_FILES = main.c io.c

#program executable name
TARGET = prog

#compiler flags
CFLAGS = -g -Wall -std=gnu11 -lpthread -lcomedi -lm

#linker flags
LDFLAGS = -lpthread -lcomedi -lm

#end flags
ENDFLAGS = -lm

#list of deps
DEPS = io.h

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
	gcc $(CFLAGS) -fsanitize=address -std=gnu11 -c -o  $@ $<

#cleaning 
clean:
	rm -f $(TARGET) $(OBJECTS)