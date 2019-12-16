BIT=$(getconf LONG_BIT)
DIRECTORY=/usr/include/alexei_prog_snob
make clean
make


if [ ! -d "$DIRECTORY" ]; then
  mkdir $DIRECTORY
fi

cp ./inc/* /usr/include/alexei_prog_snob/
cp ./dynamic_lib/"$BIT"bit/libLDS_"$BIT"bit.so /usr/lib/libLDS.so

cd ./test/
rm -rf *.out
rm -rf *.o
gcc uni_test.c -lLDS
./a.out
