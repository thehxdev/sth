#!/bin/sh

log_and_run() {
    echo "[EXEC] $@"
    $@
}

CFALGS_=(-Wall -Wextra -Og -g3)

if [ -z "$CC" ]; then
    CC=cc
elif [ "$CC" == "clang" ]; then
    CFALGS_+=(-fsanitize=address,undefined)
fi

log_and_run $CC \
    "${CFALGS_[@]}" \
    -o a.out \
    ./tests/test.c && ./a.out
