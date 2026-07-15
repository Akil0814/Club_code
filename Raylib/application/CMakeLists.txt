file(GLOB_RECURSE APP CONFIGURE_DEPENDS ./*.cpp)

add_library(app_lib ${APP})
target_link_libraries(app_lib PUBLIC raylib)
