CC       = gcc
CXX      = g++
CFLAGS   = -g -Wall --coverage
CXXFLAGS = -g -Wall --coverage
LDFLAGS  = --coverage -lgtest -lgtest_main -pthread

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

.PHONY: all test coverage cppcheck valgrind run clean audit

all: test

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_BIN): $(OBJ) $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $(TEST_SRC) $(OBJ) $(LDFLAGS) -o $(TEST_BIN)

test: $(TEST_BIN)
	./$(TEST_BIN)

coverage: test
	gcov $(SRC_C)

cppcheck:
	cppcheck --enable=warning,performance,portability --std=c11 -I. --suppress=missingIncludeSystem --suppress=incompatibleFileOpen --suppress=ignoredReturnValue . 2> cppcheck_final.txt
	@echo "--- Cppcheck Results ---"
	@cat cppcheck_final.txt

valgrind:
	$(CC) $(CFLAGS) -pthread *.c -o $(MAIN_BIN)
	valgrind --leak-check=full ./$(MAIN_BIN)

run:
	$(CC) $(CFLAGS) -pthread *.c -o $(MAIN_BIN)
	./$(MAIN_BIN)

clean:
	rm -f *.o *.gcda *.gcno *.gcov $(TEST_BIN) $(MAIN_BIN) cppcheck_final.txt test_input.txt *.dat testing_audit.log

audit:
	@rm -f testing_audit.log
	@echo "==================================================" >> testing_audit.log
	@echo "TIMESTAMP: $$(date)" >> testing_audit.log
	@echo "ACTION: Compilation and GTest Execution" >> testing_audit.log
	@echo "==================================================" >> testing_audit.log
	@$(MAKE) clean >> testing_audit.log 2>&1
	@$(MAKE) test >> testing_audit.log 2>&1
	
	@echo -e "\n==================================================" >> testing_audit.log
	@echo "TIMESTAMP: $$(date)" >> testing_audit.log
	@echo "ACTION: GCOV Coverage Analysis" >> testing_audit.log
	@echo "==================================================" >> testing_audit.log
	@gcov $(SRC_C) >> testing_audit.log 2>&1

	@echo -e "\n==================================================" >> testing_audit.log
	@echo "TIMESTAMP: $$(date)" >> testing_audit.log
	@echo "ACTION: Valgrind Memory Analysis" >> testing_audit.log
	@echo "==================================================" >> testing_audit.log
	@$(CC) $(CFLAGS) *.c -o $(MAIN_BIN)
	@echo 0 | valgrind --leak-check=full ./$(MAIN_BIN) >> testing_audit.log 2>&1
	
	@echo -e "\n==================================================" >> testing_audit.log
	@echo "TIMESTAMP: $$(date)" >> testing_audit.log
	@echo "ACTION: Cppcheck Static Analysis" >> testing_audit.log
	@echo "==================================================" >> testing_audit.log
	@cppcheck --enable=warning,performance,portability --std=c11 -I. --suppress=missingIncludeSystem . >> testing_audit.log 2>&1
	
	@echo "✅ Audit complete! Check 'testing_audit.log' for the timestamped report."
