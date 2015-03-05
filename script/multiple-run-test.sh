#!/bin/bash 

times=$1 
test_script="printf '_' | script/run-test.sh"
dir=$(date "+%H%M%S_%m%d%y")

mkdir -p $dir

function checkExist() { 
    if [ ! -f $1 ]; then
        echo "File: $executable not found!"
        echo "Build opentxs-cli and make sure you are in opentxs-cli directory!"
        exit 1
    fi
}

function run() { 
	i="0"   
	                                                                                                                                                                                             
	while [ $i -lt $times ]; do                                                                                                                                                                                                                   
        i=$[$i+1]  
        name="$dir/test$i.txt"
        echo "test number: $i $name"
        echo "test number: $i" >> $dir/info.txt
        eval $test_script --gtest_output=xml:$dir/tests_$i.xml   &>  $name                                                                                                                                                                                                                                                                                                                                                                                                                                                             
		tail $dir/tests_$i.xml
        sleep 1
	done 
}

checkExist 
run
