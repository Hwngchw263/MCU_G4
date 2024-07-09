################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../middleware/Middleware.c 

OBJS += \
./middleware/Middleware.o 

C_DEPS += \
./middleware/Middleware.d 


# Each subdirectory must supply rules for building sources it contributes
middleware/%.o: ../middleware/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@middleware/Middleware.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


