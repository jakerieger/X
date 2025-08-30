set(SANDBOX_SOURCES
    main.cpp
    SandboxApp.cpp
    SandboxApp.hpp
)

add_executable(Sandbox ${SANDBOX_SOURCES})

set_target_properties(Sandbox PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    FOLDER "Applications"
)

target_include_directories(Sandbox PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Sandbox PRIVATE Engine)

if (WIN32 AND CMAKE_GENERATOR MATCHES "Visual Studio")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)
endif ()

set(RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Assets)
if (EXISTS ${RESOURCES_DIR})
    add_custom_command(TARGET Sandbox POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${RESOURCES_DIR}
        $<TARGET_FILE_DIR:Sandbox>/Assets
        COMMENT "Copying assets to output directory"
    )
endif ()

if (WIN32)
    set_target_properties(Sandbox PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/Sandbox/$<CONFIG>"
    )

    add_custom_command(TARGET Sandbox POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:glfw>
        $<TARGET_FILE_DIR:Sandbox>
        COMMENT "Copying required DLLs"
    )
endif ()