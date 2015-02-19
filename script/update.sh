#!/bin/bash 


echo "updating" 
git pull 
git log -1 
echo "press [enter] to continue"
read _ 
cd build/

make && cd .. &&  ./run.sh

