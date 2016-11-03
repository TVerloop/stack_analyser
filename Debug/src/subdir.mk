################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/cgraphfile.cpp \
../src/cgraphnode.cpp \
../src/main.cpp \
../src/stackanalyser.cpp \
../src/sufile.cpp \
../src/sunode.cpp 

OBJS += \
./src/cgraphfile.o \
./src/cgraphnode.o \
./src/main.o \
./src/stackanalyser.o \
./src/sufile.o \
./src/sunode.o 

CPP_DEPS += \
./src/cgraphfile.d \
./src/cgraphnode.d \
./src/main.d \
./src/stackanalyser.d \
./src/sufile.d \
./src/sunode.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/tom/workspace/stack_analyser/inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


