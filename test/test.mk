TEST_SRCS := $(wildcard test/*.c)
TEST_SRCS := $(filter-out test/common.c,$(TEST_SRCS))

TESTS := $(basename $(notdir $(TEST_SRCS)))

define MAKE_TEST
test_$(1): | $(BUILD_DIR)
	$(CC) -o $(BUILD_DIR)/$(PRJ_NAME)_$(1)_test \
		$(SRC) \
		test/common.c \
		test/$(1).c \
		$(INC) \
		$(FLAGS)
	clear
	./$(BUILD_DIR)/$(PRJ_NAME)_$(1)_test

build_$(1): | $(BUILD_DIR)
	$(CC) -o $(BUILD_DIR)/$(PRJ_NAME)_$(1)_test \
		$(SRC) \
		test/common.c \
		test/$(1).c \
		$(INC) \
		$(FLAGS)

debug_$(1): build_$(1)
	gdb ./$(BUILD_DIR)/$(PRJ_NAME)_$(1)_test
endef

$(foreach test,$(TESTS),$(eval $(call MAKE_TEST,$(test))))

TEST ?= field

debug: debug_$(TEST)

.PHONY: $(addprefix test_,$(TESTS)) $(addprefix build_,$(TESTS)) $(addprefix debug_,$(TESTS)) debug