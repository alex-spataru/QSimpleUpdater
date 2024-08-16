project(QSU_Tutorial
    LANGUAGES CXX
)

add_executable(QSU_Tutorial WIN32 MACOSX_BUNDLE EXCLUDE_FROM_ALL
    src/Window.ui
    src/Window.h
    src/Window.cpp
    src/main.cpp
)
target_compile_definitions(QSimpleUpdater PUBLIC QSU_INCLUDE_MOC)
target_link_libraries(QSU_Tutorial QSimpleUpdater)
