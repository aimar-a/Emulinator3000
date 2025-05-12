# Ejecutables y carpetas
SERVER_TARGET = bin/emulinator3000
CLIENT_TARGET = bin/cliente
CC = gcc
CXX = g++

# Detectar SO para SDL y otras libs
ifdef WIN32
	CFLAGS += -lmingw32 -lSDL2main -lSDL2_ttf
	CXXFLAGS += -lmingw32 -lSDL2main -lSDL2_ttf
else
	CFLAGS += -lSDL2 -lSDL2_ttf
	CXXFLAGS += -lSDL2 -lSDL2_ttf
endif

CFLAGS += -Wall -Wextra -pedantic -Iinclude -Iinclude/chip8 -Iinclude/menu -Iinclude/nes -Iinclude/database -Iinclude/config
CXXFLAGS += -Wall -Wextra -pedantic -Icliente/include -Icliente/include/menu

# Directorios
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
CLIENT_DIR = cliente/src

# Fuentes y objetos del servidor
SERVER_SRC = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/chip8/*.c $(SRC_DIR)/menu/*.c $(SRC_DIR)/nes/*.c $(SRC_DIR)/database/*.c $(SRC_DIR)/config/*.c)
SERVER_OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SERVER_SRC))

# Fuentes y objetos del cliente
CLIENT_SRC = $(wildcard $(CLIENT_DIR)/*.cpp $(CLIENT_DIR)/menu/*.cpp)
CLIENT_OBJ = $(patsubst $(CLIENT_DIR)/%.cpp, $(BUILD_DIR)/cliente/%.o, $(CLIENT_SRC))

# Regla por defecto: compilar ambos
all: $(SERVER_TARGET) $(CLIENT_TARGET)

# Compilar servidor
$(SERVER_TARGET): $(SERVER_OBJ) | $(BIN_DIR)
	$(CC) -o $@ $^ $(CFLAGS)

# Compilar cliente
$(CLIENT_TARGET): $(CLIENT_OBJ) | $(BIN_DIR)
	$(CXX) -o $@ $^ $(CXXFLAGS)

# Compilar objetos servidor (crea carpeta automáticamente)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

# Compilar objetos cliente (crea carpeta automáticamente)
$(BUILD_DIR)/cliente/%.o: $(CLIENT_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Crear carpeta bin si no existe
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Limpieza
clean:
	rm -rf $(BUILD_DIR) $(SERVER_TARGET) $(CLIENT_TARGET)

# Recompilar desde cero
rebuild: clean all

.PHONY: clean rebuild all
