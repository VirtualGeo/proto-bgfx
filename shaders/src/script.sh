#! /bin/bash -e


for i in $(ls *.sc)
do
	# echo $i
	if [ $i = "varying.def.sc" ]; then
		continue
	fi
	name=$(echo $i | awk -F_ '{print $1}')
	ext=$(echo $i | awk -F'[_.]' '{print $2}')
	# echo $name
	# echo $ext

	# cmd='mv $i "$name.$ext_sc"'
	# echo $cmd
	mv -vi $i "$name.$ext"_sc

done
