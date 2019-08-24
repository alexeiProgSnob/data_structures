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
DLIB 		:= ./dynamic_lib/$(ARCH)bit/
UNI_TEST	:= ./uni_test/
INCLUDES 	:= -I$(INC)
CFLAGS 		:= $(C_FLAGS_$(ARCH)) -pedantic -ansi -Werror -Wall $(INCLUDES) -pthread

OBJECTS 	:= $(OBJ)log4c.o 
OBJECTS 	+= $(OBJ)hash.o 
OBJECTS		+= $(OBJ)queue.o
OBJECTS		+= $(OBJ)safe_queue.o
OBJECTS		+= $(OBJ)vector.o
OBJECTS		+= $(OBJ)list.o
OBJECTS 	+= $(OBJ)list_itr.o
OBJECTS		+= $(OBJ)list_operations.o
OBJECTS		+= $(OBJ)sorts.o

.PHONY : all

all : $(OBJECTS) $(SLIB)LDS_$(ARCH)bit.a

test: $(OBJECTS) $(OBJ)uni_test.o
	$(CC) $(CFLAGS) -o $@ $^

$(SLIB)LDS_$(ARCH)bit.a :$(OBJECTS)
	ar cr $@ $^

$(DLIB)LDS_$(ARCH)bit.so :$(OBJECTS)
	$(CC) -fPIC -shared $@ $^

$(OBJ)%.o : $(SRC)%.c $(INC)%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)%.o : $(SRC)%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)*.o
	rm -rf $(SLIB)*.a

