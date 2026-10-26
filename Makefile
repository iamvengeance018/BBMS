# --- Compilers and Flags ---
CC       = gcc
CXX      = g++
CFLAGS   = -g -Wall --coverage
CXXFLAGS = -g -Wall --coverage
LDFLAGS  = --coverage -lgtest -lgtest_main -pthread

# --- Source Files ---
# All C files EXCEPT main.c (because main.c has its own main() function)
SRC_C = Blood_Inventory_Management.c \
        Blood_Request_Management.c   \
        Blood_Distribution_Management.c \
        Donor_Management.c           \
        Donation_Management.c        \
        Users.c                      \
        Helper.c                     \
        Reports.c

OBJ       = $(SRC_C:.c=.o)
TEST_SRC  = test_bloodbank.cpp
TEST_BIN  = test_bloodbank
MAIN_BIN  = bloodbank

# --- Make Targets ---
.PHONY: all test coverage cppcheck valgrind run clean

# Default target (just builds and runs tests)
all: test

# 1. Compile the C files for testing
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 2. Link the Test Executable
$(TEST_BIN): $(OBJ) $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $(TEST_SRC) $(OBJ) $(LDFLAGS) -o $(TEST_BIN)

# 3. Run the 34 Tests
test: $(TEST_BIN)
	./$(TEST_BIN)

# 4. Generate Code Coverage with gcov
coverage: test
	gcov $(SRC_C)

# 5. Run Cppcheck Static Analysis
cppcheck:
	cppcheck --enable=warning,performance,portability --std=c11 -I. --suppress=missingIncludeSystem . 2> cppcheck_final.txt
	@echo "--- Cppcheck Results ---"
	@cat cppcheck_final.txt

# 6. Compile the Main App and Run Valgrind
valgrind:
	$(CC) $(CFLAGS) *.c -o $(MAIN_BIN)
	valgrind --leak-check=full ./$(MAIN_BIN)

# 7. Compile and Run the Main App Normally
run:
	$(CC) $(CFLAGS) *.c -o $(MAIN_BIN)
	./$(MAIN_BIN)

# 8. Clean up all generated files
clean:
	rm -f *.o *.gcda *.gcno *.gcov $(TEST_BIN) $(MAIN_BIN) cppcheck_final.txt test_input.txt *.dat
