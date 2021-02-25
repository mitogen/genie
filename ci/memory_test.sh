#!/bin/bash
# Do roundtrips with very small files (performance reasons) while checking for memory issues
set -e

############ Check prerequisites ###############

if ! command -v git &> /dev/null
then
    echo "Git could not be found. Please install git (debian: sudo apt-get install git)."
    exit 1
fi

if [[ "$OSTYPE" == "win32" || "$OSTYPE" == "cygwin" || "$OSTYPE" == "msys" ]]; then
    fileExt=".exe"
else
    fileExt=""
fi


git_root_dir="$(git rev-parse --show-toplevel)"


############ Run ###############

cd $git_root_dir/cmake-build-debug/bin
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 ./genie$fileExt run --help || { echo 'Memcheck failed!' ; exit 1; }
