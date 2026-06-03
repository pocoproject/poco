#!/bin/bash
# Has to be executed from the root of the repository.
# Usually invoked by `make test`.
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./

# Colors
RED='\033[1;31m'
GREEN='\033[1;32m'
YELLOW='\033[1;33m'
NC='\033[0m'
BOLD='\033[1;39m'

RET=0
SQL_TEST_RET=1
MEM_LEAK_EXECUTED=false
MEM_LEAK_RET=1
CONFLICT_RET=1

#################################################
# Running SQL parser tests.
printf "\n${GREEN}Running SQL parser tests...${NC}\n"
bin/tests -f "test/queries/queries-good.sql" -f "test/queries/queries-bad.sql"
SQL_TEST_RET=$?

if [ $SQL_TEST_RET -eq 0 ]; then
  printf "${GREEN}SQL parser tests succeeded!${NC}\n"
else
  printf "${RED}SQL parser tests failed!${NC}\n"
fi

#################################################
# Running memory leak checks (only on Linux).
unamestr=$(uname)
if [[ "$unamestr" == 'Linux' ]]; then
  printf "\n${GREEN}Running memory leak checks...${NC}\n"
  valgrind --leak-check=full --error-exitcode=200 --log-fd=3 \
    bin/tests -f "test/queries/queries-good.sql" -f "test/queries/queries-bad.sql" \
    3>&1>/dev/null;

  MEM_LEAK_RET=$?
  MEM_LEAK_EXECUTED=true

  if [ $MEM_LEAK_RET -eq 0 ]; then
    printf "${GREEN}Memory leak check succeeded!${NC}\n"
  elif [ $MEM_LEAK_RET -eq 200 ]; then
    printf "${RED}Memory leak check failed!${NC}\n"
  elif [ $MEM_LEAK_RET -eq 127 ]; then
    printf "${RED}Memory leak check failed: command 'valgrind' not found!${NC}\n"
  else
    printf "${RED}Memory leak check failed: error code ${MEM_LEAK_RET}!${NC}\n"
  fi
else
  printf "\n${YELLOW}Skipping memory leak checks (can only be executed on Linux)!${NC}\n"
fi

#################################################
# Checking if the grammar is conflict free.
printf "\n${GREEN}Checking for conflicts in the grammar...${NC}\n"
printf "${RED}"
make -C src/parser/ test >>/dev/null
CONFLICT_RET=$?

if [ $CONFLICT_RET -eq 0 ]; then
  printf "${GREEN}Conflict check succeeded!${NC}\n"
else
  printf "${RED}Conflict check failed!${NC}\n"
fi

# Print a summary of the test results.
printf "
----------------------------------
${BOLD}Summary:\n"
if [ $SQL_TEST_RET -eq 0 ]; then printf "SQL Tests:              ${GREEN}Success${BOLD}\n";
else                             printf "SQL Tests:              ${RED}Failure${BOLD}\n"; fi
if [ "$MEM_LEAK_EXECUTED" = true ]; then
  if [ $MEM_LEAK_RET -eq 0 ]; then printf "Memory Leak Check:      ${GREEN}Success${BOLD}\n";
  else                             printf "Memory Leak Check:      ${RED}Failure${BOLD}\n"; fi
else                               printf "Memory Leak Check:      ${YELLOW}Skipped${BOLD}\n"
fi
if [ $CONFLICT_RET -eq 0 ]; then printf "Grammar Conflict Check: ${GREEN}Success${BOLD}\n";
else                             printf "Grammar Conflict Check: ${RED}Failure${BOLD}\n"; fi

if [[ $SQL_TEST_RET -ne 0 || $CONFLICT_RET -ne 0 ]]; then
  RET=1
fi

if [ $MEM_LEAK_RET -ne 0 ]; then
  if [ "$MEM_LEAK_EXECUTED" = true ]; then
    RET=1
  fi
fi


if [ $RET -eq 0 ]; then
  if [ "$MEM_LEAK_EXECUTED" = true ]; then printf "${GREEN}All tests passed!${NC}\n"
  else                                     printf "${YELLOW}Some tests were skipped!${NC}\n"
  fi
else                                       printf "${RED}Some tests failed!${NC}\n"
fi
printf "${NC}----------------------------------\n"

exit $RET
