cdir=`pwd`
MYDAT=$(date +"20%y_%m_%d_%Hh")
Prog=VOICE

cd src
make clean
cd $cdir
tar -cjf $Prog-src-$MYDAT.tgz `ls`
