CC = gcc
CFLAGS = -std=c99 -Iinclude
LDFLAGS = -lintelfpgaup -lpthread

SRC_DIR = src
DRV_DIR = $(SRC_DIR)/drivers
INC_DIR = include
BUILD_DIR = build
TEST_DIR = tests

TRG = $(BUILD_DIR)/main
OBJS = $(BUILD_DIR)/tetris.o $(BUILD_DIR)/vga.o $(BUILD_DIR)/button.o $(BUILD_DIR)/ADXL345.o

.PHONY: all run clean test

all: $(TRG)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TRG): $(OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(BUILD_DIR)/tetris.o: $(SRC_DIR)/tetris.c $(INC_DIR)/vga.h $(INC_DIR)/button.h $(INC_DIR)/ADXL345.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/vga.o: $(DRV_DIR)/vga.c $(INC_DIR)/vga.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/button.o: $(DRV_DIR)/button.c $(INC_DIR)/button.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/ADXL345.o: $(DRV_DIR)/ADXL345.c $(INC_DIR)/ADXL345.h $(INC_DIR)/address_map_arm.h $(INC_DIR)/physical.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_DIR)/testbutton.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/testbutton $<

run: $(TRG)
	./$(TRG)

clean:
	rm -rf $(BUILD_DIR)/*.o $(TRG) $(BUILD_DIR)/testbutton
