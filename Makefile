CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Isrc -Iutils -D_DEFAULT_SOURCE
LDFLAGS = -ldl -lm

BUILD_DIR = build
PLUGIN_DIR = plugins
PLUGIN_SRC_DIR = $(PLUGIN_DIR)/src

# Recursive wildcard function to find all .c files in any nested subfolder
rwildcard = $(wildcard $1/$2) $(foreach d,$(wildcard $1/*),$(call rwildcard,$d,$2))

# Automatically gather all core .c files from src/ and utils/
SRCS = $(call rwildcard,src,*.c) $(call rwildcard,utils,*.c)
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS))

# Automatically gather plugin source files and map them to .so files in plugins/
PLUGIN_SRCS = $(call rwildcard,$(PLUGIN_SRC_DIR),*.c)
PLUGIN_SOS = $(patsubst $(PLUGIN_SRC_DIR)/%.c, $(PLUGIN_DIR)/%.so, $(PLUGIN_SRCS))

TARGET = biccuit

# Build both the main executable and all plugins by default
all: $(TARGET) plugins

# Linking step for the main executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS) -rdynamic

# Compilation step for core object files
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Target to build all plugins
plugins: $(PLUGIN_SOS)

# Pattern rule to build a plugin .so from its .c source file
$(PLUGIN_DIR)/%.so: $(PLUGIN_SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -shared -fPIC $< -o $@ $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	# Optional: uncomment the line below if you also want 'make clean' to wipe out built plugins
	# rm -f $(PLUGIN_DIR)/*.so

.PHONY: all plugins clean