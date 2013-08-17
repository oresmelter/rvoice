
for fl in *.wav;do
    echo $fl
    ./speak < $fl
./play_wav -r 8000 -c 1 -f wav < $fl
sleep 1
#sync
#play  $fl
done
