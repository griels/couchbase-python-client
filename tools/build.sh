export py_version=$1
py_version=3.6
echo $1
source ~/root/virtualenvs/$py_version/default/bin/activate
export action=make
echo $action
echo boom
while [ -n "$action" ]
do
    echo flume
echo $action
    if [ $action == "clean" ]
    then
	   echo cleaning
        rm -Rf build
    else
      	echo building
        python setup.py build_ext --inplace     --library-dir ../libcouchbase/lib     --include-dir ../libcouchbase/include && python setup.py install
    fi
    shift
    export action=$2
done