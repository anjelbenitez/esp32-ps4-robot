# error if script is executed
if [ -n "${BASH_SOURCE-}" ] && [ "${BASH_SOURCE[0]}" = "${0}" ]
    then
        echo "This script should be sourced, not executed:"
        # shellcheck disable=SC2039  # reachable only with bash
        echo ". ${BASH_SOURCE[0]}"
        exit 1
fi

# point at internal idf-tools path
REPO_ROOT=$(dirname $(realpath $BASH_SOURCE))  # git rev-parse --show-toplevel

# Overriding this variable moves location of installed toolchain to non-common directory.
#export IDF_TOOLS_PATH=$REPO_ROOT/tools/idf-tools


if [ -n "$1" ] && [ "$1" == "install" ]
    then
        $REPO_ROOT/tools/esp-idf/install.sh
fi

# source IDF environment
. $REPO_ROOT/tools/esp-idf/export.sh
