project(bunsan_bacs_external)

include(${CMAKE_SOURCE_DIR}/system-config.cmake OPTIONAL)

cmake_minimum_required(VERSION 2.8)

include(BunsanCommon)

bunsan_use_bunsan(common protobuf curl)
bunsan_use_boost(system filesystem program_options serialization thread)
bunsan_use_protobuf()

bunsan_protobuf_generate_cpp(SOURCES proto_srcs HEADERS proto_hdrs INSTALL include PROTOS
    bacs/external/external.proto
    bacs/external/result.proto
    bacs/external/submit.proto
)

bunsan_add_shared_library(${PROJECT_NAME}
    ${proto_srcs}

    src/lib/judge.cpp
)
bunsan_use(${PROJECT_NAME})

bunsan_add_executable(${PROJECT_NAME}_cli
    src/bin/cli.cpp
)

bunsan_install_headers()
bunsan_install_targets(${PROJECT_NAME})

bunsan_include_tests()
