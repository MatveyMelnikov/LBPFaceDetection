CFLAGS = -DTEST -DUNITY_INCLUDE_CONFIG_H
TESTS_DIR = Tests
UNITY_DIR = Modules/Unity-2.5.2
INTEGRAL_IMAGE_TESTS = $(TESTS_DIR)/Modules/IntegralImage
BINARY_STAGE_PARSER_TESTS = $(TESTS_DIR)/Modules/BinaryStageParser

BUILD_DIR = $(UNITY_DIR)/build
TARGET = $(BUILD_DIR)/tests.out

C_INCLUDES += \
-I$(UNITY_DIR)/src \
-I$(UNITY_DIR)/extras/fixture/src \
-I$(UNITY_DIR)/extras/memory/src \
-I$(TESTS_DIR)

C_SOURCES += \
$(UNITY_DIR)/src/unity.c \
$(UNITY_DIR)/extras/fixture/src/unity_fixture.c \
$(UNITY_DIR)/extras/memory/src/unity_memory.c \
$(TESTS_DIR)/tests.c \
$(INTEGRAL_IMAGE_TESTS)/integral_image_test_runner.c \
$(INTEGRAL_IMAGE_TESTS)/integral_image_test.c \
$(BINARY_STAGE_PARSER_TESTS)/binary_stage_parser_test_runner.c \
$(BINARY_STAGE_PARSER_TESTS)/binary_stage_parser_test.c

include MakefileFinal.mk
