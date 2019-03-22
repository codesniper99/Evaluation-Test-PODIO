git init 
git clone https://github.com/AIDASoft/podio.git
git fetch https://github.com/AIDASoft/podio.git
git checkout -b mybranch

cd podio
source ./init.sh

mkdir "build"
mkdir "install"
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install ..
make -j 4 install

../install/tests/write

../install/tests/read

python ../tests/read.py

make test