# Copyright (C) 2024  KA Wright

# Makefile - Build script

BUILD_DIR		:=	.build
OBJ_DIR			:=	$(BUILD_DIR)/obj
BIN_DIR			:=	$(BUILD_DIR)/bin
ASSET_DIR		:=	asset
SRC_DIR			:=	src
INSTALL_DIR		:=	/usr/local/bin

OBJS			:=	$(OBJ_DIR)/argv.o		\
					$(OBJ_DIR)/asset.o		\
					$(OBJ_DIR)/clean.o		\
					$(OBJ_DIR)/clock.o		\
					$(OBJ_DIR)/cpu.o		\
					$(OBJ_DIR)/err.o		\
					$(OBJ_DIR)/graphic.o	\
					$(OBJ_DIR)/idle.o		\
					$(OBJ_DIR)/key.o		\
					$(OBJ_DIR)/ram.o		\
					$(OBJ_DIR)/sound.o		\
					$(OBJ_DIR)/savest.o		\
					$(OBJ_DIR)/about.txt.o	\
					$(OBJ_DIR)/help.txt.o

GCC_FLAGS		:=	-Isrc/include			\
					-Wall					\
					-g

LIB_FLAGS		:=	-lSDL2					\
					-lasound				\
					-lm

$(BIN_DIR)/k8e: $(SRC_DIR)/main.c $(OBJS)
	@echo 'BUILDING BINARY      [$@]'
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	@gcc $(GCC_FLAGS) $(OBJS) $(SRC_DIR)/main.c $(LIB_FLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo 'BUILDING OBJECT      [$@]'
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(OBJ_DIR)
	@gcc $(GCC_FLAGS) -c $< $(LIB_FLAGS) -o $@

$(OBJ_DIR)/%.o: $(ASSET_DIR)/%
	@echo 'LINKING ASSET        [$@]'
	@ld -r -b binary -o $@ $<

.PHONY: install
install: $(BIN_DIR)/k8e
	@cp $< $(INSTALL_DIR)/k8e
	@echo DONE!

.PHONY: uninstall
uninstall:
	@rm -f $(INSTALL_DIR)/k8e

.PHONY: all
all: $(BIN_DIR)/k8e
	@echo DONE!

.PHONY: clean
clean:
	@echo CLEANING BUILD ARTIFACTS
	@rm -rf $(BUILD_DIR)
	@echo DONE!
