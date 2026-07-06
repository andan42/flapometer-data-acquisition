#TODO: add -Werror to CXXFLAGS
#TODO: make separate build targets for debug and release in order to allow for -O3 and -g to be used at the same time in the right way
TARGET_EXEC ?= a.out
TEST_TARGET_EXEC ?= test.out

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src
TEST_DIR ?= ./test
TEST_BUILD_DIR ?= ./test/build

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.s) #:= means define it now and dont change it later (lazy evaluation)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
OBJS_NO_MAIN := $(filter-out $(BUILD_DIR)/$(SRC_DIRS)/main.cpp.o, $(OBJS))

TEST_SRCS := $(shell find $(TEST_DIR) -name *.cpp)
TEST_OBJS := $(TEST_SRCS:%=$(TEST_BUILD_DIR)/%.o)

DEPS := $(OBJS:.o=.d) $(TEST_OBJS:.o=.d)

#CXX := clang++ #g++ #clang++ clang has cleaner looking errors and warnings
CXX := g++

#AS := nasm
LDFLAGS := -L/usr/local/lib -lm
LDLIBS := -li2c -lcurl -lpthread -ljsoncpp #-lssl -lcrypto#-lncurses -lmenu -lform -lpanel -lutil -lstdc++fs -lboost_system -lboost_filesystem -lboost_thread -lboost_chrono -lboost_date_time -lboost_regex -lboost_atomic -lboost_serialization -lboost_program_options -lboost_iostreams -lboost_locale -lboost_log -lboost_log_setup -lboost_timer -lboost_graph -lboost_coroutine -lboost_context -lboost_contract -lboost_exception -lboost_fiber -lboost_math_c99 -lboost_math_c99f -lboost_math_c99l -lboost_math_tr1 -lboost_math_tr1f -lboost_math_tr1l -lboost_prg_exec_monitor -lboost_random -lboost_random -lboost_regex -lboost_serialization -lboost_signals -lboost_system -lboost_test_exec_monitor -lboost_thread -lboost_timer -lboost_unit_test_framework -lboost_wave -lboost_wserialization -lboost_chrono -lboost_date_time -lboost_atomic -lboost_filesystem -lboost_thread -lboost_system -lboost_regex -lboost_serialization -lboost_program_options -lboost_iostreams -lboost_locale -lboost_log -lboost_log_setup -lboost_timer -lboost_graph -lboost_coroutine -lboost_context -lboost_contract -lboost_exception -lboost_fiber -lboost_math_c99 -lboost_math_c99f -lboost_math_c99l -lboost_math_tr1 -lboost_math_tr1f -lboost_math_tr1l -lboost_prg_exec_monitor -lboost_random -lboost_random -lboost_regex -lboost_serialization -lboost_signals -lboost_system -lboost_test_exec_monitor -lboost_thread -lboost_timer -lboost_unit_test_framework -lboost_wave -lboost_wserialization -lboost_chrono -lboost_date_time -lboost_atomic -lboost_filesystem -lboost_thread -lboost_system -lboost_regex -lboost_serialization -lboost_program_options -lboost_iostreams -lboost_locale -lboost_log -lboost_log_setup -lboost_timer -lboost_graph -lboost_coroutine -lboost_context -lboost_contract -lboost_exception
LDLIBS_TEST := -lgtest -lgtest_main #-lgmock -lgmock_main

INC_DIRS := $(shell find $(SRC_DIRS) -type d) 
INC_FLAGS := $(addprefix -I,$(INC_DIRS)) -I/usr/include/openssl -I/usr/include

CPPFLAGS := $(INC_FLAGS) -MMD -MP -std=c++14 -O0 -g #	O3 and Ofast are faster 
#														-g for debugging. consider -Og or -O0 for debugging. -ggdb3 or similar for gdb debugging, if not using llvm
#dont forget the -g if needed
#-g compiler flag is for debug only. remove it lol


#ASFLAGS := -felf64 -g -F dwarf

#all targets
all: $(BUILD_DIR)/$(TARGET_EXEC)
#all: build/a.out

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
#build/a.out: main.cpp.o somefile.cpp.o someotherfile.cpp.o
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS) 
#	g++ main.cpp.o somefile.cpp.o someotherfile.cpp.o -o build/a.out -L/usr/local/lib -li2c etc.

# assembly
$(BUILD_DIR)/%.s.o: %.s
#build/somefile.s.o: somefile.s
	$(MKDIR_P) $(dir $@)
#	mkdir -p build/somefile.s.o
	$(AS) $(ASFLAGS) -c $< -o $@
#	nasm -felf64 -g -F dwarf somefile.s -o build/somefile.s.o

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
#build/somefile.cpp.o: somefile.cpp
	$(MKDIR_P) $(dir $@)
#	mkdir -p build/somefile.cpp.o
	$(CXX) $(CPPFLAGS) -c $< -o $@
#	g++ -Isrc -MMD -MP -std=c++14 -O0 -g -c somefile.cpp -o build/somefile.cpp.o

# compile test targe and run ./test/build/test.out
runTest: $(TEST_BUILD_DIR)/$(TEST_TARGET_EXEC)
	./$(TEST_BUILD_DIR)/$(TEST_TARGET_EXEC)

# compile test target
test: $(TEST_BUILD_DIR)/$(TEST_TARGET_EXEC)

$(TEST_BUILD_DIR)/$(TEST_TARGET_EXEC): $(TEST_OBJS) $(OBJS_NO_MAIN)
	$(CXX) $(OBJS_NO_MAIN) $(TEST_OBJS) -o $@ $(LDFLAGS) $(LDLIBS_TEST) $(LDLIBS)
#	g++ build/somefile.cpp.o build/somefiletest.cpp.o -o build/somefiletest.out -L/usr/local/lib -lgtest -lgtest_main -li2c etc.

# compile test source
$(TEST_BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@



test: 
.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) -r $(TEST_BUILD_DIR)

-include $(DEPS)
MKDIR_P ?= mkdir -p