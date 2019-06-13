#!/bin/sh
cd `dirname $0`
workdir=$PWD
echo $workdir

wget https://sourceforge.net/projects/opende/files/ODE/0.13/ode-0.13.tar.bz2
tar -xf ode-0.13.tar.bz2
cd ode-0.13/
cd build
#premake4 --with-demos --platform=x64 --only-double --only-shared codeblocks
premake4 --with-demos --platform=x64 --only-double --only-shared gmake
#cd codeblocks
cd gmake
#codeblocks --target=Release --rebuild ode.cbp
#codeblocks --target=Release --rebuild drawstuff.cbp
make config=release ode
make config=release drawstuff

#./configure --enable-release --with-x --enable-double-precision --with-libccd CFLAGS="-fPIC"
#make
#sudo make install
#sudo cp -r include/drawstuff /usr/local/include/
#sudo cp drawstuff/src/.libs/libdrawstuff.* /usr/local/lib
#sudo ldconfig

#premake4 --with-demos  --with-tests  --platform=x64 --only-double --only-shared codeblocks
#codeblocks --target=Release --rebuild drawstuff.cbp

cd $workdir
git clone https://github.com/Nobu19800/EV3SimulatorRTC.git
cd EV3SimulatorRTC/
mkdir build
cd build
cmake -DODE_DIRECTORIY=$workdir/ode-0.13 ..
make
cp -r $workdir/ode-0.13/drawstuff $workdir/EV3SimulatorRTC/build/src

cd $workdir
wget https://raw.githubusercontent.com/OpenRTM/RTM_Tutorial_EV3/master/EXE/test.csv
wget https://raw.githubusercontent.com/OpenRTM/RTM_Tutorial_EV3/master/EXE/rtc.conf
wget https://raw.githubusercontent.com/OpenRTM/RTM_Tutorial_EV3/master/EXE/EV3Simulator.conf

cp test.csv $workdir/EV3SimulatorRTC/build/
cp rtc.conf $workdir/EV3SimulatorRTC/build/
cp EV3Simulator.conf $workdir/EV3SimulatorRTC/build/

