#!/bin/bash

# check if build directory already exists, if so, remove it
if [ -d "build" ]; then
    rm -rf build
fi

# create build directory
mkdir build

# change to build directory
cd build

# run cmake
cmake ..

# run make
make

# copy .so file to site-packages directory
echo "Site-packages directory:"
python3 -m site --user-site
echo "Copying .so files to site-packages directory..."
python3 -m site --user-site | xargs -I {} sh -c 'cp ../dist/*.so {}; echo "Copied: " ../dist/*.so'
echo "Finished copying .so files."
