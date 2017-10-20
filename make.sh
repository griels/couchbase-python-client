#!/bin/bash

source ~/root/virtualenvs/2.7/default/bin/activate
rm -Rf build
python setup.py $1 --inplace --include-dir=../libcouchbase/include --library-dir=../libcouchbase/build/lib
python setup.py install
