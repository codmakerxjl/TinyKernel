cmake_minimum_required(VERSION 3.10)
project(tinyKernel)

# 设置编译器和链接器
set(ASM nasm)
set(CC i686-elf-gcc)
set(LD i686-elf-gcc)

# 打印 CMAKE_SOURCE_DIR
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")

# 设置编译和链接选项
set(ASM_FLAGS -f elf32 -g -F dwarf)
set(C_FLAGS -std=c99 -g -m32 -O3 -c -fno-exceptions -nostdlib -ffreestanding -Wall -Wextra -pedantic)
set(LD_FLAGS -m32 -T ${CMAKE_SOURCE_DIR}/arch/i386/linker.ld -ffreestanding -nostdlib)
set(LIBS -lgcc)

file(GLOB INCLUDE_DIRS 
    "${CMAKE_SOURCE_DIR}/include/*"
    "${CMAKE_SOURCE_DIR}/include/*/*"

)

# 设置包含目录
set(INCLUDE_FLAGS "")
foreach(dir  ${INCLUDE_DIRS})
    list(APPEND INCLUDE_FLAGS "-I${dir}")
endforeach()


# 查找源文件
file(GLOB ASM_SRC
    "${CMAKE_SOURCE_DIR}/*/*/*/*.asm"
    "${CMAKE_SOURCE_DIR}/*/*/*.asm"
    
)

file(GLOB C_SRC
    "${CMAKE_SOURCE_DIR}/*/*.c"
    "${CMAKE_SOURCE_DIR}/*/*/*.c"
)

# 创建对象文件列表
set(OBJ_FILES "")
foreach(src ${C_SRC})
    get_filename_component(name ${src} NAME_WE)
    set(OBJ_FILE "${CMAKE_CURRENT_BINARY_DIR}/${name}.o")
    list(APPEND OBJ_FILES ${OBJ_FILE})

    # 编译 C 源文件
    add_custom_command(
        OUTPUT ${OBJ_FILE}
        COMMAND ${CC} ${C_FLAGS} ${INCLUDE_FLAGS} -o ${OBJ_FILE} ${src}
        DEPENDS ${src}
        COMMENT "Compiling ${src}..."
    )
endforeach()

foreach(src ${ASM_SRC})
    get_filename_component(name ${src} NAME_WE)
    set(OBJ_FILE "${CMAKE_CURRENT_BINARY_DIR}/${name}.o")
    list(APPEND OBJ_FILES ${OBJ_FILE})

    # 编译 ASM 源文件
    add_custom_command(
        OUTPUT ${OBJ_FILE}
        COMMAND ${ASM} ${ASM_FLAGS} -I${INCLUDE_FLAGS} -o ${OBJ_FILE} ${src}
        DEPENDS ${src}
        COMMENT "Assembling ${src}..."
    )
endforeach()



add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/build/myos.bin
    COMMAND ${LD} ${LD_FLAGS} -o ${CMAKE_SOURCE_DIR}/build/myos.bin ${OBJ_FILES} ${LIBS}
    DEPENDS ${OBJ_FILES}
    COMMENT "Linking myos.bin..."
)

add_custom_target(myos ALL DEPENDS ${CMAKE_SOURCE_DIR}/build/myos.bin)


# 生成最终目标文件

# 清理目标
set(CMAKE_CLEAN_FILES ${CMAKE_SOURCE_DIR}/build/myos.bin)