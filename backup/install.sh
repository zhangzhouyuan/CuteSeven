#!/bin/bash
echo "开始配置运行环境"
echo "请输入超级管理员密码"
sudo ls
install_base_soft()
{
	echo "开始安装基础软件"
	sudo apt-get install mplayer vim git	
	sudo apt-get install lame
	sudo apt-get install gcc-multilib g++-multilib
}
usb_serial_rules()
{
	echo "配置USB串口设备的权限"
	echo KERNEL==\"ttyUSB*\" , MODE = \"0666\" >>20-usb-serial.rules
	echo KERNEL==\"ttyS*\" , MODE = \"0666\"  >>20-usb-serial.rules
	sudo mv 20-usb-serial.rules /etc/udev/rules.d
}

main()
{
	#install_base_soft #安装基础软件
	usb_serial_rules
	echo "环境配置完成"

}
main
