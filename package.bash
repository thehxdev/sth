#!/usr/bin/env bash

log_run() {
    echo "[X] $@"
    $@
}

DIRECTORIES=( allocators/ base/ ds/ io/ os/ encoding/ )

log_run tar czf libsth.tar.gz sth.* LICENSE README.txt "${DIRECTORIES[@]}"

echo "[I] packaged to \`libsth.tar.gz'"
