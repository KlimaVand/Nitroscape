################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/tools/compare.cpp \
../src/tools/convertData.cpp \
../src/tools/fileAccess.cpp 

OBJS += \
./src/tools/compare.o \
./src/tools/convertData.o \
./src/tools/fileAccess.o 

CPP_DEPS += \
./src/tools/compare.d \
./src/tools/convertData.d \
./src/tools/fileAccess.d 


# Each subdirectory must supply rules for building sources it contributes
src/tools/%.o: ../src/tools/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -D__ANSICPP__ -D__WINDOWS__ -DCYGWIN -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


