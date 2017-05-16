set(user1 galklein)
set(user2 adamelhakham)
set(id1 "200902377")
set(id2 "201390085")
set (SOURCE_FILES
	battle.cpp
	board.h
	board.cpp
	cmd_parser.h
	cmd_parser.cpp
	error_class.h
	error_class.cpp
	file_parser.h
	file_parser.cpp
	ships.h
	ships.cpp	
	IBattleshipGameAlgo.h
	macros.h
	point.h
	status.h
	main.cpp
	display.h
	display.cpp
	battle_utils.h
	battle_utils.cpp
	file_algo.h
	file_algo.cpp
	naive_algo.h
	naive_algo.cpp
	
)
add_definitions(-DUNICODE -D_UNICODE)

### DO NOT CHANGE FROM HERE ON ###

message("user: ${user1}, id: ${id1}")
message("user: ${user2}, id: ${id2}")
message("user: ${user3}, id: ${id3}")

set(EXE_NAME "ex1.${user1}")
add_executable( ${EXE_NAME}
	${SOURCE_FILES}
)
#BUILD_BINARY_OUTPUT_DIRECTORY
set_property(TARGET ${EXE_NAME} PROPERTY FOLDER "ex1/${user1}")
set_property(TARGET ${EXE_NAME} PROPERTY OUTPUT_NAME "${user1}")