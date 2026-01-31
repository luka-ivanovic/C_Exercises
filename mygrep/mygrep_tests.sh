####################################################
#TESTS GENERATED WITH AI ##########################
#BASED ON THE EXERCISE TESTS, BUT AS SHELL SCRIPT##
###################################################

#!/bin/bash

# Configuration
EXECUTABLE="./mygrep"
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color
TOTAL_TESTS=0
PASSED_TESTS=0

# Check if executable exists
if [ ! -x "$EXECUTABLE" ]; then
    echo -e "${RED}Error: $EXECUTABLE not found or not executable.${NC}"
    echo "Please compile your program (e.g., gcc -o mygrep mygrep.c) and try again."
    exit 1
fi

# Helper function to print results
check_result() {
    local test_name="$1"
    local expected_code="$2"
    local actual_code="$3"
    local expected_output="$4" # Optional file containing expected output
    local actual_output="$5"   # Optional file containing actual output

    TOTAL_TESTS=$((TOTAL_TESTS + 1))

    local fail=0

    # Check Exit Code
    if [ "$expected_code" -ne "$actual_code" ]; then
        echo -e "${RED}[FAIL] $test_name: Expected exit code $expected_code, got $actual_code${NC}"
        fail=1
    fi

    # Check Output Content (if files provided)
    if [ -n "$expected_output" ] && [ -n "$actual_output" ]; then
        if ! diff -q "$expected_output" "$actual_output" > /dev/null; then
             echo -e "${RED}[FAIL] $test_name: Output mismatch${NC}"
             echo "Expected:"
             cat "$expected_output"
             echo "Got:"
             cat "$actual_output"
             fail=1
        fi
    fi

    if [ $fail -eq 0 ]; then
        echo -e "${GREEN}[PASS] $test_name${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    fi
}

echo "Starting tests for mygrep..."
echo "----------------------------"

# Clean up previous runs
rm -f infile* outfile* longline longgrep expected.txt actual.txt

# --- USAGE TESTS (Expect Exit Code 1) ---
#

# A-Testcase 01: usage-1
$EXECUTABLE -x > /dev/null 2>&1
check_result "01: usage-1 (-x)" 1 $?

# A-Testcase 02: usage-2
$EXECUTABLE -i -i > /dev/null 2>&1
check_result "02: usage-2 (-i -i)" 1 $?

# A-Testcase 03: usage-3
$EXECUTABLE -o > /dev/null 2>&1
check_result "03: usage-3 (-o)" 1 $?

# A-Testcase 04: usage-4
$EXECUTABLE > /dev/null 2>&1
check_result "04: usage-4 (no args)" 1 $?


# --- EASY TESTS (Expect Exit Code 0 + Output Match) ---
#

# A-Testcase 05: easy-1
echo -e "operating systems." > expected.txt
echo -e "Systems, welcome\nto\noperating systems.\n" | $EXECUTABLE "sys" > actual.txt 2>&1
check_result "05: easy-1 (simple pipe)" 0 $? expected.txt actual.txt

# A-Testcase 06: easy-2
echo -e "Small\nis smaller" > expected.txt
echo -e "Small\nis smaller\nthan huge." | $EXECUTABLE -i "small" > actual.txt 2>&1
check_result "06: easy-2 (case insensitive)" 0 $? expected.txt actual.txt

# A-Testcase 07: easy-3
echo -e "Oh, z.,\nZZZzZ.\nzZ...." > expected.txt
echo -e "Oh, z.,\nZZZzZ.\nz\nZ\nZZZz\nzZ...." | $EXECUTABLE -i "Z." > actual.txt 2>&1
check_result "07: easy-3 (regex dot)" 0 $? expected.txt actual.txt

# A-Testcase 08: easy-4
echo -e "new\nline" > expected.txt
echo -e "new\nline" | $EXECUTABLE "" > actual.txt 2>&1
check_result "08: easy-4 (empty string)" 0 $? expected.txt actual.txt


# --- FILE TESTS (Expect Exit Code 0 + File Output) ---
#

# A-Testcase 09: file-1
echo -e "2 is the oddest prime.\nwhat?\nAre YOU sure\n12345, yes." > infile.txt
echo -e "2 is the oddest prime.\n12345, yes." > expected.txt
$EXECUTABLE -o outfile.txt 2 infile.txt > /dev/null 2>&1
RESULT=$?
# Verify content of outfile.txt, not stdout
check_result "09: file-1 (output file)" 0 $RESULT expected.txt outfile.txt

# A-Testcase 10: file-2
echo ".,:-!=?% is %n%% TEST???" > infile1
echo ".,:-!=?% is %n%% yes!!!" >> infile1
echo -e "testing\ntester\nTESTEST\nteeest\nabc\ndef" > infile2
echo -e ".,:-!=?% is %n%% TEST???\ntesting\ntester\nTESTEST" > expected.txt
$EXECUTABLE -i Test infile1 infile2 > actual.txt 2>&1
check_result "10: file-2 (multiple files)" 0 $? expected.txt actual.txt

# A-Testcase 11: file-3
echo "some old content" > outfile
echo -e "abcdef\naabcdef" > expected.txt
echo -e "abcdef\nabbcdef\nabscdef\nabecdef\naabcdef\n" | $EXECUTABLE -i -o outfile "ABC" > /dev/null 2>&1
RESULT=$?
check_result "11: file-3 (overwrite output)" 0 $RESULT expected.txt outfile


# --- COMPLEX / ERROR TESTS ---
#

# A-Testcase 12: long-line
# Constructing the long line exactly as shown in the screenshot using bash
( echo -n "yes"; printf -- "-%.0s" {1..8000}; echo "..." ) > longline
echo "yes..." > expected.txt
$EXECUTABLE -i "yes" longline > longgrep 2>&1
RESULT=$?
# Verify output by trimming dashes as per the screenshot test verification
tr -d "-" < longgrep > actual.txt
check_result "12: long-line (buffer handling)" 0 $RESULT expected.txt actual.txt

# A-Testcase 13: file-error-1
rm -f nonExistingTestfile
$EXECUTABLE test nonExistingTestfile > /dev/null 2>&1
check_result "13: file-error-1 (missing input file)" 1 $?

# A-Testcase 14: file-error-2
touch existingTestfile
chmod 0000 existingTestfile # Make unreadable
# Note: This test might fail if run as root (root ignores permissions)
echo "test" | $EXECUTABLE -o existingTestfile test > /dev/null 2>&1
RESULT=$?
chmod +w existingTestfile # Restore permissions so we can delete it later
check_result "14: file-error-2 (permission denied)" 1 $RESULT

# --- SUMMARY ---
echo "----------------------------"
echo "Tests Completed: $PASSED_TESTS/$TOTAL_TESTS"

# Cleanup
rm -f infile* outfile* longline longgrep expected.txt actual.txt existingTestfile

if [ $PASSED_TESTS -eq $TOTAL_TESTS ]; then
    echo -e "${GREEN}ALL TESTS PASSED${NC}"
    exit 0
else
    echo -e "${RED}SOME TESTS FAILED${NC}"
    exit 1
fi