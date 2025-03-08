cmake_minimum_required(VERSION 3.12)

# Устанавливаем версию проекта
set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})
project(validator VERSION ${PROJECT_VERSION})

# Опция для сборки с Boost Test
option(WITH_BOOST_TEST "Whether to build Boost test" ON)

# Включаем языки C и C++
enable_language(C)
enable_language(CXX)

# Настройки Boost
set(Boost_USE_STATIC_LIBS ON)  # Статическая линковка Boost
set(Boost_USE_MULTITHREAD ON)  # Многопоточность
set(BOOST_ROOT "/usr")

# Поиск и подключение зависимостей
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Boost 1.58 REQUIRED COMPONENTS program_options system filesystem)

# Для поиска и использования статической версии libxml2
find_package(LibXml2 REQUIRED)

# Указываем статическую линковку для libxml2
set(LIBXML2_STATIC ON)  # Статическая линковка для libxml2

# Поиск и подключение yaml-cpp
# Добавляем флаг для статической линковки
find_package(yaml-cpp REQUIRED)

# Включаем поддержку статической линковки для yaml-cpp
set(YAML_CPP_STATIC ON)

# Добавление исполняемого файла
add_executable(validator
    main.cpp
    utils/utils.cpp
    dataformatter/JSONformatter.cpp
    dataformatter/XMLformatter.cpp
    dataformatter/YAMLformatter.cpp
)

# Подключение библиотек
target_link_libraries(validator
    nlohmann_json::nlohmann_json
    ${Boost_LIBRARIES}
    ${LIBXML2_LIBRARIES}
    yaml-cpp
)

# Настройки стандарта C++
set_target_properties(validator PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Включаем директории заголовков
target_include_directories(validator
    PRIVATE ${CMAKE_BINARY_DIR}
    PRIVATE ${nlohmann_json_INCLUDE_DIRS}
    PRIVATE ${Boost_INCLUDE_DIRS}
    PRIVATE ${LIBXML2_INCLUDE_DIR}
    PRIVATE ${YAML_CPP_INCLUDE_DIR}
)

# Настройки компиляции
if (MSVC)
    target_compile_options(validator PRIVATE /W4)
else()
    target_compile_options(validator PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

# Установка бинарника
install(TARGETS validator RUNTIME DESTINATION bin)

# Настройки для CPack (создание .deb пакета)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT nik2704@yandex.ru)

include(CPack)
