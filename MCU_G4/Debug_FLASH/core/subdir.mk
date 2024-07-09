################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../core/Core_NVIC.c 

OBJS += \
./core/Core_NVIC.o 

C_DEPS += \
./core/Core_NVIC.d 


# Each subdirectory must supply rules for building sources it contributes
core/%.o: ../core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@core/Core_NVIC.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


