################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/DataSet.cc \
../src/EmailRecognizer.cc \
../src/KnowledgeBase.cc \
../src/function.cc 

OBJS += \
./src/DataSet.o \
./src/EmailRecognizer.o \
./src/KnowledgeBase.o \
./src/function.o 

CC_DEPS += \
./src/DataSet.d \
./src/EmailRecognizer.d \
./src/KnowledgeBase.d \
./src/function.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


