### YOU SHOULD EDIT THE BELOW WITH YOUR DETAILS AND FILES ###

set(user1 galklein)
set(user2 adamelhakham)
set(id1 "200902377")
set(id2 "201390085")

set (SOURCE_FILES
	battle.h
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
)

### DO NOT CHANGE FROM HERE ON ###

message("user: ${user1}, id: ${id1}")
message("user: ${user2}, id: ${id2}")
message("user: ${user3}, id: ${id3}")

set(EXE_NAME "ex1.${user1}")
add_executable( ${EXE_NAME}
	SOURCE_FILES
)

set_property(TARGET ${EXE_NAME} PROPERTY FOLDER "ex1/${user1}")
set_property(TARGET ${EXE_NAME} PROPERTY OUTPUT_NAME "ex1.${user1})

install (TARGETS ${EXE_NAME} DESTINATION ${INSTALL_BINARY_DIR})
