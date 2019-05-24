cmake_minimum_required(VERSION 2.8.12)
project(cw07 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-Wall -pthread -std=c99 -lrt")

add_executable(SystemV/loader.c utils/utils.c utils/utils.h SystemV/common.h SystemV/systemv.c SystemV/common.c SystemV/defines.h )
add_executable(SystemV/trucker.c utils/utils.c utils/utils.h SystemV/common.h SystemV/systemv.c SystemV/common.c SystemV/defines.h)

set(CMAKE_C_FLAGS "-Wall -pthread -std=gnu99 -lrt -D=POSIX")

add_executable(SystemV/loader.c utils/utils.c utils/utils.h SystemV/common.h SystemV/common.c SystemV/defines.h POSIX/posix.c)
add_executable(SystemV/trucker.c utils/utils.c utils/utils.h SystemV/common.h  SystemV/common.c SystemV/defines.h POSIX/posix.c)
