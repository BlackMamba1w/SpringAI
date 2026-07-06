#!/bin/sh
#
# Use this script to run your program LOCALLY.
#
#
set -e # Exit early if any commands fail

#
# - Edit this to change how your program compiles locally

#
# - Edit this to change how your program runs locally
exec "$(dirname "$0")/build/claude-code" "$@"