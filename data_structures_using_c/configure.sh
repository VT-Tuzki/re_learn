#!/bin/bash

# Default values
LIB_INSTALL_DIR="/usr/local/lib"
LIB_NAME="vtuzki"
USE_LIB="yes"
STD="c99"
STATIC_OR_DYNAMIC="dynamic"
# Parse command line arguments
while [[ "$#" -gt 0 ]]; do
    case $1 in
        --lib-dir) LIB_INSTALL_DIR="$2"; shift ;;
        --lib-name) LIB_NAME="$2"; shift ;;
        --use-lib) USE_LIB="$2"; shift ;;
        --std) STD="$2"; shift ;;
        --lib-type) STATIC_OR_DYNAMIC="$2"; shift ;;
        *) echo "Unknown parameter passed: $1"; exit 1 ;;
    esac
    shift
done

# Update Makefile
sed -i "s|^\(LIB_GOLBAL_INSTALL_DIR = \).*|\1$LIB_INSTALL_DIR|g" Makefile

sed -i "s|^\(C_LIB_NAMES = \).*|\1$LIB_NAME|g" Makefile

sed -i "s|^\(IS_C_APP_USE_LIB = \).*|\1$USE_LIB|g" Makefile

sed -i "s|^\(USE_LIB_STATIC_OR_DYNAMIC = \).*|\1$STATIC_OR_DYNAMIC|g" Makefile

sed -i "s|^\(LANGUAGE_STANDARD = \).*|\1$STD|g" Makefile

echo "Configuration complete."
echo "LIB_GOLBAL_INSTALL_DIR set to $LIB_INSTALL_DIR."
echo "C_LIB_NAMES set to $LIB_NAME."
echo "IS_C_APP_USE_LIB set to $USE_LIB."
echo "USE_LIB_STATIC_OR_DYNAMIC set to $STATIC_OR_DYNAMIC."
echo "LANGUAGE_STANDARD set to $STD."