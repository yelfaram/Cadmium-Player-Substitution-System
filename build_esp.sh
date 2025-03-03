#! /bin/bash

source $EXPORT_IDF
if [ -d "build" ]; then rm -Rf build; fi

idf.py set-target esp32s3
idf.py build
