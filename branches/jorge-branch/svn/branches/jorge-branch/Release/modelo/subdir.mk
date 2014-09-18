################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../modelo/Objeto.cpp \
../modelo/Personaje.cpp 

OBJS += \
./modelo/Objeto.o \
./modelo/Personaje.o 

CPP_DEPS += \
./modelo/Objeto.d \
./modelo/Personaje.d 


# Each subdirectory must supply rules for building sources it contributes
modelo/%.o: ../modelo/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


