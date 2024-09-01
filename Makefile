CORE_DIR = Core
MODULES_DIR = Modules
INTEGRAL_IMAGE_DIR = $(MODULES_DIR)/IntegralImage
LBP_FEATURE_DIR = $(MODULES_DIR)/LBPFeature
STAGE_DIR = $(MODULES_DIR)/Stage
BINARY_STAGE_PARSER_DIR = $(MODULES_DIR)/BinaryStageParser
STB_IMAGE_DIR = $(MODULES_DIR)/STBImage
TESTS = Y

C_INCLUDES = \
-I$(CORE_DIR)/Inc \
-I$(INTEGRAL_IMAGE_DIR)/Inc \
-I$(LBP_FEATURE_DIR)/Inc \
-I$(STAGE_DIR)/Inc \
-I$(BINARY_STAGE_PARSER_DIR)/Inc \
-I$(STB_IMAGE_DIR)/Inc

C_SOURCES = \
$(INTEGRAL_IMAGE_DIR)/Src/integral_image.c \
$(LBP_FEATURE_DIR)/Src/lbp_feature.c \
$(STAGE_DIR)/Src/stage.c \
$(BINARY_STAGE_PARSER_DIR)/Src/binary_stage_parser.c

ifeq ($(TESTS), Y)
include MakefileTest.mk
else
include MakefileNonTest.mk                                           
endif