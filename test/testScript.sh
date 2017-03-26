# !/bin/sh
echo "==CLEARING FILES=="

if [ -f thrillhouse.jpg ]
then 
    rm thrillhouse.jpg
fi 

if [ -f testOut.ppm ]
then 
    rm testOut.ppm
fi 

echo "==TESTING HIDE=="
./hide ppm-files/mandrill.ppm testOut.ppm < img/milhouse.jpg
echo "-----------------"
echo "==TESTING UNHIDE=="
./unhide testOut.ppm > thrillhouse.jpg
echo "=================="
echo "If everything worked, there should be a picture of a monkey called 
'testOut.ppm' and a picture of milhouse called 'thrillhouse.jpg'"
