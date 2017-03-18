# !/bin/sh
echo "==TESTING HIDE=="
echo "Hello World!" | ./hide ppm-files/mandrill.ppm testOut.ppm
echo "-----------------"
echo "==TESTING UNHIDE=="
./unhide testOut.ppm