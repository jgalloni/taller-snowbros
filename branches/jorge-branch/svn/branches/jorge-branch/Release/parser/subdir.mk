################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../parser/jsoncpp.cpp \
../parser/parser.cpp 

OBJS += \
./parser/jsoncpp.o \
./parser/parser.o 

CPP_DEPS += \
./parser/jsoncpp.d \
./parser/parser.d 


# Each subdirectory must supply rules for building sources it contributes
parser/%.o: ../parser/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


