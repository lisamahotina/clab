project(strategies)
set(SOURCES
		human.cpp
		computer1.cpp
		computer2.cpp
		computer_interface.cpp
		interface.cpp)
set(HEADERS
		human.h
		computer1.h
		computer2.h
		interface.h
		computer_interface.h)

include_directories(../)

add_library(strategies ${SOURCES} ${HEADERS})
