# complier
APP_CC = gcc


C_APP_FLAGS += $(addprefix -I, $(C_APP_INCLUDES)) \
$(addprefix -L, $(C_APP_LIB)) \
$(addprefix -l, $(C_LIB_NAMES))

APP_DEVFLAGS = -g

#directories
APP_SRCS = $(wildcard app/**/*.c app/*.c)
APP_OBJS = $(patsubst app/%.c,build/_app/%.o,$(APP_SRCS))
APP_TARGET = $(patsubst %.o,%,$(APP_OBJS))

app_all: clean_app build_app_dir lib_all install_local
app_all: $(APP_TARGET)

app_dev: C_APP_FLAGS += $(APP_DEVFLAGS)
app_dev: app_all

build/_app/% : app/%.c
	@echo "\033[1;32m CC $@ $<\033[0m"
	@mkdir -p $(dir $@)
	@$(APP_CC) $< $(C_APP_FLAGS) -Wl,-Map=$@.map -o $@
	@echo "\033[1;32mfile: $@    map: $@.map \033[0m"
build_app_dir:
	@mkdir -p build
	@mkdir -p build/_app

clean_app:
	@echo "\033[1;32m with lib app clean\033[0m"
	@-rm -rf build/_app

.PHONY: clean_app build_app_dir app_dev app_all
# 保存中间文件
.SECONDARY:

include $(WORK_DIR)/scripts/build_lib.mk