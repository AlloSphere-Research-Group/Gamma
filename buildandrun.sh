

FILENAME=`echo $1 |cut -d'.' -f1 | sed -e "s|/|_|g"`
TARGET=${FILENAME}_run

if [ ! -f Makefile ]; then
    ./distclean
    cmake .
    if [ $? != 0 ]; then
	exit 1
    fi
fi 

make $TARGET -j4 $*

# If command failed, try again after running cmake (in case it is a new target)
if [ $? != "0" ]
    then
    TARGETS=`make -qp | awk -F':' '/^[a-zA-Z0-9][^$#\/\t=]*:([^=]|$)/ {split($1,A,/ /);for(i in A)print A[i]}'`
    case $TARGETS in
	*"$TARGET"*) echo "Target exists." ;;
	*) echo "Can't find target --------------- Running CMAKE again"
	    cmake .
	    if [ $? != 0 ]; then
		exit 1
	    fi
	    make ${FILENAME} -j4 $* &> make_output.txt
	    if [ $? != "0" ]
	    then
		cat make_output.txt
	    fi
    esac
fi