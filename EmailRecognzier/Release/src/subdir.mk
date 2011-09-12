################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/EmailRecognzier.cpp 

CC_SRCS += \
../src/DataSet.cc \
../src/KnowledgeBase.cc \
../src/function.cc 

OBJS += \
./src/DataSet.o \
./src/EmailRecognzier.o \
./src/KnowledgeBase.o \
./src/function.o 

CC_DEPS += \
./src/DataSet.d \
./src/KnowledgeBase.d \
./src/function.d 

CPP_DEPS += \
./src/EmailRecognzier.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


