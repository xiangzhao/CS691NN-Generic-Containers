################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AdaptiveKeyValuePairs.cpp \
../AdaptiveMultiSet.cpp \
../AdaptiveSequence.cpp \
../Main.cpp 

OBJS += \
./AdaptiveKeyValuePairs.o \
./AdaptiveMultiSet.o \
./AdaptiveSequence.o \
./Main.o 

CPP_DEPS += \
./AdaptiveKeyValuePairs.d \
./AdaptiveMultiSet.d \
./AdaptiveSequence.d \
./Main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


