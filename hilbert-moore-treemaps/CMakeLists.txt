
# 
# External dependencies
# 

# Added as thirdparty
# find_package(glm       REQUIRED)


# 
# Executable name and options
# 

# Target name
set(target hilbert-moore-treemap-layouts)

# Exit here if required dependencies are not met
message(STATUS "Prototype ${target}")


# 
# Sources
#

set(source_path  "${CMAKE_CURRENT_SOURCE_DIR}")

set(sources
    ${source_path}/AbstractBuffer.h
    ${source_path}/AbstractBuffer.cpp
    ${source_path}/ArgumentParser.h
    ${source_path}/ArgumentParser.inl
    ${source_path}/Buffer.h
    ${source_path}/Buffer.inl
    ${source_path}/Tree.h
    ${source_path}/Tree.inl
    ${source_path}/TreeNode.h
    ${source_path}/TreeNode.inl
    ${source_path}/CSVImporter.h
    ${source_path}/AttributeSumAggregation.h
    ${source_path}/TreeProcessing.h
    ${source_path}/WeightPartitioning.h
    ${source_path}/SingleLevelHilbertMoore.h
    ${source_path}/HilbertMoorePattern.h
    ${source_path}/RectangularLayoutAlgorithm.h
    ${source_path}/TreemapLayouter.h
    ${source_path}/Rect.h
    ${source_path}/RectangularTreemapLayout.h
    ${source_path}/conversion.h
    ${source_path}/conversion.inl
    ${source_path}/ArgumentParser.cpp
    ${source_path}/Tree.cpp
    ${source_path}/TreeNode.cpp
    ${source_path}/CSVImporter.cpp
    ${source_path}/AttributeSumAggregation.cpp
    ${source_path}/TreeProcessing.cpp
    ${source_path}/WeightPartitioning.cpp
    ${source_path}/SingleLevelHilbertMoore.cpp
    ${source_path}/TreemapLayouter.cpp
    ${source_path}/Rect.cpp
    ${source_path}/conversion.cpp
    ${source_path}/export.h
    ${source_path}/export.cpp
    ${source_path}/CPUMeasurement.h
    ${source_path}/CPUMeasurement.cpp

    ${source_path}/main.cpp
)


# 
# Create library
# 

# Build library
add_executable(${target}
    ${sources}
)


# 
# Project options
# 

set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    FOLDER "${IDE_FOLDER}"
)


# 
# Include directories
# 

target_include_directories(${target}
    PRIVATE
    ${PROJECT_BINARY_DIR}/source/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)


# 
# Libraries
# 

target_link_libraries(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_LIBRARIES}
    glm::glm

    INTERFACE
)


# 
# Compile definitions
# 

target_compile_definitions(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_COMPILE_DEFINITIONS}

    INTERFACE
)


# 
# Compile options
# 

target_compile_options(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_COMPILE_OPTIONS}

    INTERFACE
)


# 
# Linker options
# 

target_link_libraries(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_LINKER_OPTIONS}

    INTERFACE
)


# 
# Deployment
# 

# Executable
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN} COMPONENT runtime
    BUNDLE  DESTINATION ${INSTALL_BIN} COMPONENT runtime
)
