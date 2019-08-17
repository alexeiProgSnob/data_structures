ARCH := ${ARGS}
 
ifeq ($(ARCH), )
ARCH := $(shell getconf LONG_BIT)
endif

C_FLAGS_32 := -m32
C_FLAGS_64 := -m64

CC 			:= gcc
INC 		:= ./inc/
SRC 		:= ./src/
OBJ 		:= ./obj/$(ARCH)bit/
SLIB 		:= ./static_lib/$(ARCH)bit/
INCLUDES 	:= -I$(INC)
CFLAGS 		:= $(C_FLAGS_$(ARCH)) -pedantic -ansi -Werror -Wall $(INCLUDES)

OBJECTS 	:= $(OBJ)log4c.o 
OBJECTS 	+= $(OBJ)hash.o 
OBJECTS		+= $(OBJ)safe_queue.o
OBJECTS		+= $(OBJ)queue.o
OBJECTS		+= $(OBJ)vector.o
OBJECTS		+= $(OBJ)list.o
OBJECTS 	+= $(OBJ)list_itr.o
OBJECTS		+= $(OBJ)list_operations.o

.PHONY : all

all : $(OBJECTS) $(SLIB)LDS.a

$(SLIB)LDS.a :$(OBJECTS)
	ar cr $@ $^
	
$(OBJ)%.o : $(SRC)%.c $(INC)%.h
	$(CC) $(CFLAGS) -c $< -o $@ 

run:
		
clean:
	rm -rf $(OBJ)*.o
	rm -rf $(SLIB)*.a

