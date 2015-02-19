#!/bin/bash

executable="./build/bin/otx"

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


checkExist
checkSrv
removePid

$executable ./build/bin/otx +normal +debugfile --complete-shell	
# gdb -return-child-result -ex run -ex "thread apply all bt" -ex "quit" --args ./build/bin/otx +normal +debugfile --complete-shell	

removePid
