cmake_minimum_required(VERSION 3.22)
project(TicketSystem_)

set(CMAKE_CXX_STANDARD 20)

include_directories(BPT)
include_directories(BPT/STLite)

add_executable(code
        main.cpp
        BPT/STLite/exceptions.h
        BPT/STLite/map.h
        BPT/STLite/priority_queue.h
        BPT/STLite/utility.h
        BPT/STLite/vector.h
        Management/TrainManagement.h
        Management/UserManagement.h
        Management/TicketManagement.h
        BasicInfo/User.h
        BasicInfo/Train.h
        BasicInfo/Ticket.h
        Tool/Time.h
        Tool/CommandParser.h
        Tool/MyChar.h
        BPT/BPLusTree.h
        BPT/MemoryRiver.h
        BPT/STLite/exceptions.h
        Tool/Error.h)
