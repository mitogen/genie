#!/usr/bin/env bash

set -euxo pipefail

self="${0}"
self_name="${self##*/}"

git rev-parse --git-dir 1>/dev/null # exit if not inside Git repo
readonly git_root_dir="$(git rev-parse --show-toplevel)"

readonly build_dir="${git_root_dir}/cmake-build-debug"
readonly calq_tests_app="${build_dir}/bin/calq-tests"
if [[ ! -x "${calq_tests_app}" ]]; then
    echo "[${self_name}] error: calq-tests application does not exist: ${calq_tests_app}"
    exit 1
fi

"${calq_tests_app}"
