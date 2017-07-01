#!/bin/bash
sendcmd()
{
	echo cmd:$1>/Robot/cmd/cmd_of_gui
}
GetKey()
{
	aKey=(0 0 0)
        cESC=`echo -ne "\033"`  #注意，这不是单引号，是ESC下面那个，里面的指令会先执行，并将结果赋值
	while : #':'表示永远为真
	do
		read -s -n 1 key #读取按键，每次只能读取一个ASCII字符
		case $key in
			0) echo "stop the car"
			sendcmd 0
			;;
			1) echo "start the car"
			sendcmd 1
			;;
			w) echo "go"
			sendcmd w
			;;
			s) echo "back"
			sendcmd s
			;;
			a) echo "turn left"
			sendcmd a
			;;
			d) echo "turn right"
			sendcmd d
			;;
		
		esac
		

    	        aKey[0]=${aKey[1]}
		aKey[1]=${aKey[2]}
		aKey[2]=$key


		if [[ ${aKey[0]} == $cESC && ${aKey[1]} == "[" ]]
		then
			if [[ $key == "A" ]];then sendcmd w#方向键判断
			elif [[ $key == "B" ]];then sendcmd s
			elif [[ $key == "D" ]];then sendcmd a
			elif [[ $key == "C" ]];then sendcmd d
			fi

			if [[ $key == "A" ]];then echo KEYUP #方向键判断
			elif [[ $key == "B" ]];then echo KEYDOWN
			elif [[ $key == "D" ]];then echo KEYLEFT
			elif [[ $key == "C" ]];then echo KEYRIGHT
			fi
		fi

	done
}

GetKey
