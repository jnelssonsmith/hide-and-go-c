# !/bin/sh
echo "==CLEARING FILES=="

if [ -f mFlagWorked.gif ]
then 
    rm mFlagWorked.gif
fi 

if [ -f output/mFlagOut-* ]
then 
    rm output/mFlagOut-*
fi 

echo "==TESTING M FLAG FOR HIDE=="
./hide -m  5 ppm-files/mInput output/mFlagOut < img/spongememe.gif
echo "-----------------"
echo "==TESTING M FLAG FOR UNHIDE=="
./unhide -m output/mFlagOut > mFlagWorked.gif
echo "=================="
echo "If everything worked, there should be a gif of a spongebob called 'mFlagWorked.gif'"
