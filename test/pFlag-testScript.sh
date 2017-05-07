# !/bin/sh
echo "==CLEARING FILES=="

if [ -f secretMessage*.txt ]
then 
    rm secretMessage*.txt
fi 

if [ -f output/pFlagOut* ]
then 
    rm output/pFlagOut*
fi 

echo "==TESTING P FLAG FOR HIDE=="
./hide -p ./test/pinput.txt
echo "-----------------"
echo "==TESTING P FLAG FOR UNHIDE=="
./unhide ./output/pFlagOut1.ppm > secretMessage1.txt
./unhide ./output/pFlagOut2.ppm > secretMessage2.txt
./unhide ./output/pFlagOut3.ppm > secretMessage3.txt
echo "=================="
echo "If everything worked, there should be three text files called secretMessage1 through 3"