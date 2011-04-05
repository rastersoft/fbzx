################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../cargador.c \
../characters.c \
../computer.c \
../emulator.c \
../menus.c \
../microdrive.c \
../sound.c \
../spk_ay.c \
../tape.c 

OBJS += \
./cargador.o \
./characters.o \
./computer.o \
./emulator.o \
./menus.o \
./microdrive.o \
./sound.o \
./spk_ay.o \
./tape.o 

C_DEPS += \
./cargador.d \
./characters.d \
./computer.d \
./emulator.d \
./menus.d \
./microdrive.d \
./sound.d \
./spk_ay.d \
./tape.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


