#!/bin/sh
cd tests
for i in * ;
do
	if [ -e  ../test_ref/"$i" ] ; then
		../canons < "$i" | diff - ../test_ref/"$i";
		if [ $? != "0" ]; then 
			echo -n 'output of `../canons` < '"$i"' and its refernce'
			echo " file differ!"
			exit 1
		fi
	fi
done
