#!/bin/bash 

executable="./build/tests/unittests-opentxs-cli"

function checkExist() { 
    if [ ! -f $executable ]; then
        echo "File: $executable not found!"
        echo "Build opentxs-cli and make sure you are in opentxs-cli directory!"
        exit 1
    fi
}

function removePid() {
    rm -f $HOME/.ot/client_data/ot.pid
}

function checkSrv() {
    if [ "$(pidof opentxs-notary)" ]; then
        echo "ok server is running" 
    else
        echo "server not found!" 
        exit 1
    fi
}

function printHelp() { 
	echo "Filter example: "
	echo "--gtest_filter=cUseOtChequeTest.* "
	echo ""
}

function parseArgs() { 
    param="--gtest_filter=$1.$2"
    echo $param 
    sleep 1
    eval "./build/tests/unittests-opentxs-cli $param" 
    exit 
}

checkExist
removePid 

if [[ $# == 3 ]] && [[ $1 == "parse" ]]; then
    parseArgs $2 $3 
 elif [[ $# == 2 ]] && [[ $1 == "parse" ]]; then
    parseArgs $2 "*"
fi

checkSrv
printHelp 

cmd="$executable --gtest_break_on_failure $@"
echo $cmd 

 echo "[enter]"
 read _ 

eval $cmd

removePid 