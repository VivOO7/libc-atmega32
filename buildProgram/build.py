
import os
import sys

dir=os.listdir(os.getcwd())

mcu=input("Enter the mcu name :")
mcuc=input("Enter the mcu code :")
freq=input("Enter the cpu frequency :")


for file in dir:
	if file.endswith('.c'):
		os.system("avr-gcc -g -Os -mmcu={} -DF_CPU={} -c {}".format(mcu,freq,file))


dir=os.listdir(os.getcwd())

buildCommand="avr-gcc -g -mmcu={} -o main.elf".format(mcu)

for file in dir:
	if file.endswith('.o'):
		buildCommand=buildCommand+" "+file

os.system(buildCommand)

os.system("avr-objcopy -j .text -j .data -O ihex main.elf main.hex")

dir=os.listdir(os.getcwd())

for file in dir:
	if file.endswith('.o') or file.endswith('.elf'):
		os.system("rm {}".format(file))

os.system("avrdude -c usbasp -p {} -U flash:w:main.hex".format(mcuc))

os.system("rm main.hex")

print("DONE")
