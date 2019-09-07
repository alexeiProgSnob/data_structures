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
CFLAGS 		:= $(C_FLAGS_$(ARCH)) -fPIC -pedantic -ansi -Werror -Wall $(INCLUDES) -pthread

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

all : $(OBJECTS) $(SLIB)LDS_$(ARCH)bit.a $(DLIB)LDS_$(ARCH)bit.so

utest: $(OBJECTS) $(OBJ)uni_test.o
	@echo "__________________ Linking __________________"
	@echo "__________________ $@ __________________"
	@echo "$^";$(CC) $(CFLAGS) -o $@ $^

$(SLIB)LDS_$(ARCH)bit.a :$(OBJECTS)
	@echo "Build static lib $@";ar cr $@ $^

$(DLIB)LDS_$(ARCH)bit.so :$(OBJECTS)
	$(CC) $(CFLAGS) -shared $^ -o $@

$(OBJ)%.o : $(SRC)%.c $(INC)%.h
	@echo "Compile $@";$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)%.o : $(SRC)%.c
	@echo "Compile $@";$(CC) $(CFLAGS) -c $< -o $@

run_uni_test: utest
	./utest

clean:
	rm -rf $(OBJ)*.o
	rm -rf $(SLIB)*.a
	rm -rf $(DLIB)*.so

