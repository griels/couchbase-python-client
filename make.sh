#!/bin/bash

source ~/root/virtualenvs/2.7/default/bin/activate
rm -Rf build
python setup.py $1
python setup.py install