#!/bin/bash
FILE_DIR0=/dev/ttyUSB
FILE_DIR1=/dev/ttyACM

for ((i = 0; i <= 10; i++)); do
	if [ "$i" -le 5 ]; then
		if test -e "$FILE_DIR0$i"; then
			echo -e "\n\e[32m$FILE_DIR0$i exists.\e[0m\n"
			sudo chmod 777 "$FILE_DIR0$i"
			break
		fi
	else
		if test -e "$FILE_DIR1$i"; then
			echo -e "\n\e[32m$FILE_DIR1$i exists.\e[0m\n"
			sudo chmod 777 "$FILE_DIR1$i"
			break
		fi
	fi
	if [ "$i" -eq 10 ]; then
		echo -e "\e[31m\nThe device does not exist.
Please check if the device is disabled and if the driver is installed\e[0m"
		exit
	fi
done

if test -e "build"; then
	rm -r build
fi
mkdir build
cd build || exit
cmake ..
make -j4
chmod 777 car_vision_demo
./car_vision_demo
