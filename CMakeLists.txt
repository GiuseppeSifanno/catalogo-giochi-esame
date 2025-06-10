cmake_minimum_required(VERSION 3.31)
project(catalogo_giochi_esame C)

set(CMAKE_C_STANDARD 11)

add_executable(catalogo_giochi_esame catalogo.c
        catalogolib.c
        catalogolib.h
        gioco.h
        utility.c
        utility.h
        gioco.c
)

add_executable(test_unity
        tunity_catalogo.c
        Unity-2.6.1/src/unity.h
        Unity-2.6.1/src/unity_internals.h
        Unity-2.6.1/src/unity.c
        catalogolib.h
        catalogolib.c
        gioco.h
        gioco.c
        utility.h
        utility.c
)