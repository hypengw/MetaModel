cmake_minimum_required(VERSION 3.28)

project(
  MetaModel
  VERSION 0.1.1
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(META_MODEL_BUILD_TESTS "Build tests" ${PROJECT_IS_TOP_LEVEL})

find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(
  meta_model STATIC src/qmetaobjectmodel.cpp src/qtable_proxy_model.cpp
                    src/moc.cpp src/share_store.cpp)
add_library(meta_model::meta_model ALIAS meta_model)

target_compile_features(meta_model PRIVATE cxx_std_20)
set_target_properties(meta_model PROPERTIES AUTOMOC ON)
target_include_directories(meta_model PUBLIC include)
target_link_libraries(meta_model PUBLIC Qt6::Core)

if(META_MODEL_BUILD_TESTS)
  include(CTest)
  enable_testing()
  add_subdirectory(test)
endif()
