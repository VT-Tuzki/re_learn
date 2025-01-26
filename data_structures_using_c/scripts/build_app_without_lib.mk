# complier
APP_CC = gcc


C_APP_FLAGS += $(addprefix -I, $(C_APP_INCLUDES)) \

APP_DEVFLAGS = -g

#directories
APP_SRCS = $(wildcard app/**/*.c app/*.c)
APP_OBJS = $(patsubst app/%.c,build/_app_without_lib/%.o,$(APP_SRCS))
APP_TARGET = $(patsubst %.o,%,$(APP_OBJS))

APP_LIB_SRCS = $(wildcard src/**/*.c src/*.c)
APP_LIB_OBJS = $(patsubst src/%.c,build/_app_without_lib_src/%.o,$(APP_LIB_SRCS))

app_all: clean_app build_app_dir
app_all: $(APP_LIB_OBJS) $(APP_TARGET)

app_dev: C_APP_FLAGS += $(APP_DEVFLAGS)
app_dev: app_all

build/_app_without_lib_src/%.o: src/%.c
	@echo "\033[1;32m CC $@ $<\033[0m"
	@mkdir -p $(dir $@)
	@$(APP_CC) $(C_APP_FLAGS) -c -o $@ $<

build/_app_without_lib/%:  app/%.c
	@echo "\033[1;32m CC $@ $<\033[0m"
	@mkdir -p $(dir $@)
	@$(APP_CC) $(C_APP_FLAGS) -Wl,-Map=$@.map $(APP_LIB_OBJS) $< -o $@
	@echo "\033[1;32mfile: $@    map: $@.map \033[0m"
build_app_dir:
	@mkdir -p build
	@mkdir -p build/_app_without_lib
	@mkdir -p build/_app_without_lib_src

clean_app:
	@echo "\033[1;32m without lib app clean\033[0m"
	@rm -rf build/_app_without_lib
	@rm -rf build/_app_without_lib_src

.PHONY: clean_app build_app_dir app_dev app_all
# 保存中间文件
.SECONDARY:

include $(WORK_DIR)/scripts/build_lib.mk