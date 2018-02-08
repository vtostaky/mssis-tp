#################################################
##
CC 				= 	gcc
CFLAGS 			= 	-O3 -Wall
LDFLAGS 		= 	-L$(LIB_DIR) -lmbedtls -lmbedcrypto -lmbedx509
INCLUDE 		= 	-I$(INC_DIR)
DEFINE 			= 	#-DDEBUG

#################################################
##
INC_DIR 		= 	include
OBJ_DIR 		= 	obj
BIN_DIR 		= 	bin
LIB_DIR 		= 	lib
SRC_DIR 		= 	src

#################################################
##
SRC 			= 	$(wildcard $(SRC_DIR)/*.c)
OBJ 			= 	$(SRC:.c=.o)
OBJ 			:= 	$(notdir $(OBJ))
## 
EXE 			= 	deriv_passwd

#################################################
##
vpath %.c 	$(SRC_DIR)
vpath %.o 	$(OBJ_DIR)
vpath %.h 	$(INC_DIR)
vpath %.a 	$(LIB_DIR)

#################################################
##
.PHONY: all clean

all: $(OBJ)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(BIN_DIR)

#################################################
##
$(OBJ): OUT_DIR = -o $(OBJ_DIR)/$@
%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@echo " CC $<"
	@$(CC) $(CFLAGS) -c $< $(OUT_DIR) $(INCLUDE) $(DEFINE) 

#################################################
##
$(EXE): OUT_DIR = -o $(BIN_DIR)/$@
$(EXE): OBJ:= $(addprefix $(OBJ_DIR)/, $(OBJ))
$(EXE): $(OBJ)
	@mkdir -p $(BIN_DIR)
	@echo " LN $@"
	@$(CC) $(CFLAGS) $(OUT_DIR) $(OBJ) $(INCLUDE) $(DEFINE) $(LDFLAGS)

