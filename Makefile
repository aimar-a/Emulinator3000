# Nombre del ejecutable y carpetas
TARGET = bin/chip8_emulator
CC = gcc
CFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -Wall -Wextra -pedantic -Iinclude -Iinclude/chip8 -Iinclude/menu

# Carpetas
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
BIN_DIR = bin

# Archivos fuente y objetos
SRC = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/chip8/*.c $(SRC_DIR)/menu/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

# Regla para compilar el ejecutable
$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CC) -o $@ $^ $(CFLAGS)

# Regla para compilar archivos fuente a objetos
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

# Crear carpetas si no existen
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/chip8 $(BUILD_DIR)/menu

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Limpiar archivos compilados
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Recompilar desde cero
rebuild: clean $(TARGET)

.PHONY: clean rebuild
