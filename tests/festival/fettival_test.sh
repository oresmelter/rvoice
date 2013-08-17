cdir=`pwd`
MYDAT=$(date +"20%y год %m го %d го %H часов %M минут %S секунд")
clear
#cat /proc/asound/pcm
echo $MYDAT

# Install festival
#sudo aptitude install festival speech-tools festvox-ru; #exit
echo "Текущее время $MYDAT" | festival --tts --language russian
sleep 2
echo "Среда 31 июля 2013 го года 10 часов 28 минут 0 секунд." | festival --tts --language russian
exit
sleep 2
#festival -b "(begin (voice_msu_ru_nsh_clunits) (SayText \"$1\" nil))"

cat $cdir/Festival.txt |festival --tts --language russian 
