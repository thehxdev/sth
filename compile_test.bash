#!/usr/bin/env bash

log_run() {
    echo "[X] $@"
    $@
}

if [[ -z "$CC" ]]; then
    CC=cc
fi
CFLAGS=(-std=gnu99 -Wall -Wextra -Og -g3)

log_run $CC "${CFLAGS[@]}" -c sth.c ; rm -f sth.o
