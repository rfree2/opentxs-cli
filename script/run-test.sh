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
checkExist
checkSrv
removePid 
printHelp
cmd="$executable --gtest_break_on_failure $@"
echo $cmd 

 echo "[enter]"
 read _ 

eval $cmd
