# Ejecutables y carpetas 
SERVER_TARGET = bin/emulinator3000
CLIENT_TARGET = bin/cliente
CC = gcc
CXX = g++

# Flags de compilación (compartidos)
CFLAGS += -Wall -Wextra -pedantic -Iservidor/include -Iservidor/include/chip8 -Iservidor/include/menu -Iservidor/include/nes -Iservidor/include/database -Iservidor/include/config -Iservidor/include/net
CXXFLAGS += -Wall -Wextra -pedantic -Icliente/include -Icliente/include/menu -Icliente/include/net -Icliente/include/nes -Icliente/include/chip8 -Icliente/include/config

# Detección de sistema operativo
ifeq ($(OS),Windows_NT)
    # Windows
    CFLAGS += -D_WIN32
    CXXFLAGS += -D_WIN32
    SERVER_LDFLAGS = -lmingw32 -lws2_32
    CLIENT_LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lws2_32
else
    # Linux/macOS
    SERVER_LDFLAGS =
    CLIENT_LDFLAGS = -lSDL2 -lSDL2_ttf
endif

# Directorios
SERVER_DIR = servidor/src
BUILD_DIR = build
BIN_DIR = bin
CLIENT_DIR = cliente/src

# Fuentes y objetos del servidor
SERVER_SRC = $(wildcard $(SERVER_DIR)/*.c $(SERVER_DIR)/chip8/*.c $(SERVER_DIR)/menu/*.c $(SERVER_DIR)/nes/*.c $(SERVER_DIR)/database/*.c $(SERVER_DIR)/config/*.c $(SERVER_DIR)/net/*.c)
SERVER_OBJ = $(patsubst $(SERVER_DIR)/%.c, $(BUILD_DIR)/servidor/%.o, $(SERVER_SRC))

# Fuentes y objetos del cliente
CLIENT_SRC = $(wildcard $(CLIENT_DIR)/*.cpp $(CLIENT_DIR)/menu/*.cpp $(CLIENT_DIR)/net/*.cpp $(CLIENT_DIR)/nes/*.cpp $(CLIENT_DIR)/chip8/*.cpp $(CLIENT_DIR)/config/*.cpp)
CLIENT_OBJ = $(patsubst $(CLIENT_DIR)/%.cpp, $(BUILD_DIR)/cliente/%.o, $(CLIENT_SRC))

# Regla por defecto: compilar ambos
all: $(SERVER_TARGET) $(CLIENT_TARGET)

# Compilar servidor
$(SERVER_TARGET): $(SERVER_OBJ) | $(BIN_DIR)
	$(CC) -o $@ $^ $(CFLAGS) $(SERVER_LDFLAGS)

# Compilar cliente
$(CLIENT_TARGET): $(CLIENT_OBJ) | $(BIN_DIR)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(CLIENT_LDFLAGS)

# Compilar objetos servidor
$(BUILD_DIR)/servidor/%.o: $(SERVER_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

# Compilar objetos cliente
$(BUILD_DIR)/cliente/%.o: $(CLIENT_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Crear carpeta bin si no existe
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Limpieza
clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

# Recompilar desde cero
rebuild: clean all

.PHONY: all clean rebuild
