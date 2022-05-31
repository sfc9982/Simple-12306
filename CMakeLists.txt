cmake_minimum_required(VERSION 3.22)
project(Simple_12306 C)

set(CMAKE_C_STANDARD 23)

include_directories(TicketManagementSystem)

add_executable(Simple_12306
        TicketManagementSystem/admin_user.c
        TicketManagementSystem/manage_records.c
        TicketManagementSystem/manage_tickets.c
        TicketManagementSystem/query.c
        TicketManagementSystem/salesman.c
        TicketManagementSystem/statistics.c
        TicketManagementSystem/stdafx.c
        TicketManagementSystem/stdafx.h
        TicketManagementSystem/targetver.h
        TicketManagementSystem/tickets.h
        TicketManagementSystem/TicketsManagementSystem.c
        TicketManagementSystem/uthash.h)
