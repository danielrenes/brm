#/bin/bash

TEST=0
DEBUG=0

for arg in "$@";
do
  case "$arg" in
  "test")
    TEST=1
    ;;
  "debug")
    DEBUG=1
    ;;
  esac
done

if [ -d build/ ]; then
  rm -r build/
fi

mkdir build
cd build/

export TESTING=$TEST
export DEBUGGING=$DEBUG

echo "TEST=$TEST"
echo "DEBUG=$DEBUG"

cmake ..

make
