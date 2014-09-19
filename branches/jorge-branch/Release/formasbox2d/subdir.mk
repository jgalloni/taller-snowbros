################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../formasbox2d/parametros.cpp \
../formasbox2d/shapes.cpp 

OBJS += \
./formasbox2d/parametros.o \
./formasbox2d/shapes.o 

CPP_DEPS += \
./formasbox2d/parametros.d \
./formasbox2d/shapes.d 


# Each subdirectory must supply rules for building sources it contributes
formasbox2d/%.o: ../formasbox2d/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


