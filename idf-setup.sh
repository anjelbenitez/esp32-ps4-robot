# define idf tools path
PROJ_ROOT_PATH=$(dirname $(realpath $BASH_SOURCE))

# install esp-idf
if [ -n "$1" ] && [ "$1" == "install" ]
    then
        $PROJ_ROOT_PATH/tools/esp-idf/install.sh
fi

# source IDF environment
. $PROJ_ROOT_PATH/tools/esp-idf/export.sh
