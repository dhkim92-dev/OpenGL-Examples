
function(buildExample EXAMPLE_NAME)
    SET(EXAMPLE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/${EXAMPLE_NAME})
    MESSAGE(STATUS "Generate Example file for in ${EXAMPLE_FOLDER}")

    file(GLOB SOURCE *.cpp ${BASE_HEADERS} ${EXAMPLE_FOLTER}/*.cpp)
    SET(MAIN_CPP ${EXAMPLE_FOLDER}/main.cpp)

    SET(SHADER_DIR "../shaders/${EXAMPLE_NAME}")
    file(GLOB SHADERS "${SHADER_DIR}/*.vert" "${SHADER_DIR}/*.frag")

    source_group("SHADERS" FILES ${SHADERS})
    MESSAGE(STATUS "GLOB SHADER result : ${SHADERS}")

	INCLUDE_DIRECTORIES(../core ../external ${GLEX_ASSIMP_INCLUDE_DIRECTORY})
    find_package(glfw3)
    find_package(assimp)

    IF(APPLE)
        LINK_LIBRARIES(glfw assimp "-framework OpenGL")
    ELSEIF(UNIX)
        LINK_LIBRARIES(glfw GL assimp pthread)
    ENDIF()
   
    add_executable(${EXAMPLE_NAME} 
           ${MAIN_CPP} 
           ${SOURCE} 
           ${MAIN_HEADER} 
           ${SHADERS} )
    IF(APPLE)
        target_link_libraries(${EXAMPLE_NAME} glfw assimp glexlib ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY/${GLEX_CORE_LIB_NAME}})
    ELSEIF(UNIX)
        target_link_libraries(${EXAMPLE_NAME} glfw assimp glexlib GL ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY/${GLEX_CORE_LIB_NAME}})
    ENDIF()

    install(TARGETS ${EXAMPLE_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
endfunction(buildExample)

function(buildExamples)
    foreach(EXAMPLE ${EXAMPLES})
        buildExample(${EXAMPLE})
    endforeach(buildExamples)
endfunction(buildExamples)

SET(EXAMPLES 
    cube
    rotateCube
    textureCube
    eulerCamera
    quatCamera
    multipleCubes
    pixelBasedObjectPicking
    pickedSilhouette
    eulerObjectRotation
    modelLoading
)

buildExamples()




