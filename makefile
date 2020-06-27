include compiler.mk
include sources.mk
include includes.mk
include prepare_libs.mk


UNI_TEST 	:= ./uni_test/
OBJECTS 	:= $(SRCS:%.c=$(OBJ)/%.o)
UTEST_NAME 	:= utest
CFLAGS 		:= $(CXXFLAGS) $(addprefix -I,$(INC_DIRS))

ifneq ($(LIB_NAME),)
CFLAGS 		:= -fPIC $(CFLAGS)
endif

.PHONY : all
.PHONY : clean

all : $(OBJECTS) $(PREPARE_LIBS)

$(UTEST_NAME): $(OBJECTS)
	@echo "__________________ Linking __________________"
	@echo "__________________ $@ __________________"
	@echo "$^";$(CC) $(CFLAGS) -o $@ $^

$(SLIB)/lib$(LIB_NAME).a :$(OBJECTS)
	@echo "Build static lib $@";ar cr $@ $^

$(DLIB)/lib$(LIB_NAME).so :$(OBJECTS)
	@echo "Build dynamic lib $@";$(CC) $(CFLAGS) -shared $^ -o $@

$(OBJ)/%.o : $(SRC)/%.$(SUFFIX) $(INC)/%.h
	@echo "Compile $@";$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o : $(SRC)/%.$(SUFFIX)
	@echo "Compile $@";$(CC) $(CFLAGS) -c $< -o $@

run_uni_test: $(UTEST_NAME)
	@echo "run $<";./$<

valgrind: $(UTEST_NAME)
	$@ --leak-check=full --show-leak-kinds=all -v ./$<

clean:
	rm -rf $(OBJ)/*.o
	rm -rf $(SLIB)/*.a
	rm -rf $(DLIB)/*.so

