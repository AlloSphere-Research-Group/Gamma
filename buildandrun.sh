

FILENAME=`echo $1 |cut -d'.' -f1 | sed -e "s|/|_|g"`

if [ ! -f Makefile ]; then
    .distclean
    cmake .
    if [ $? != 0 ]; then
	exit 1
    fi
fi 

make `echo $1 |cut -d'.' -f1 | sed -e "s|/|_|g"`_run -j4 $*

# If command failed, try again after running cmake (in case it is a new target)
if [ $? != "0" ]
    then
    echo "Can't find target --------------- Running CMAKE again"
    cmake .
    if [ $? != 0 ]; then
	exit 1
    fi
    make `echo $1 |cut -d'.' -f1 | sed -e "s|/|_|g"`_run -j4 $*
fi