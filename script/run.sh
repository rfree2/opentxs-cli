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
    fi 
}


checkExist
checkSrv
removePid

if [[ $# -eq 1 ]]; then 
    if [[ $1 -eq "gdb" ]]; then 
        setfattr -n user.pax.flags -v rm ./build/bin/otx &> /dev/null
        gdb -return-child-result -ex run -ex "thread apply all bt" -ex "quit" --args ./build/bin/otx +normal +debugfile --complete-shell	
    fi
else 
    $executable ./build/bin/otx +normal +debugfile --complete-shell  
fi

removePid
