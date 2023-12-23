SRC = src
INCLUDE = include
BIN = bin
PROJ_NAME = ht_driver
UNAME_M := $(shell uname -m)
ifeq ($(UNAME_M),x86_64)
    CFLAGS = -Wall -Wextra -Werror -I./include
endif
ifeq ($(UNAME_M),arm64)
    CFLAGS = -Wall -Wextra -I./include -arch arm64
endif
DEPS = $(wildcard $(INCLUDE)/*.h)
SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst %.c, %.o, $(SRCS))

$(SRC)/%.o: $(SRC)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: setup program

setup:
	@echo "[i] Setting up project..."
	@mkdir -p $(BIN)
	@mkdir -p $(INCLUDE)
	@echo "[i] Setup complete"

program: clean $(OBJS)
	@echo "[i] Compiling program..."
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN)/$(PROJ_NAME)
	@echo "[i] Compilation complete"

clean: setup
	@echo "[i] Cleaning up..."
	@$(RM) -rf **/*.pyc
	@$(RM) -rf **/__pycache__
	@$(RM) -rf $(BIN)/*
	@$(RM) -rf $(DOCS)/latex
	@$(RM) -rf $(DOCS)/docs
	@$(RM) -rf $(OBJS)
	@$(RM) -rf $(TEST_OBJS)
	@echo "[i] Cleanup complete"