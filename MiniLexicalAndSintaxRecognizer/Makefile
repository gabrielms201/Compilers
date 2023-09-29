PROGRAM_NAME = MiniLexicalAndSintaxRecognizer
SRC_DIR = src
OBJ_DIR = bin

output: $(OBJ_DIR)/$(PROGRAM_NAME)
	@echo "Finished Compiling"

$(OBJ_DIR)/$(PROGRAM_NAME): $(OBJ_DIR)/$(PROGRAM_NAME).o
	gcc $(OBJ_DIR)/$(PROGRAM_NAME).o -o $(OBJ_DIR)/$(PROGRAM_NAME) -lc

$(OBJ_DIR)/$(PROGRAM_NAME).o: $(SRC_DIR)/$(PROGRAM_NAME).c
	gcc -c $(SRC_DIR)/$(PROGRAM_NAME).c -o $(OBJ_DIR)/$(PROGRAM_NAME).o -lc

clean: 
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/$(PROGRAM_NAME)
