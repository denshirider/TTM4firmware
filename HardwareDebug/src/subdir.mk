################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/E2ROM.c \
../src/I2C.c \
../src/MCP23017.c \
../src/SevenSegment.c \
../src/interface.c \
../src/queue.c 

CPP_SRCS += \
../src/TTM4firmware.cpp 

COMPILER_OBJS += \
src/E2ROM.obj \
src/I2C.obj \
src/MCP23017.obj \
src/SevenSegment.obj \
src/TTM4firmware.obj \
src/interface.obj \
src/queue.obj 

C_DEPS += \
src/E2ROM.d \
src/I2C.d \
src/MCP23017.d \
src/SevenSegment.d \
src/interface.d \
src/queue.d 

CPP_DEPS += \
src/TTM4firmware.d 

# Each subdirectory must supply rules for building sources it contributes
src/%.obj: ../src/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\cDepSubCommand.tmp"
	ccrx -subcommand="src\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" "$<"
	@echo src\cSubCommand.tmp=
	@sed -e "s/^/ /" "src\cSubCommand.tmp"
	ccrx -subcommand="src\cSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

src/%.obj: ../src/%.cpp 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\cppDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\cppDepSubCommand.tmp"
	ccrx -subcommand="src\cppDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" "$<"
	@echo src\cppSubCommand.tmp=
	@sed -e "s/^/ /" "src\cppSubCommand.tmp"
	ccrx -subcommand="src\cppSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

