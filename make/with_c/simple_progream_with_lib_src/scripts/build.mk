WORK_DIR  = $(shell pwd)
BUILD_DIR = $(WORK_DIR)/build

#C_INCLUDES := ${WORK_DIR}/include $(C_INCLUDES)

OBJECTS += $(addprefix $(BUILD_DIR)/, $(notdir $(C_SOURCE_FILES:.c=.c.o)))

C_FLAGS += -g -Wall -Wextra -Wno-unused -ffunction-sections -fdata-sections \
-Wp,-MMD,-MT"$@",-MF"$(@:.o=.d)",-MP \
$(addprefix -I, $(C_INCLUDES))

LD_FLAGS += -Wl,--gc-sections,-Map=$@.map



TARGET ?= target
BUILD_DIR ?= build
CC = cc
AR = ar

ifeq ($(MEMCHECK), 1)
MEMORY_CHECK_PROG = valgrind --leak-check=full --error-exitcode=1
else ifeq ($(MEMCHECK), 2)
MEMORY_CHECK_PROG = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1
endif

ifeq ($(GDBTEST), 1)
GDB_TEST_PROG = /bin/gdb
endif

.PHONY:clean all

vpath %.c $(sort $(dir $(C_SOURCE_FILES)))
vpath %.c $(WORK_DIR)/test
vpath %.c ./test ./test/gdb
vpath %.c ./app
all: $(OBJECTS)

$(BUILD_DIR)/%.c.o: %.c | build_dir
	@/bin/echo -e "\tCC $<"
	@$(CC) -c -o $@ $< $(C_FLAGS)

build/%: %.c $(OBJECTS) | build_dir
	@/bin/echo -e "\tCC $<"
	$(CC) -o $@ $^ $(C_FLAGS) $(LD_FLAGS)
	$(MEMORY_CHECK_PROG) $@
	@/bin/echo -e "\033[34m $@ \033[0m"
	@$(GDB_TEST_PROG) ./$@

build_dir:
	@mkdir -p $(BUILD_DIR)
clean:
	@rm -rf $(BUILD_DIR)


-include $(OBJECTS:.o=.d)