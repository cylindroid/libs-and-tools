#!/bin/bash
#g++ -fPIC -shared -I/usr/include/python2.6/ -lpython2.6 -o wlev.so weighted_levenshtein.cc
python setup.py install --install-platlib=/usr/local/lib/python2.7/site-packages
