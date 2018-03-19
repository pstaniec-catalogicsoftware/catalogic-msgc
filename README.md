# catalogic-msgc

to build, install flex 2.6.4 into flex-2.6.4 subdirectory, then run make (should work on Linux):
```sh
wget https://github.com/westes/flex/releases/download/v2.6.4/flex-2.6.4.tar.gz -O - | tar -zxvf - 
cd flex-2.6.4
./configure
make
cd ..
make
```
