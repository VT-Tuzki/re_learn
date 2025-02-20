WORK_DIR  ?= $(shell pwd)
BUILD_DIR ?= $(WORK_DIR)/build
LIB_BUILD_DIR ?= $(BUILD_DIR)/lib
LIB_LOCAL_INSTALL_DIR ?= $(WORK_DIR)/lib

C_LIB_DEFINES = NDEBUG



# complier
LIB_CC = gcc
C_LIB_FLAGS = -Wall -Wextra -Wno-unused -ffunction-sections -fdata-sections \
$(addprefix -I, $(C_LIB_INCLUDES)) \
-std=$(LANGUAGE_STANDARD)

C_LIB_NDEBUG_FLAGS = $(addprefix -D, $(C_LIB_DEFINES))
C_LIB_DEBUG_FLAGS = -g -rdynamic
C_LIB_TEST_STATIC_FLAGS = $(addprefix -L, $(BUILD_DIR)/lib) $(C_APP_LOCAL_STATIC_LIB)
C_LIB_TEST_DYNAMIC_FLAGS = $(addprefix -L, $(BUILD_DIR)/lib) $(C_APP_LOCAL_DYNAMIC_LIB)

#directories
TEST_STATIC_SRCS = $(wildcard test/*_static_test.c)
TEST_DYNAMIC_SRCS = $(wildcard test/*_dynamic_test.c)

TEST_STATIC_OBJS = $(patsubst test/%.c,build/_test/static/%.o,$(TEST_STATIC_SRCS))
TEST_DYNAMIC_OBJS = $(patsubst test/%.c,build/_test/dynamic/%.o,$(TEST_DYNAMIC_SRCS))

TESTS_STATIC_TARGET = $(patsubst %.o,%,$(TEST_STATIC_OBJS))
TESTS_DYNAMIC_TARGET = $(patsubst %.o,%,$(TEST_DYNAMIC_OBJS))
TARGET_TESTS = $(TESTS_STATIC_TARGET) $(TESTS_DYNAMIC_TARGET)

LIB_SRCS = $(wildcard src/**/*.c src/*.c)
LIB_OBJS = $(patsubst src/%.c,build/_lib/%.o,$(LIB_SRCS))

TARGET_STATIC_LIB = $(LIB_BUILD_DIR)/$(C_LIB_STATIC_NAMES)
TARGET_DYNAMIC_LIB = $(LIB_BUILD_DIR)/$(C_LIB_DYNAMIC_NAMES)
TARGET_LIB = $(TARGET_STATIC_LIB) $(TARGET_DYNAMIC_LIB)


lib_all: $(TARGET_LIB) install_local
lib_all: lib_test

lib_dev: clean_lib
lib_dev: C_LIB_FLAGS_DEV = $(C_LIB_DEBUG_FLAGS) $(OPTFLAGS)
lib_dev: lib_all

#------------------LIB_TARGETS------------------
$(TARGET_STATIC_LIB): build_lib_dir $(LIB_OBJS)
	@echo "\033[1;32m   Building static library: $@\033[0m"
	@ar rcsv $@ $(LIB_OBJS)
	@ranlib $@

$(TARGET_DYNAMIC_LIB): build_lib_dir $(LIB_OBJS)
	@echo "\033[1;32m   Building shared library: $@\033[0m"
	@$(LIB_CC) -shared -o $@ $(LIB_OBJS)

build/_lib/%.o : src/%.c
	@echo "\033[1;32m   CC $@ $<\033[0m"
	@mkdir -p $(dir $@)
	@$(LIB_CC) $(C_LIB_FLAGS_DEV) $(C_LIB_FLAGS) $(C_LIB_NDEBUG_FLAGS) -fPIC -c -o $@ $<
	@chmod +x $@

#------------------TEST_TARGETS------------------
lib_test: $(TARGET_LIB) $(TARGET_TESTS)
build/_test/static/%_static_test: test/%_static_test.c
	@echo "\033[1;32m   CC $@ $<\033[0m"
	@mkdir -p $(dir $@)
	@$(LIB_CC) $< $(C_LIB_DEBUG_FLAGS) $(C_LIB_FLAGS) $(C_LIB_TEST_STATIC_FLAGS) -Wl,-Map=$@.map -o $@
	@timeout 20 $(MEMORY_CHECK_PROG) --log-file=$@_leck_check.log $@ > $@.log

build/_test/dynamic/%_dynamic_test: test/%_dynamic_test.c
	@echo "\033[1;32m   CC $@ $<\033[0m"
	@mkdir -p $(dir $@)
	@$(LIB_CC) $< $(C_LIB_DEBUG_FLAGS) $(C_LIB_FLAGS) $(C_LIB_TEST_DYNAMIC_FLAGS) -Wl,-Map=$@.map -o $@
	@$(MEMORY_CHECK_PROG) --log-file=$@_leck_check.log $@ > $@.log
#------------------INSTALL_TARGETS------------------
install: install_local

install_local:
	@echo "\033[1;32m   Installing library directory: $(LIB_LOCAL_INSTALL_DIR) \033[0m"
	@install -d $(LIB_LOCAL_INSTALL_DIR)
	@install $(TARGET_DYNAMIC_LIB) $(LIB_LOCAL_INSTALL_DIR)
	@install $(TARGET_STATIC_LIB) $(LIB_LOCAL_INSTALL_DIR)

build_lib_dir:
	@mkdir -p build
	@mkdir -p lib
	@mkdir -p build/lib


#------------------CHECK_TARGETS------------------
BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'

check:
	@echo "Files with potentially dangerous functions."
	@egrep $(BADFUNCS) $(LIB_SRCS) || true


clean_lib:
	@echo "\033[1;32m lib clean\033[0m"
	@rm -rf $(BUILD_DIR)/_lib
	@rm -rf $(BUILD_DIR)/_test
	@rm -rf $(LIB_BUILD_DIR)
	@rm -rf $(LIB_LOCAL_INSTALL_DIR)
.PHONY: tests lib_all lib_dev install check clean_lib
# 保存中间文件
.SECONDARY: