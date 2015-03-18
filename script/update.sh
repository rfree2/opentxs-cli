#!/bin/bash 


echo "updating" 
git pull 
git log -1 
sleep 3
#read _ 
cd build/

make && cd .. #&&  ./run.sh

