#!/usr/bin/env bash

log_run() {
    echo "[X] $@"
    $@
}

DIRECTORIES=( allocators/ base/ ds/ io/ os/ )

log_run tar czf libsth.tar.gz sth.* "${DIRECTORIES[@]}"

echo "[I] packaged to \`libsth.tar.gz'"
