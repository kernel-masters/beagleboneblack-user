if [ -z "$1" ]; then
        echo "${Red}usage: sudo ./$(basename $0)  <set|clear|input|toggle> <gpio pin number> ${NC}"
fi


while [ ! -z "$1" ] ; do
        case $1 in
        -h|--help)
                echo "${Red}usage: sudo ./$(basename $0)  <set|clear|input|toggle> <gpio pin number> ${NC}"
                ;;
	set|clear)
                cd /sys/class/gpio
	      	if [ -d gpio$2 ];then
			cd gpio$2
		else			
	        	echo $2 > export
                	cd gpio$2
		fi
		status=$(cat direction)
		if [ $status = "in" ];then
                        echo out > direction   	
		fi	
		if [ $1 = "set" ]; then
			echo 1 > value
		fi
		if [ $1 = "clear" ]; then
			echo 0 > value
		fi
		;;
	
	input)
		status=$(cat direction)
                if [ $status = "out" ];then
                    	echo in > direction
		fi
			echo -n "value:"
		cat value
		;;
	 
	toggle)
		temp=$(cat value)
		if [ $temp = "1" ]; then
                        echo 0  > value  
		else 
			echo 1 > value
		fi

		cd ~/	
                ;;
	esac
    	shift
done
