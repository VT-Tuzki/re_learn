WORK_DIR  = $(shell pwd)
BUILD_DIR = $(WORK_DIR)/build

#C_INCLUDES := ${WORK_DIR}/include $(C_INCLUDES)

OBJECTS += $(addprefix $(BUILD_DIR)/, $(notdir $(C_SOURCE_FILES:.c=.c.o)))

C_FLAGS += -g -Wall -Wextra -Wno-unused -ffunction-sections -fdata-sections \
-Wp,-MMD,-MT"$@",-MF"$(@:.o=.d)",-MP \
$(addprefix -I, $(C_INCLUDES))

ifeq ($(DEBUG), 1)
C_FLAGS += -g
endif


LD_FLAGS += -Wl,--gc-sections,-Map=$@.map
TARGET ?= target
BUILD_DIR ?= build
CC = cc
AR = ar
.PHONY:clean all

vpath %.c $(sort $(dir $(C_SOURCE_FILES)))
vpath %.c $(WORK_DIR)/test
vpath %.c ./test ./app

all: $(OBJECTS)

$(BUILD_DIR)/%.c.o: %.c | build_dir
	@/bin/echo -e "\tCC $<"
	@$(CC) -c -o $@ $< $(C_FLAGS)

build/%: %.c $(OBJECTS) | build_dir
	@/bin/echo -e "\tCC $<"
	@$(CC) -o $@ $^ $(C_FLAGS) $(LD_FLAGS)
	@/bin/echo -e "\t./$@\n"
	@$(MEMORY_CHECK_PROG) $@
	@rm $@

build_dir:
	@mkdir -p $(BUILD_DIR)
clean:
	@rm -rf $(BUILD_DIR)


-include $(OBJECTS:.o=.d)