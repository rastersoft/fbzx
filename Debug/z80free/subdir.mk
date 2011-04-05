################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../z80free/Z80free.c \
../z80free/Z80free_codes.c \
../z80free/Z80free_codesCB.c \
../z80free/Z80free_codesDD.c \
../z80free/Z80free_codesDDCB.c \
../z80free/Z80free_codesED.c \
../z80free/Z80free_codesFD.c \
../z80free/Z80free_codesFDCB.c \
../z80free/z80free_tester.c 

OBJS += \
./z80free/Z80free.o \
./z80free/Z80free_codes.o \
./z80free/Z80free_codesCB.o \
./z80free/Z80free_codesDD.o \
./z80free/Z80free_codesDDCB.o \
./z80free/Z80free_codesED.o \
./z80free/Z80free_codesFD.o \
./z80free/Z80free_codesFDCB.o \
./z80free/z80free_tester.o 

C_DEPS += \
./z80free/Z80free.d \
./z80free/Z80free_codes.d \
./z80free/Z80free_codesCB.d \
./z80free/Z80free_codesDD.d \
./z80free/Z80free_codesDDCB.d \
./z80free/Z80free_codesED.d \
./z80free/Z80free_codesFD.d \
./z80free/Z80free_codesFDCB.d \
./z80free/z80free_tester.d 


# Each subdirectory must supply rules for building sources it contributes
z80free/%.o: ../z80free/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


