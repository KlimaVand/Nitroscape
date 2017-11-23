################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/base/base.cpp \
../src/base/bstime.cpp \
../src/base/message.cpp \
../src/base/nixcbits.cpp \
../src/base/unix_util.cpp 

OBJS += \
./src/base/base.o \
./src/base/bstime.o \
./src/base/message.o \
./src/base/nixcbits.o \
./src/base/unix_util.o 

CPP_DEPS += \
./src/base/base.d \
./src/base/bstime.d \
./src/base/message.d \
./src/base/nixcbits.d \
./src/base/unix_util.d 


# Each subdirectory must supply rules for building sources it contributes
src/base/%.o: ../src/base/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -D__ANSICPP__ -D__WINDOWS__ -DCYGWIN -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


