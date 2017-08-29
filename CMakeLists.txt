cmake_minimum_required(VERSION 3.8)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES MidiEvent.cpp MidiEventList.cpp MidiFile.cpp MidiMessage.cpp Binasc.cpp)


add_library(midifile_sapp ${SOURCE_FILES})

include_directories( "include" )

add_executable( create_midifile_example main.cpp ${SOURCE_FILES} )