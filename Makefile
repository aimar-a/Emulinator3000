# Nombre del ejecutable y carpetas
TARGET = bin/emulinator3000
CC = gcc

# Si estamos en Windows, añadimos las librerías de MinGW y SDL2
ifdef WIN32
	CFLAGS += -lmingw32 -lSDL2main -lSDL2_ttf
# Si estamos en Linux, añadimos las librerías de SDL2
else
	CFLAGS += -lSDL2 -lSDL2_ttf
endif
CFLAGS += -Wall -Wextra -pedantic -Iinclude -Iinclude/chip8 -Iinclude/menu -Iinclude/nes -Iinclude/database -Iinclude/config

# Carpetas
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
BIN_DIR = bin

# Archivos fuente y objetos
SRC = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/chip8/*.c $(SRC_DIR)/menu/*.c $(SRC_DIR)/nes/*.c $(SRC_DIR)/database/*.c $(SRC_DIR)/config/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

# Regla para compilar el ejecutable
$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CC) -o $@ $^ $(CFLAGS)

# Regla para compilar archivos fuente a objetos
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

# Crear carpetas si no existen
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/chip8 $(BUILD_DIR)/menu $(BUILD_DIR)/nes $(BUILD_DIR)/database $(BUILD_DIR)/config

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Limpiar archivos compilados
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Recompilar desde cero
rebuild: clean $(TARGET)

.PHONY: clean rebuild
