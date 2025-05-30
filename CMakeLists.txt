cmake_minimum_required(VERSION 3.22)
project(TicketSys)

set(CMAKE_CXX_STANDARD 14)

add_executable(code
        main.cpp
        STLite/vector.h
        BPT/BPlusTree.h
        BPT/MemoryRiver.h
        STLite/map.h
        STLite/utility.h
        STLite/exceptions.h
        STLite/vector.h
        Tool/Error.h
        Tool/MyChar.h
        Tool/CommandParser.h
        Tool/Time.h
        Management/UserManagement.cpp
        Management/UserManagement.h
        BasicInfo/User.h
        BasicInfo/Train.h
        BasicInfo/Ticket.h
        Management/TrainManagement.cpp
        Management/TrainManagement.h
        Management/TicketManagement.cpp
        Management/TicketManagement.h
        STLite/priority_queue.h
        Tool/Time.cpp
)
