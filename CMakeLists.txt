#### CMake 版本
cmake_minimum_required(VERSION 3.14)

### 用于生成 .lib 
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)



#set(CMAKE_BUILD_TYPE Debug)

############################################ 参数设置区 ########################################
#### 当前项目的版本号
set(PROJECT_VERSION 0.1)


###################### 必改项： 库名称 库名称宏 ##############
#### 配置修改 项目/库名称 
set(THISPROJECTNAME CommunicationModule)

#### 库名称宏 命名取决于 文件 xxx.global, 用于区分不同库的导出和导入
#if defined(TEST11_LIBRARY)
#define TEST11_EXPORT Q_DECL_EXPORT
#else
#define TEST11_EXPORT Q_DECL_IMPORT
#endif

#### 库名称宏
set(THIS_LIB_NAME COMMUNICATIONMODULE_LIBRARY)


###################### 必改项： 文件输出路径  ##############
#### 库文件输出路径
# 设置输出路径
# 根据操作系统类型设置库和运行时输出路径
if (UNIX)
    ##  linux 库文件输出路径
    set(LINUX_LIBRARY_OUTPUT_PATH_ROOT "/home/user01/CommunicationModule")

    ## 当前工程所在文件夹，默认为当前CMakeLiat 所在目录
    set(PROJECT_CURRENT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
    ## 
else (WIN32)
    ## 设置Windows库的输出路径和包含路径
    set(WIN_LIBRARY_OUTPUT_PATH__ROOT "D:/Soft/AtamiLib/CommunicationModule")

     ## 当前工程所在文件夹，默认为当前CMakeLiat 所在目录
    set(PROJECT_CURRENT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
    set(PROJECT_CURRENT_PATH_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()
## 路径显示
message("WIN_PROJECT_CURRENT_PATH : " ${PROJECT_CURRENT_PATH})


###################### 必改项： 外部库导入  ##################

##### 是否引用 QT 
set(QT_OPEN true)
##### QT 版本 5
set(QT_VERSION_5 true)

#### QT 版本 6
set(QT_VERSION_6 false)

#### QT 核心模块
set(THIS_QT_CORE Core Widgets Network Gui Concurrent)

#### QT5 核心库
if(QT_VERSION_5)
    set(THIS_QT_CORE_LIB Qt5::Core Qt5::Widgets Qt5::Network Qt5::Gui Qt5::Concurrent)
endif()

#### QT6 核心库
if(QT_VERSION_6)
    set(THIS_QT_CORE_LIB Qt6::Core)
endif()


#### 其余外部库
## 设置是否加载外部库 若为空，设置为 false
set(ADD_OTHER false)

if(ADD_OTHER)
    set(ADD_OTHER_LIB xxx.lib)
endif()


###################### 可选项： 指定模式  ##############
# 自动收集模式
set(AUTOFINDFILE true)

# 非自动收集时，需要手动指定
if(AUTOFINDFILE)
    #开启自动收集
else()
    set(HEADERFILE include/Test11_global.h include/Test11.h)
    set(SOURCEFILE source/Test11.cpp)
endif()







######################################### 功能区 #################################################

#### 定义一个CMake项目，项目名称为Test11，项目版本为0.1，使用的编程语言为C++
project(${THIS_LIB_NAME} VERSION ${PROJECT_VERSION} LANGUAGES CXX)
add_definitions(-DQSCINTILLA_DLL)
##  判断是否为debug模式
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug Mode")
    set(DEBUG_MODE ON)
else()
    message(STATUS "Release Mode")
    set(DEBUG_MODE OFF)
endif()

# 设置CMake自动处理Qt的UI、MOC和RCC文件
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 设置C++标准为C++17，并要求严格遵循标准，禁用编译器扩展
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

if(QT_OPEN)
    # 查找Qt软件包，首先查找Qt6，然后是Qt5，必须包含Core组件
    find_package(QT NAMES  Qt5 REQUIRED COMPONENTS ${THIS_QT_CORE})
    
    # 根据查找到的Qt版本查找对应版本的Core组件
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${THIS_QT_CORE})
endif()


### 自动收集 .h .cpp
if(AUTOFINDFILE)
    file(GLOB AUTOHEADERSFILE RELATIVE ${PROJECT_CURRENT_PATH} "**/*.h")
    file(GLOB AUTOSOURCEFILE RELATIVE ${PROJECT_CURRENT_PATH} "**/*.cpp")
    set(HEADERFILE ${AUTOHEADERSFILE})
    set(SOURCEFILE ${AUTOSOURCEFILE}) 
endif()

message("HEADERFILE : " ${HEADERFILE})
set(PROJECT_SOURCES
   ${HEADERFILE}
   ${SOURCEFILE}
)
# 设置编译器路径
# 添加共享库 Test11，并指定源文件
add_library(${THISPROJECTNAME} SHARED ${PROJECT_SOURCES})

 # 链接 Qt 核心库到 Test11
if(QT_OPEN)
    target_link_libraries(${THISPROJECTNAME} PRIVATE 
    ${THIS_QT_CORE_LIB}
    )
 endif()

 # 链接其他库
if(ADD_OTHER)
    target_link_libraries(${THISPROJECTNAME} PRIVATE ${ADD_OTHER_LIB})
endif()

target_compile_definitions(${THISPROJECTNAME} PRIVATE ${THIS_LIB_NAME})

# 设置输出属性

if (UNIX)
    ##  linux 库文件输出路径
    if(DEBUG_MODE)
        ## 调试模式
        set(LINUX_LIBRARY_OUTPUT_PATH ${LINUX_LIBRARY_OUTPUT_PATH_ROOT}/Lib/Debug)
    else()
        ## 发布模式
        set(LINUX_LIBRARY_OUTPUT_PATH ${LINUX_LIBRARY_OUTPUT_PATH_ROOT}/Lib/Release)
    endif()

    ##  linux .h 文件输出路径
    set(LINUX_LIBRARY_INCLUDE_PATH ${LINUX_LIBRARY_OUTPUT_PATH_ROOT}/Include)

    ## 运行目录输出
    set(LINUX_RUNTIME_DIRECTORY ${LINUX_LIBRARY_OUTPUT_PATH_ROOT})
     ## 路径文件创建
    make_directory(${LINUX_LIBRARY_OUTPUT_PATH})
    make_directory(${LINUX_LIBRARY_INCLUDE_PATH})
    file(COPY ${PROJECT_CURRENT_PATH_INCLUDE} DESTINATION ${LINUX_LIBRARY_INCLUDE_PATH})
    

else (WIN32)
    ## 设置Windows库的输出路径和包含路径

    if(DEBUG_MODE)
        ## 调试模式
        set(WIN_LIBRARY_OUTPUT_PATH ${WIN_LIBRARY_OUTPUT_PATH__ROOT}/Lib/Debug)
    else()
        ## 发布模式
        set(WIN_LIBRARY_OUTPUT_PATH ${WIN_LIBRARY_OUTPUT_PATH__ROOT}/Lib/Release)
    endif()

    ## 设置Windows库的包含路径
    set(WIN_LIBRARY_INCLUDE_PATH ${WIN_LIBRARY_OUTPUT_PATH__ROOT})
    ## 运行目录输出
    set(WIN_RUNTIME_DIRECTORY ${WIN_LIBRARY_OUTPUT_PATH__ROOT})
    ## 路径文件创建
    make_directory(${WIN_LIBRARY_OUTPUT_PATH})
    make_directory(${WIN_LIBRARY_INCLUDE_PATH})
endif()



# 路径文件创建
#make_directory(${WIN_LIBRARY_INCLUDE_PATH})

################################################## 安装 ##########################################
#“安装”通常指的是将编译或构建过程中生成的文件（如可执行文件、库文件、配置文件等）复制到特定的目录
################################################## 头文件 安装 ###################################
if(WIN32)
    install(DIRECTORY ${PROJECT_CURRENT_PATH_INCLUDE} DESTINATION ${WIN_LIBRARY_INCLUDE_PATH})
else(UNIX)
    install(DIRECTORY ${PROJECT_CURRENT_PATH_INCLUDE} DESTINATION ${LINUX_LIBRARY_OUTPUT_PATH})
endif()

################################################## 库文件 安装 ###################################
if (UNIX)
    if(DEBUG_MODE)
    # 设置输出属性
        set_target_properties(${THISPROJECTNAME} PROPERTIES
            OUTPUT_NAME "${THISPROJECTNAME}"
            SUFFIX "_debug.so"
            #LIBRARY_OUTPUT_DIRECTORY ${LINUX_LIBRARY_OUTPUT_PATH}
            #RUNTIME_OUTPUT_DIRECTORY ${LINUX_LIBRARY_OUTPUT_PATH}
            make_directory(${LINUX_LIBRARY_INCLUDE_PATH})
            install(DIRECTORY ${PROJECT_CURRENT_PATH_INCLUDE} DESTINATION ${LINUX_LIBRARY_INCLUDE_PATH})
        )
        install(TARGETS ${THISPROJECTNAME}
            EXPORT ${THIS_LIB_NAME}
            ARCHIVE DESTINATION ${LINUX_LIBRARY_OUTPUT_PATH}
            LIBRARY DESTINATION ${LINUX_LIBRARY_OUTPUT_PATH}
            RUNTIME DESTINATION ${LINUX_LIBRARY_OUTPUT_PATH})
    else()
            # 设置输出属性
            set_target_properties(${THISPROJECTNAME} PROPERTIES
                OUTPUT_NAME "${THISPROJECTNAME}"
                SUFFIX "_release.so"
                LIBRARY_OUTPUT_DIRECTORY ${LINUX_LIBRARY_OUTPUT_PATH}
                RUNTIME_OUTPUT_DIRECTORY ${LINUX_LIBRARY_OUTPUT_PATH}
                make_directory(${LINUX_LIBRARY_INCLUDE_PATH})
                 install(DIRECTORY ${PROJECT_CURRENT_PATH_INCLUDE} DESTINATION ${LINUX_LIBRARY_INCLUDE_PATH})
            )
            install(TARGETS ${THISPROJECTNAME}
                    EXPORT ${THIS_LIB_NAME}
                    ARCHIVE DESTINATION ${LINUX_LIBRARY_OUTPUT_PATH}
                    LIBRARY DESTINATION ${LINUX_LIBRARY_OUTPUT_PATH}
                    RUNTIME DESTINATION ${LINUX_LIBRARY_OUTPUT_PATH})
   endif()


else (WIN32)
    if(DEBUG_MODE)
        ### 调试模式 修改后缀
        set_target_properties(${THISPROJECTNAME} PROPERTIES OUTPUT_NAME "${THISPROJECTNAME}" SUFFIX "_debug.dll")
        
        # 安装到输出目录
        install(TARGETS ${THISPROJECTNAME}
                EXPORT ${THIS_LIB_NAME}
                ARCHIVE DESTINATION ${WIN_LIBRARY_OUTPUT_PATH}
                LIBRARY DESTINATION ${WIN_LIBRARY_OUTPUT_PATH}
                RUNTIME DESTINATION ${WIN_LIBRARY_OUTPUT_PATH})
    else()

        ### 发布模式 修改后缀
        set_target_properties(${THISPROJECTNAME} PROPERTIES OUTPUT_NAME "${THISPROJECTNAME}" SUFFIX "_release.dll")
        
        # TARGETS   ${THISPROJECTNAME}： 指定要安装的目标文件，${THISPROJECTNAME}是目标的名称。
        # EXPORT    ${THIS_LIB_NAME}：   导出目标的安装信息，${THIS_LIB_NAME}是导出的库名称。
        # ARCHIVE   DESTINATION         ${WIN_LIBRARY_OUTPUT_PATH}：指定静态库文件的安装路径，${WIN_LIBRARY_OUTPUT_PATH}是路径变量。
        # LIBRARY   DESTINATION         ${WIN_LIBRARY_OUTPUT_PATH}：指定共享库文件的安装路径，${WIN_LIBRARY_OUTPUT_PATH}是路径变量。
        # RUNTIME   DESTINATION          ${WIN_LIBRARY_OUTPUT_PATH}：指定可执行文件的安装路径，${WIN_LIBRARY_OUTPUT_PATH}是路径变量。
        install(TARGETS ${THISPROJECTNAME}
                EXPORT ${THIS_LIB_NAME}
                ARCHIVE DESTINATION ${WIN_LIBRARY_OUTPUT_PATH}
                LIBRARY DESTINATION ${WIN_LIBRARY_OUTPUT_PATH}
                RUNTIME DESTINATION ${WIN_LIBRARY_OUTPUT_PATH})

    # 结束
    endif()
endif()

# 设置宏定义
if(BUILD_SHARED_LIBS)
    target_compile_definitions(${THISPROJECTNAME} PUBLIC ${THIS_LIB_NAME})
endif()
