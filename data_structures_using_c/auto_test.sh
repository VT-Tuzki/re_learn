#!/bin/bash

AUTO_TEST_DIR=$(pwd)/autotest


print_color() {
    local color_code=$1
    local text=$2
    case $color_code in
        "red")
            echo -e "\033[31m$text\033[0m"
            ;;
        "green")
            echo -e "\033[32m$text\033[0m"
            ;;
        "yellow")
            echo -e "\033[33m$text\033[0m"
            ;;
        "blue")
            echo -e "\033[34m$text\033[0m"
            ;;
        *)
            echo "$text"
            ;;
    esac
}

# Array to hold test case descriptions
declare -a use_lib_cases=(
    "yes"
    "no"
)

declare -a std_cases=(
    "c99"
    "c11"
)

traverse_test_cases() {
    for use_lib in "${use_lib_cases[@]}"; do
        for std in "${std_cases[@]}"; do
            mkdir -p "$AUTO_TEST_DIR/lib_($use_lib)_lib_std($std)"
            echo "Running test with --use-lib $use_lib and --std $std"
            ./configure.sh --use-lib $use_lib --std $std
            run_make $use_lib $std
        done
    done
}


run_make()  {
    # make clean
    make -B all > "$AUTO_TEST_DIR/lib_($1)_lib_std($2)/make.log" 2>&1
    if [ $? -eq 0 ]; then
        print_color "green" "Test passed."
    else
        print_color "red" "Test failed."
        fail_cases+=("--use-lib $1 --std $2")
    fi
    for fail_case in "${fail_cases[@]}"; do
        print_color "red" "Failed test case: $fail_case"
        print_color "red" "Check log file: $AUTO_TEST_DIR/lib_($1)_lib_std($2)/make.log"
    done
}


# Function to run configure and make
run_test() {
    make clean
    rm -rf "$AUTO_TEST_DIR"
    mkdir -p "$AUTO_TEST_DIR"
    traverse_test_cases
}

# Test cases
run_test

echo "All tests completed."