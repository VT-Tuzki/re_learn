C_APP_INCLUDES = ${WORK_DIR}/include ${WORK_DIR}/app
C_APP_LIB = ${WORK_DIR}/lib
C_APP_FLAGS = -Wall -Wextra -Wno-unused -ffunction-sections -fdata-sections


C_LIB_INCLUDES = ${WORK_DIR}/include
C_LIB_FLAGS = -Wall -Wextra -Wno-unused -ffunction-sections -fdata-sections

ifeq ($(IS_C_APP_USE_LIB),yes)
MK_TARGET = $(WORK_DIR)/scripts/build_app_with_lib.mk
else
MK_TARGET = $(WORK_DIR)/scripts/build_app_without_lib.mk
endif

include $(MK_TARGET)

print-message:
ifeq ($(IS_C_APP_USE_LIB),yes)
	@echo "\033[1;33mbuild with lib\033[0m"
else
	@echo "\033[1;33mbuild without lib\033[0m"
endif

all: print-message app_all


_clean: clean_app clean_lib
clean:
	@echo "\033[1;32mclean all\033[0m"
	@rm -rf $(BUILD_DIR)
	@rm -rf $(LIB_LOCAL_INSTALL_DIR)

.PHONY: all clean _clean print-message
