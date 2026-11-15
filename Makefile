CC = gcc
CFLAGS = -Wall -Wextra -O2

BIN_DIR = bin
SRC_DIR = src

all: $(BIN_DIR)/collector $(BIN_DIR)/service_a $(BIN_DIR)/service_b $(BIN_DIR)/tracelite

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/collector: $(SRC_DIR)/collector.c $(SRC_DIR)/trace_common.h | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/service_a: $(SRC_DIR)/service_a.c $(SRC_DIR)/trace_common.h | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/service_b: $(SRC_DIR)/service_b.c $(SRC_DIR)/trace_common.h | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/tracelite: $(SRC_DIR)/main.c $(SRC_DIR)/trace_common.h | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean
