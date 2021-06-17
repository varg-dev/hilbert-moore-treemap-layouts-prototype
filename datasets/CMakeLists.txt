
set(demo_datasets
    firefox-metrics
    github-5-mio-selected-metrics
    kubernetes-metrics
)

set(demo_full_datasets
    calculator
    cpprestsdk
#     DirectXTK12
    glbinding
    globjects
    glfw
    GSL
    opensource-portal
    STL
    tensorflow-tfjs
    thealgorithms-cpp
    VFSForGit
    visual-analytics-components
    webgl-operate
)

foreach (dataset ${demo_datasets})

    set(source "${dataset}.tar.xz")
    set(target "${dataset}.csv")
    
    message(STATUS "Check for ${CMAKE_CURRENT_SOURCE_DIR}/${target}")
    if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${target}")
        message(STATUS "Unpack demo dataset ${dataset}")
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_CURRENT_SOURCE_DIR}/${source}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endif ()

endforeach ()

foreach (dataset ${demo_full_datasets})

    set(source "${dataset}.tar.xz")
    set(target "${dataset}")
    
    message(STATUS "Check for ${CMAKE_CURRENT_SOURCE_DIR}/${target}")
    if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${target}")
        message(STATUS "Unpack demo dataset ${dataset}")
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_CURRENT_SOURCE_DIR}/${source}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endif ()

endforeach ()
