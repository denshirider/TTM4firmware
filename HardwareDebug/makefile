################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include makefile.init

RM := rm -rf *.sni *.dbg *.lst *.lis *.lpp *.map *.x *.lbp *.bls *.libelf *.rel *.lbk *.abs *.mi  *.obj *.p *.pp *.d *.asm *.src 1st/*.obj 1st/*.d 1st/*.asm 1st/*.abs 1st/*.src 1st/*.p 1st/*.pp

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include generate/subdir.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif 
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif 
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif 
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif 
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif 
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif 
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif 
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif 
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif 
ifneq ($(strip $(JMP_DEPS)),)
-include $(JMP_DEPS)
endif 
ifneq ($(strip $(SRC_DEPS)),)
-include $(SRC_DEPS)
endif 
ifneq ($(strip $(PP_DEPS)),)
-include $(PP_DEPS)
endif 
ifneq ($(strip $(P_DEPS)),)
-include $(P_DEPS)
endif 
ifneq ($(strip $(FSY_DEPS)),)
-include $(FSY_DEPS)
endif 
endif

ifneq ($(strip $(SRC_UPPER_SRCS)),)
ALL_ASMS += $(SRC_UPPER_SRCS) 
endif
ifneq ($(strip $(SRC_SRCS)),)
ALL_ASMS += $(SRC_SRCS) 
endif
ifneq ($(strip $(S_UPPER_SRCS)),)
ALL_ASMS += $(S_UPPER_SRCS) 
endif
ifneq ($(strip $(S_SRCS)),)
ALL_ASMS += $(S_SRCS) 
endif


-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables
LINKER_OUTPUT_OUTPUTS += \
TTM4firmware.abs

LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS += \
TTM4firmware.lib

CONVERTOR_OUTPUT_OUTPUTS += \
TTM4firmware.mot

OBJS := \
$(COMPILER_OBJS) \
$(INDIVIDUAL_COMPILER_OBJS) \
$(ASSEMBLER_OBJS) \
$(CUSTOM_OBJS)

# All Target
all: $(CONVERTOR_OUTPUT_OUTPUTS) TTM4firmware.x
	@echo 'Build complete.'

TTM4firmware.mot: $(LINKER_OUTPUT_OUTPUTS) 
	@echo 'Invoking: Converter'
	@echo 'Building target:'
	@echo ConverterTTM4firmware.tmp=
	@sed -e "s/^/    /" "ConverterTTM4firmware.tmp"
	rlink   "TTM4firmware.abs" -subcommand="ConverterTTM4firmware.tmp"
	@echo 'Finished building target:'
	@echo.


TTM4firmware.abs: $(OBJS)  $(ALL_ASMS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) 
	@echo 'Invoking: Linker'
	@echo LinkerTTM4firmware.tmp=
	@sed -e "s/^/    /" "LinkerTTM4firmware.tmp"
	rlink -subcommand="LinkerTTM4firmware.tmp"
	@echo 'Finished building:'
	@echo.


TTM4firmware.lib: 
	@echo 'Invoking: Library Generator'
	@echo LibraryGeneratorTTM4firmware.tmp=
	@sed -e "s/^/    /" "LibraryGeneratorTTM4firmware.tmp"
	lbgrx -subcommand="LibraryGeneratorTTM4firmware.tmp"
	@echo 'Finished building:'
	@echo.

TTM4firmware.x: TTM4firmware.abs
		-C:\Renesas\e2_studio\Utilities\\ccrx\renesas_cc_converter.exe TTM4firmware.abs TTM4firmware.x

# Other Targets
clean:
	-$(RM) $(LINKER_OUTPUT_OUTPUTS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) $(CONVERTOR_OUTPUT_OUTPUTS)
	-rm -rf src/*.obj src/*.d src/*.asm src/*.p src/*.src src/*.jmp src/*.lst src/*.prn src/*.i 
	-rm -rf generate/*.obj generate/*.d generate/*.asm generate/*.p generate/*.src generate/*.jmp generate/*.lst generate/*.prn generate/*.i 
	@echo
	@echo 'Clean complete.'

.PHONY: all clean 
-include ../makefile.targets