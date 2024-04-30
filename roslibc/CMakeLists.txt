target_sources(app PRIVATE
    src/ring_array.c
    src/mathutils.c
    src/stringutils.c
    src/fixed32_t.c)

target_include_directories(app PRIVATE
    src/include)
