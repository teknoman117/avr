add_library(FreeRTOS STATIC
    FreeRTOS-Kernel/portable/ThirdParty/GCC/ATmega/port.c
    FreeRTOS-Kernel/portable/MemMang/heap_3.c
    FreeRTOS-Kernel/croutine.c
    FreeRTOS-Kernel/event_groups.c
    FreeRTOS-Kernel/list.c
    FreeRTOS-Kernel/queue.c
    FreeRTOS-Kernel/stream_buffer.c
    FreeRTOS-Kernel/tasks.c
    FreeRTOS-Kernel/timers.c
)

target_include_directories(FreeRTOS
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel/portable/ThirdParty/GCC/ATmega>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
)

target_compile_options(FreeRTOS PRIVATE $<$<C_COMPILER_ID:GNU>:-Wno-cpp>)
