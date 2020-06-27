LIB_NAME 		:= LDS


SLIB 			:= 
DLIB 			:= 
PREPARE_LIBS 	:=

ifneq ($(LIB_NAME),)
	ifneq ($(shell if [ -d "./static_lib" ]; then echo true;fi),)
		SLIB := ./static_lib
		PREPARE_LIBS += $(SLIB)/lib$(LIB_NAME).a
	endif

	ifneq ($(shell if [ -d "./dynamic_lib" ]; then echo true;fi),)
		DLIB := ./dynamic_lib
		PREPARE_LIBS += $(DLIB)/lib$(LIB_NAME).so
	endif
endif