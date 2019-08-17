
CC 			:= gcc
INC 		:= ./inc/
SRC 		:= ./src/
OBJ 		:= ./obj/
SLIB 		:= ./static_lib/
INCLUDES 	:= -I$(INC)
CFLAGS 		:= -pedantic -ansi -Werror -Wall $(INCLUDES)
OBJECTS 	:= $(OBJ)hash.o $(OBJ)safe_queue.o $(OBJ)queue.o $(OBJ)vector.o $(OBJ)list.o
LIST_OBJ 	:= $(OBJ)list_itr.o $(OBJ)list_operations.o

.PHONY : all

all : $(OBJECTS) $(SLIB)LDS.a

$(SLIB)LDS.a :$(OBJECTS)
	ar cr $@ $^ $(LIST_OBJ)
	
$(OBJ)%.o : $(SRC)%.c $(INC)%.h
	$(CC) $(CFLAGS) -c $< -o $@ 
		
clean:
	rm -rf $(OBJ)*.o
	rm -rf $(SLIB)*.a

