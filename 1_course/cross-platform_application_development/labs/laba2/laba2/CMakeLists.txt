cmake_minimum_required(VERSION 3.22)

# ------------------------------------------------------------
#  Проект
# ------------------------------------------------------------
project(BioRhythm
        VERSION 1.0
        DESCRIPTION "Qt‑based biorhythm plotter"
        HOMEPAGE_URL "https://example.com/biorhythm"
        LANGUAGES CXX)

# Автоматически генерируем moc/uic/rcc‑файлы
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Qt6_DIR "D:/programs/Qt/6.8.2/msvc2022_64/lib/cmake/Qt6")
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# ------------------------------------------------------------
#  Qt модули (добавьте остальные при необходимости)
# ------------------------------------------------------------
find_package(Qt6 REQUIRED COMPONENTS
             Widgets
             Gui
             Core)

# ------------------------------------------------------------
#  Исходники программы
# ------------------------------------------------------------
add_executable(biorhythm
    main.cpp
    mainwindow.cpp        mainwindow.h
    date.cpp              date.h
)

target_link_libraries(biorhythm PRIVATE
    Qt6::Widgets
    Qt6::Gui
    Qt6::Core
)

# ------------------------------------------------------------
#  Развёртывание (Windows, динамика)
# ------------------------------------------------------------
if (WIN32)
    # ► Создаём install‑script, который в момент &laquo;install&raquo; вызовет
    #   qt6_deploy_runtime_dependencies / windeployqt.
    qt_generate_deploy_app_script(
        TARGET                biorhythm
        OUTPUT_SCRIPT         deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR   # молча игнорировать non‑Win
    )
endif()

# ------------------------------------------------------------
#  Установка
# ------------------------------------------------------------
include(GNUInstallDirs)

# 1) Сам исполняемый файл (.exe)
install(TARGETS biorhythm
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# 2) Qt‑DLL, плагины, crt – автоматически через сгенерированный скрипт
if (WIN32)
    install(SCRIPT ${deploy_script})
endif()

# ------------------------------------------------------------
#  Доп. опции (необязательно)
# ------------------------------------------------------------
# Укладывать pdb-файлы рядом с exe в Release‑конфигурации
if (MSVC AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_INSTALL_DEBUG_LIBRARIES TRUE)
    set(CMAKE_INSTALL_DEBUG_EXECUTABLE TRUE)
endif()
