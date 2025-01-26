WORK_DIR  ?= $(shell pwd)
BUILD_DIR ?= $(WORK_DIR)/build
LIB_BUILD_DIR ?= $(BUILD_DIR)/lib
LIB_LOCAL_INSTALL_DIR ?= $(WORK_DIR)/lib

C_LIB_INCLUDES := ${WORK_DIR}/include
C_LIB_DEFINES := NDEBUG
# complier
LIB_CC = gcc
# C_LIB_FLAGS = -Wall -Wno-unused -ffunction-sections -fdata-sections -rdynamic \
# $(addprefix -I, $(C_LIB_INCLUDES)) \
# $(addprefix -D, $(C_LIB_DEFINES))

C_LIB_TEST_FLAGS = -g -Wall -Wno-unused -ffunction-sections -fdata-sections \
$(addprefix -I, $(C_LIB_INCLUDES)) \

#directories
TEST_SRCS = $(wildcard test/*_test.c)
TEST_OBJS = $(patsubst test/%.c,build/_test/%.o,$(TEST_SRCS))
TESTS_TARGET = $(patsubst %.o,%,$(TEST_OBJS))

LIB_SRCS = $(wildcard src/**/*.c src/*.c)
LIB_OBJS = $(patsubst src/%.c,build/_lib/%.o,$(LIB_SRCS))

TARGET_LIB = $(LIB_BUILD_DIR)/libvtuzki.a
SO_TARGET_LIB = $(patsubst %.a,%.so,$(TARGET_LIB))


# lib_all: $(TARGET_LIB) $(SO_TARGET_LIB)
lib_all: $(TARGET_LIB) lib_test

# lib_all: lib_test
lib_dev: C_LIB_FLAGS = -g -Wall -Wextra $(OPTFLAGS)
lib_dev: lib_all install_local

#------------------LIB_TARGETS------------------
$(TARGET_LIB): C_LIB_FLAGS += -fPIC $(addprefix -I, $(C_LIB_INCLUDES))
$(TARGET_LIB): build_lib_dir $(LIB_OBJS)
	@echo "   Building static library: $@"
	@ar rcsv $@ $(LIB_OBJS)
	@ranlib $@

$(SO_TARGET_LIB): $(TARGET_LIB) $(LIB_OBJS)
	@echo "   Building shared library: $@"
	$(LIB_CC) -shared -o $@ $(LIB_OBJS)

build/_lib/%.o : src/%.c
	@echo "\033[1;32m   CC $@ $<\033[0m"
	@mkdir -p $(dir $@)
	@$(LIB_CC) $(C_LIB_FLAGS) -c -o $@ $<

#------------------TEST_TARGETS------------------
lib_test: C_LIB_TEST_FLAGS += $(addprefix -L, $(BUILD_DIR)/lib) -lvtuzki
lib_test: $(TARGET_LIB) $(TESTS_TARGET)
build/_test/%_test : test/%_test.c
	@echo "\033[1;32m   CC $@ $<\033[0m"
	@mkdir -p $(dir $@)
	@$(LIB_CC) $< $(C_LIB_TEST_FLAGS) -Wl,-Map=$@.map -o $@
	@$@ > $@.log



#------------------INSTALL_TARGETS------------------
install: install_local

install_local:
	@echo "   Installing library to local directory: $(LIB_LOCAL_INSTALL_DIR)"
	@install -d $(LIB_LOCAL_INSTALL_DIR)
	@install $(TARGET_LIB) $(LIB_LOCAL_INSTALL_DIR)

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
.PHONY: tests lib_all lib_dev install check
# 保存中间文件
.SECONDARY: