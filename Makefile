PROJECT      := libcrypto
VERSION      := 1.1.1
LIB_NAME     := $(PROJECT).a
SHARED_LIB   := $(PROJECT).so.$(VERSION)

# Toolchain
CC           := gcc
AR           := ar
CFLAGS       := -std=c11 -Wall -Wextra -pedantic \
                -fstack-protector-strong -D_FORTIFY_SOURCE=2 \
                -fPIC -O2 -Iinclude/$(PROJECT)
LDFLAGS      := -Wl,-z,now,-z,relro
ARFLAGS      := rcs

# Platform-Specific Optimizations
UNAME_S      := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    CFLAGS   += -D_GNU_SOURCE
    LDFLAGS  += -ldl
endif

# Hardware Acceleration
ifeq ($(shell uname -m),x86_64)
    CFLAGS   += -maes -msse4.1 -mpclmul
endif

# Directory Structure
SRC_DIR      := src
BUILD_DIR    := build
TEST_DIR     := tests
INCLUDE_DIR  := include/$(PROJECT)

# Source Files
SRCS         := $(shell find $(SRC_DIR) -name '*.c')
OBJS         := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS         := $(OBJS:.o=.d)

# Main Targets
.PHONY: all static shared tests clean fuzz

all: static shared

static: $(BUILD_DIR)/$(LIB_NAME)

shared: $(BUILD_DIR)/$(SHARED_LIB)

# Static Library
$(BUILD_DIR)/$(LIB_NAME): $(OBJS)
	@mkdir -p $(@D)
	$(AR) $(ARFLAGS) $@ $^
	ranlib $@

# Shared Library
$(BUILD_DIR)/$(SHARED_LIB): $(OBJS)
	@mkdir -p $(@D)
	$(CC) -shared $(LDFLAGS) $^ -o $@
	ln -sf $(SHARED_LIB) $(BUILD_DIR)/$(PROJECT).so

# Pattern Rules
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Include Dependencies
-include $(DEPS)

# Clean
clean:
	rm -rf $(BUILD_DIR)

# Install (System-Wide)
install: shared
	@echo "Installing to /usr/local (requires sudo)"
	sudo mkdir -p /usr/local/lib
	sudo install -m755 $(BUILD_DIR)/$(SHARED_LIB) /usr/local/lib/$(SHARED_LIB)
	sudo ln -sf $(SHARED_LIB) /usr/local/lib/$(PROJECT).so
	sudo mkdir -p /usr/local/$(INCLUDE_DIR)
	sudo cp -r $(INCLUDE_DIR)/* /usr/local/include/libcrypto/
	sudo ldconfig

# Help
help:
	@echo "Build targets:"
	@echo "  all       - Build static and shared libraries (default)"
	@echo "  static    - Build static library ($(LIB_NAME))"
	@echo "  shared    - Build shared library ($(SHARED_LIB))"
	@echo "  install   - Install system-wide"
	@echo "  clean     - Remove build artifacts"
