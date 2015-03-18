#!/bin/bash 

times=$1 
args=$#

test_script="printf '_' | script/run-test.sh"
dir=$(date "+%d-%m-%y_%H%M%S")
full_dir="tests_output/$dir"
mkdir -p "tests_output"
mkdir -p $full_dir

function checkExist() { 
    if [ ! -f $1 ]; then
        echo "$1 not found!"
        exit 1
    fi
}

function checkSrv() {
    if ! [ "$(pidof opentxs-notary)" ]; then
        echo "server not found!" 
        exit 1
    fi
}

function check() { 
    echo -en "\e[91m" 
    if ! [[ $args == 1 ]]; then 
        echo "usage: script/multiple-run-test.sh <number of tests>"
        exit 1
    fi    

    re='^[0-9]+$'
    if ! [[ $times =~ $re ]] ; then
        echo "[$times] isn't a number!"
        exit 1
    fi 
    
    checkExist "script/run-test.sh"
    checkExist "./build/tests/unittests-opentxs-cli"
    checkSrv
    echo -en "\e[39m"
} 

function percent() {
    if ! [[ $# == 2 ]] ; then 
        return 
    fi
    item=$2
    total=$1
    if [ $item -le "0" ] && [ $total -le "0" ]; then 
        echo "" 
    elif [ $item -eq "0" ] && [ $total -gt "0" ]; then
        echo "0%"       
    else
        prc=$(printf '%i %i' $item $total | awk '{ pc=100*$1/$2; i=int(pc); print (pc-i<0.5)?i:i+1 }')
        echo "$prc%"
    fi
}

function run() { 
    i="0"   
    passed="0"
    failed="0"
	                                                                                                                                                                                             
	while [ $i -lt $times ]; do                                                                                                                                                                                                                   
        i=$[$i+1]  
        name="$full_dir/test$i.txt"
        echo "test number: $i" >> $full_dir/info.txt
        echo -n "test number: $i $name"
        eval $test_script --gtest_output=xml:$full_dir/$i.xml   &>  $name                                                                                                                                                                                                                                                                                                                                                                                                                                                             
        sleep 1
        if [ -f $full_dir/$i.xml ]; then 
            echo -e " ... \e[92mpassed \e[39m"
            echo "Test $i passed" >> $full_dir/summary.txt
            passed=$[$passed+1] 
        else 
            echo -e " ... \e[91mfailed \e[39m"
            echo "Test $i failed" >> $full_dir/summary.txt
            failed=$[$failed+1] 
            echo "$i -- $full_dir/$i.xml $name" >> $full_dir/failed.txt
        fi
	done 
    passed_percent="`percent $i $passed`"
    failed_percent="`percent $i $failed`"
    echo "====================================================="  >> $full_dir/summary.txt
    echo -e "  tests: $i\n"  >> $full_dir/summary.txt 
    echo  " passed: $passed - $passed_percent" >> $full_dir/summary.txt
    echo  " failed: $failed - $failed_percent" >> $full_dir/summary.txt

    tail -n 5 $full_dir/summary.txt
    echo "====================================================="
    if [ $failed -gt "0" ]; then 
        echo -e "\e[91mSome tests failed. More informations are in file: $full_dir/failed.txt\e[39m"   
    else 
        echo -e "\e[92mAll tests passed\e[39m"
    fi    
    echo ""
}

check
run
