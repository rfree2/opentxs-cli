#!/bin/bash 

times=$1 
test_script="printf '_' | script/run-test.sh"
dir=$(date "+%d-%m-%y_%H%M%S")
full_dir="tests_output/$dir"

mkdir -p "tests_output"
mkdir -p $full_dir

function checkExist() { 
    if [ ! -f $1 ]; then
        echo "File: $executable not found!"
        echo "Build opentxs-cli and make sure you are in opentxs-cli directory!"
        exit 1
    fi
}

function run() { 
	i="0"   
    passed="0"
    failed="0"
	                                                                                                                                                                                             
	while [ $i -lt $times ]; do                                                                                                                                                                                                                   
        i=$[$i+1]  
        name="$full_dir/test$i.txt"
        echo "test number: $i $name"
        echo "test number: $i" >> $full_dir/info.txt
        eval $test_script --gtest_output=xml:$full_dir/$i.xml   &>  $name                                                                                                                                                                                                                                                                                                                                                                                                                                                             
		tail $name    
        sleep 1
        if [ -f $full_dir/$i.xml ]; then 
            echo "Test $i passed" >> $full_dir/summary.txt
            passed=$[$passed+1] 
        else 
            echo "Test $i failed"
            failed=$[$failed+1] >> $full_dir/summary.txt
        fi
	done 
    echo "====================================================="  >> $full_dir/summary.txt
    echo "passed: $passed" >> $full_dir/summary.txt
    echo "failed: $failed" >> $full_dir/summary.txt
    echo "all: $i"  $full_dir/summary.txt
}

checkExist 
run
