WORK_DIR  ?= $(shell pwd)
BUILD_DIR ?= $(WORK_DIR)/build


#Memory leaks
ifeq ($(MEMCHECK),1)
MEMORY_CHECK_PROG = valgrind --leak-check=full --error-exitcode=1
else ifeq ($(MEMCHECK),2)
MEMORY_CHECK_PROG = valgrind --leak-check=full --error-exitcode=1 --track-origins=yes
endif

mem_mkdir:
	@mkdir -p build/valgrind


mem_/build/%: mem_mkdir
# @echo "Running $(@:mem_/%=%) with memory check"
	@$(MEMORY_CHECK_PROG) build/$(@:mem_/build/%=%) > build/valgrind/$(@:mem_/build/bin/%=%).log  2>&1

gdb_/build/%:
# @echo "Running $(@:gdb_/%=%) with gdb"
	@gdb build/$(@:gdb_/build/%=%)


.PHONY: mem_/% gdb_/%

