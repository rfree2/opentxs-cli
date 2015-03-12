#!/bin/bash 

times=$1 
test_script="printf '_' | script/run-test.sh"
dir=$(date "+%d-%m-%y_%H%M%S")
full_dir="test_output/$dir"

mkdir -p "tests_output"
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
        name="$full_dir/test$i.txt"
        echo "test number: $i $name"
        echo "test number: $i" >> $full_dir/info.txt
        eval $test_script --gtest_output=xml:$full_dir/$i.xml   &>  $name                                                                                                                                                                                                                                                                                                                                                                                                                                                             
		tail $name    
        sleep 1
	done 
}

checkExist 
run
