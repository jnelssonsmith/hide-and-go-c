# !/bin/sh
echo "==CLEARING FILES=="

if [ -f sFlagWorked.jpg ]
then 
    rm sFlagWorked.jpg
fi 

if [ -f output/sFlagOut.ppm ]
then 
    rm output/sFlagOut.ppm
fi 

echo "==TESTING S FLAG FOR HIDE=="
./hide -s ppm-files/mandrill.ppm output/sFlagOut.ppm < img/abeVsCroc.jpg
echo "-----------------"
echo "==TESTING S FLAG FOR UNHIDE=="
./unhide output/sFlagOut.ppm > sFlagWorked.jpg
echo "=================="
echo "If everything worked, there should be a picture of a Abe Simpson fighting a toilet gator called 'sFlagWorked.jpg' after you close the comparison window"