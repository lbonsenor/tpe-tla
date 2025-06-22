#! /bin/bash

set -euo pipefail

BASE_PATH="$(dirname "$0")/../.."
cd "$BASE_PATH"


# if [ "$#" -ne 2 ] || [ "$#" -ne 3 ]; then
#     echo "Usage: $0 <input file> <output file> -input or Usage: $0 <input file> <output file>"
# fi


INPUT="$1"
OUTPUT="$2"

shift 2
build/Compiler "$@" < "$INPUT" > "$OUTPUT"
#cat "$INPUT" | build/Compiler "$@"

# set -u

# BASE_PATH="$(dirname "$0")/../.."
# cd "$BASE_PATH"

# GREEN='\033[0;32m'
# RED='\033[0;31m'
# YELLOW='\033[1;33m'
# BLUE='\033[0;34m'
# CYAN='\033[0;36m'
# OFF='\033[0m'

# if [ $# -eq 0 ]; then
#     echo -e "${RED}Usage: $0 <test_file> [compiler_args...]${OFF}"
#     echo -e "${YELLOW}Example: $0 src/test/c/accept/test1.txt${OFF}"
#     exit 1
# fi

# INPUT="$1"
# shift 1

# echo -e "${BLUE}=== SINGLE FILE COMPILER TEST ===${OFF}"
# echo -e "${YELLOW}Testing file: $INPUT${OFF}"
# echo -e "${YELLOW}Additional args: $@${OFF}"
# echo ""

# # Enable ALL debugging
# export LOG_IGNORED_LEXEMES=true
# export YYDEBUG=1
# export LOG_LEVEL=DEBUG
# export DEBUG=1

# echo -e "${CYAN}╔══════════════════════════════════════════════════════════════════════════════╗${OFF}"
# echo -e "${CYAN}║${OFF} ${BLUE}INPUT CONTENT${OFF}"
# echo -e "${CYAN}╚══════════════════════════════════════════════════════════════════════════════╝${OFF}"
# echo -e "${CYAN}┌──────────────────────────────────────────────────────────────────────────────┐${OFF}"
# cat "$INPUT" | sed 's/^/│ /'
# echo -e "${CYAN}└──────────────────────────────────────────────────────────────────────────────┘${OFF}"
# echo ""

# echo -e "${CYAN}╔══════════════════════════════════════════════════════════════════════════════╗${OFF}"
# echo -e "${CYAN}║${OFF} ${BLUE}COMPILER OUTPUT${OFF}"
# echo -e "${CYAN}╚══════════════════════════════════════════════════════════════════════════════╝${OFF}"
# echo -e "${CYAN}┌──────────────────────────────────────────────────────────────────────────────┐${OFF}"

# # Run compiler and capture both stdout and stderr
# cat "$INPUT" | build/Compiler "$@" 2>&1 | sed 's/^/│ /'
# RESULT="${PIPESTATUS[1]}" # Get exit code of build/Compiler, not sed

# echo -e "${CYAN}└──────────────────────────────────────────────────────────────────────────────┘${OFF}"
# echo ""

# echo -e "${YELLOW} Exit code: $RESULT${OFF}"

# if [ "$RESULT" == "0" ]; then
#     echo -e "${GREEN}COMPILATION SUCCEEDED${OFF}"
# else
#     echo -e "${RED}COMPILATION FAILED${OFF}"
# fi

# exit $RESULT