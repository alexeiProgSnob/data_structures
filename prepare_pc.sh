DIRECTORY=/usr/include/aps
INC_DIR="$DIRECTORY"/ds

make clean
make

if [ ! -d "$DIRECTORY" ]; then
  mkdir $DIRECTORY
fi

if [ ! -d "$INC_DIR" ]; then
  mkdir $INC_DIR
fi

cp ./inc/* $INC_DIR/
cp ./dynamic_lib/libLDS.so /usr/lib/libLDS.so

cd ./test/
rm -rf *.out
rm -rf *.o
gcc uni_test.c -lLDS
./a.out
