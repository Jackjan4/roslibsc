target_sources(app PRIVATE
    src/display_gfx.c
    src/display_adapter.c)

target_include_directories(app PRIVATE
    src/include
    src/include/fonts/adafruit_gfx)
