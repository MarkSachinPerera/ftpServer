#!/bin/bash

# DIR=$(ls | grep dataDir)

# if  [[ "$DIR" ]] ; then
#     rm -r dataDir/
# fi

# mkdir dataDir/
# cd dataDir

# for i in {1..10}
# do
#     echo "File $i content" > $i
# done


##### problem with the above code: there are some permission issues 
# i think and due to that things arent working
# solutions: dump files to cwd

for i in {1..10}
do
    echo "File $i content" > data.$i
done