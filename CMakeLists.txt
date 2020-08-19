cmake_minimum_required(VERSION 3.12)

project(savor-tests)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR atmega328p)
set(CMAKE_CROSSCOMPILING 1)
set(SERIAL_BAUD 38400)

set(CMAKE_C_COMPILER avr-gcc)
set(SOURCES src/main.c)

set(CMAKE_C_FLAGS "-g -Wall -mcall-prologues -mmcu=${CMAKE_SYSTEM_PROCESSOR} -Os")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-gc-sections -Wl,-relax")

set(CMAKE_C_OBJCOPY avr-objcopy CACHE PATH "" FORCE)

add_subdirectory(lib/savor)

function(add_test TEST_NAME SOURCE_NAME)
	add_executable(${TEST_NAME}.elf ${SOURCE_NAME} ${ARGN})
	set_target_properties(${TEST_NAME}.elf PROPERTIES COMPILE_DEFINITIONS BAUD=${SERIAL_BAUD})
	target_link_libraries(${TEST_NAME}.elf savor)
	add_custom_target(${TEST_NAME}-hex ALL DEPENDS ${TEST_NAME}.hex)
	add_custom_command(OUTPUT ${TEST_NAME}.hex COMMAND ${CMAKE_C_OBJCOPY} ARGS -R .eeprom -O ihex ${TEST_NAME}.elf ${TEST_NAME}.hex DEPENDS ${TEST_NAME}.elf )
	add_custom_target(${TEST_NAME}-flash DEPENDS ${TEST_NAME}.hex
		COMMAND ${CMAKE_COMMAND} -E env bash -c "avrdude -c stk500v2 -P $(pavr2cmd --prog-port) -p ${CMAKE_SYSTEM_PROCESSOR} -U flash:w:${TEST_NAME}.hex:i" 
		VERBATIM)
endfunction()

add_test(test-serial src/test-serial.c)

add_custom_target(serial
	COMMAND ${CMAKE_COMMAND} -E env bash -c "stty -F $(pavr2cmd --ttl-port) raw ${SERIAL_BAUD}"
	COMMAND ${CMAKE_COMMAND} -E env bash -c "cat $(pavr2cmd --ttl-port)"
	VERBATIM)

