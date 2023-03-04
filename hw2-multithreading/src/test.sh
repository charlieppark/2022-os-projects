#!/bin/bash
make clear
make all
# echo "Welcome to prod-cons test"
# echo "please choose file you want to test"
# echo "1. /opt/android.tar   2. /opt/FreeBSD9-orig.tar  3. manual file"
# read choice
# if [ $choice -eq 1 ]; then
#     filename="/opt/android.tar"
# elif [ $choice -eq 2 ]; then
#     filename="/opt/FreeBSD9-orig.tar"
# elif [ $choice -eq 3 ]; then
#     echo "Input Filename"
#     read filename
# fi
for filename in "/opt/android.tar" "/opt/FreeBSD9-orig.tar"
do
    for nthreads in {1..1001}
    do
        for buf_size in 1048576 # 4096 131072 1048576 32768 131072 524288 1048576
        do
	    #echo "$filename - $nthreads prods - $nthreads cons - Buffer $buf_size Bytes"
            echo "$filename - $nthreads prods - $nthreads cons - Buffer $buf_size Bytes :" >> result
            { time ./main $filename $nthreads $nthreads $buf_size ; } 2>> result
            echo " " >> result
        done
    done
done

echo -ne '\007'
echo -ne '\007'
echo -ne '\007'
echo -ne '\007'
echo -ne '\007'
