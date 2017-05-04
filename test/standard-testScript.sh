# !/bin/sh
echo "==CLEARING FILES=="

if [ -f standardHideUnhideWorked.jpg ]
then 
    rm standardHideUnhideWorked.jpg
fi 

if [ -f output/testOut.ppm ]
then 
    rm output/testOut.ppm
fi 

echo "==TESTING HIDE=="
./hide ppm-files/mandrill.ppm output/testOut.ppm < img/milhouse.jpg
echo "-----------------"
echo "==TESTING UNHIDE=="
./unhide output/testOut.ppm > standardHideUnhideWorked.jpg
echo "=================="
echo "If everything worked, there should be a picture of a monkey called 
'testOut.ppm' and a picture of milhouse called 'standardHideUnhideWorked.jpg'"
