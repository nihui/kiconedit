project(kiconedit)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include_directories(${KDE4_INCLUDES})

########### next target ###############

set(kiconedit_SRCS
    icongrid.cpp
    gridview.cpp
    main.cpp
    mainwindow.cpp
    palette.cpp
)

kde4_add_executable(kiconedit ${kiconedit_SRCS})

target_link_libraries(kiconedit ${KDE4_KIO_LIBS} )

install(TARGETS kiconedit ${INSTALL_TARGETS_DEFAULT_ARGS} )

########### install files ###############
install( FILES kiconeditui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kiconedit )
