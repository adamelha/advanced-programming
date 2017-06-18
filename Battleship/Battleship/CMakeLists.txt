#############################################################
### YOU SHOULD EDIT THE BELOW WITH YOUR DETAILS AND FILES ###
#############################################################

set(user1 galklein)
set(user2 adamelhakham)
set(id1 "200902377")
set(id2 "201390085")

# All source files needed to compile your exe
set (EXE_SOURCE_FILES
	battle.h
	battle_utils.h
	board.h
	cmd_parser.h
	config.h
	display.h
	error_class.h
	file_parser.h
	IBattleshipGameAlgo.h
	macros.h
	point.h
	ships.h
	status.h
	thread_manager.h
	battle.cpp
	battle_utils.cpp
	board.cpp
	cmd_parser.cpp
	debug_utils.cpp
	display.cpp
	error_class.cpp
	file_parser.cpp
	main.cpp
	ships.cpp
	point.cpp
	thread_manager.cpp
)
# All source files needed to compile your smart algorithm's .dll
set (SMART_ALGO_SOURCE_FILES
	battle_utils.h
	board.h
	cmd_parser.h
	error_class.h
	file_parser.h
	IBattleshipGameAlgo.h
	macros.h
	point.h
	rand_pool.h
	ships.h
	status.h
	smart_algo.h
	battle_utils.cpp
	board.cpp
	cmd_parser.cpp
	debug_utils.cpp
	error_class.cpp
	file_parser.cpp
	rand_pool.cpp
	ships.cpp
	smart_algo.cpp
)

##################################
### DO NOT CHANGE FROM HERE ON ###
##################################

message("user: ${user1}, id: ${id1}")

add_definitions(-DUNICODE -D_UNICODE)

set(EX ex3)

# Set exe build parameters
set(EXE_NAME "${EX}.${user1}")
add_executable( ${EXE_NAME}	${EXE_SOURCE_FILES})
set_property(TARGET ${EXE_NAME} PROPERTY FOLDER "${EX}/${user1}")
set_property(TARGET ${EXE_NAME} PROPERTY OUTPUT_NAME "${user1}")

# Set smart algorithm's .dll build parameters
set(ALG3_NAME "${EX}.${user1}.smart")
add_library(${ALG3_NAME} SHARED ${SMART_ALGO_SOURCE_FILES})
set_property(TARGET ${ALG3_NAME} PROPERTY FOLDER "${EX}/${user1}")
set_property(TARGET ${ALG3_NAME} PROPERTY OUTPUT_NAME "${user1}.smart")
target_compile_definitions(${ALG3_NAME} PRIVATE -DALGO_EXPORTS) # preprocessor flag saying we are the one compiling the header now