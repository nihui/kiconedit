project(kiconedit)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include_directories(${KDE4_INCLUDES})

########### next target ###############

set(kiconeditpart_SRCS
    kiconeditpart.cpp
    icongrid.cpp
    gridview.cpp
    palette.cpp
)

kde4_add_plugin(kiconeditpart ${kiconeditpart_SRCS})
target_link_libraries(kiconeditpart ${KDE4_KPARTS_LIBS})

install(TARGETS kiconeditpart DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES kiconeditpart.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES kiconeditpartui.rc DESTINATION ${DATA_INSTALL_DIR}/kiconeditpart)

set(kiconedit_SRCS
    main.cpp
    mainwindow.cpp
)

kde4_add_executable(kiconedit ${kiconedit_SRCS})
target_link_libraries(kiconedit ${KDE4_KDEUI_LIBS} ${KDE4_KPARTS_LIBS})

install(TARGETS kiconedit DESTINATION ${BIN_INSTALL_DIR})
install(FILES kiconeditui.rc DESTINATION ${DATA_INSTALL_DIR}/kiconedit)
