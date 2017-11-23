################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/cropOpGen.cpp \
../src/cropSeriesClass.cpp \
../src/farminitclassC5.cpp \
../src/fieldOp.cpp \
../src/fieldOpCut.cpp \
../src/fieldOpFert.cpp \
../src/fieldOpGrass.cpp \
../src/fieldOpSow.cpp \
../src/fieldOpTill.cpp \
../src/fieldopharv.cpp 

OBJS += \
./src/cropOpGen.o \
./src/cropSeriesClass.o \
./src/farminitclassC5.o \
./src/fieldOp.o \
./src/fieldOpCut.o \
./src/fieldOpFert.o \
./src/fieldOpGrass.o \
./src/fieldOpSow.o \
./src/fieldOpTill.o \
./src/fieldopharv.o 

CPP_DEPS += \
./src/cropOpGen.d \
./src/cropSeriesClass.d \
./src/farminitclassC5.d \
./src/fieldOp.d \
./src/fieldOpCut.d \
./src/fieldOpFert.d \
./src/fieldOpGrass.d \
./src/fieldOpSow.d \
./src/fieldOpTill.d \
./src/fieldopharv.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -D__ANSICPP__ -D__WINDOWS__ -DCYGWIN -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/cropSeriesClass.o: ../src/cropSeriesClass.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -D__ANSICPP__ -D__WINDOWS__ -DCYGWIN -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/cropSeriesClass.d" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


