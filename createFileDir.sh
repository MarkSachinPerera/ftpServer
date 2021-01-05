#!/bin/bash

DIR=$(ls | grep dataDir)

if  [[ "$DIR" ]] ; then
    rm -r dataDir/
fi

mkdir dataDir/
cd dataDir

for i in {1..10}
do
    echo "File $i content" > $i
done