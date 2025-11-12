CC = gcc
CFLAGS = -Wall -Wextra -pthread -Iinc
AR = ar rcs

SRC = tpool.c dynamic_queue/d_queue.c core/thread_queue.c core/worker_queue.c
OBJ = $(SRC:.c=.o)
LIB = libtpool.a

all: $(LIB)

$(LIB): $(OBJ)
	$(AR) $(LIB) $(OBJ)
	@$(MAKE) clean

build: $(OBJ)
	@echo "object files built"

clean:
	@echo "removing object files"
	@rm -rf $(OBJ)

clean-all: clean
	@echo "removing library"
	@rm -rf $(LIB)

.PHONY: all clean clean-all build
