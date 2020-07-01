#!/bin/sh

if [ -z "$1" ]; then
        echo "${Red}usage: sudo ./$(basename $0)  <set|clear|input|toggle|unexport> <gpio pin number> ${NC}"
	exit 0
fi

export SYS_GPIO=/sys/class/gpio

if [ -d ${SYS_GPIO}/gpio$2 ];then
	echo "gpio$2 export has done"
else
	echo $2 > ${SYS_GPIO}/export
fi

while [ ! -z "$1" ] ; do
        case $1 in
        -h|--help)
                echo "${Red}usage: sudo ./$(basename $0)  <set|clear|input|toggle> <gpio pin number> ${NC}"
                ;;
	set|clear)
		status=$(cat ${SYS_GPIO}/gpio$2/direction)
		if [ $status = "in" ];then
                        echo out > ${SYS_GPIO}/gpio$2/direction
		fi	
		if [ $1 = "set" ]; then
			echo 1 > ${SYS_GPIO}/gpio$2/value
		fi
		if [ $1 = "clear" ]; then
			echo 0 > ${SYS_GPIO}/gpio$2/value
		fi
		;;
	
	input)
		status=$(cat ${SYS_GPIO}/gpio$2/direction)
		if [ $status = "out" ];then
			echo in > ${SYS_GPIO}/gpio$2/direction
		fi
		echo -n "value:"
		cat ${SYS_GPIO}/gpio$2/value
		;;
	 
	toggle)
		status=$(cat ${SYS_GPIO}/gpio$2/direction)
		if [ $status = "in" ];then
			echo out > ${SYS_GPIO}/gpio$2/direction
		fi
		temp=$(cat ${SYS_GPIO}/gpio$2/value)
		if [ $temp = "1" ]; then
			echo 0  > ${SYS_GPIO}/gpio$2/value
		else
			echo 1 > ${SYS_GPIO}/gpio$2/value
		fi
		;;

	unexport)
		if [ -d ${SYS_GPIO}/gpio$2 ];then
			echo $2 > ${SYS_GPIO}/unexport
			echo "gpio$2 unexport has done"
		fi
		;;
	esac
	shift
done
