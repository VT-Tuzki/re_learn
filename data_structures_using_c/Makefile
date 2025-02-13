#----------for all----------------
WORK_DIR = $(shell pwd)
BUILD_DIR = $(WORK_DIR)/build
LIB_BUILD_DIR = $(BUILD_DIR)/lib
LIB_LOCAL_INSTALL_DIR = $(WORK_DIR)/lib
ALL_DIRS_IN_LIB_INCLUDE = $(shell find $(WORK_DIR)/include -type d)

#configure.sh > will chenge this line LANGUAGE_STANDARD
LANGUAGE_STANDARD = c11
#configure.sh  explain: set language standard, such as c90 c99 c11 c17 c23<

#configure.sh > will chenge this line LIB_GOLBAL_INSTALL_DIR
LIB_GOLBAL_INSTALL_DIR = /usr/local/lib
#confiure.sh  explain: set lib install dir,such as /usr/local/lib <
#-----------for lib---------------
C_LIB_CORE_INCLUDES =  $(filter $(WORK_DIR)/include/core%, $(ALL_DIRS_IN_LIB_INCLUDE))
C_LIB_DS_INCLUDES =  $(filter $(WORK_DIR)/include/ds%, $(ALL_DIRS_IN_LIB_INCLUDE))
C_LIB_INCLUDES = ${WORK_DIR}/include $(C_LIB_CORE_INCLUDES) $(C_LIB_DS_INCLUDES)
#configure.sh > will chenge this line C_LIB_NAMES
C_LIB_NAMES = vtuzki
#configure.sh explain: set lib name, such as vtuzki->libvtuzki.a <
C_LIB_INSTALL_DIR = ${WORK_DIR}/lib
TEST_FILE_DIR = $(WORK_DIR)/test
TEST_BUILD_DIR = $(BUILD_DIR)/_test


#-----------for app---------------
APP_FILE_DIR = $(WORK_DIR)/app
APP_BUILD_DIR = $(BUILD_DIR)/_app
C_APP_INCLUDE_LIB_ALL_DIR = $(filter $(WORK_DIR)/include%, $(ALL_DIRS_IN_LIB_INCLUDE))
C_APP_INCLUDES = $(C_APP_INCLUDE_LIB_ALL_DIR) ${WORK_DIR}/app
C_APP_LIB = ${WORK_DIR}/lib

#configure.sh will chenge this line IS_C_APP_USE_LIB
#configure.sh explain: set app use lib or not, such as yes <
#configure.sh explain: set app use lib or not, such as yes <
IS_C_APP_USE_LIB = no
#configure.sh explain: set app use lib or not, such as yes or no <


#complier
include ./scripts/build.mk

#test
include ./scripts/test.mk