add_executable(xyz_diff xyz_diff.cpp)
target_link_libraries(xyz_diff gnss)
install(TARGETS xyz_diff DESTINATION bin)

add_executable(time_convert time_convert.cpp)
target_link_libraries(time_convert gnss)
install(TARGETS time_convert DESTINATION bin)

add_executable(broadcast_test broadcast_test.cpp rtk_test.cpp socket_test.cpp)
target_link_libraries(broadcast_test gnss)
install(TARGETS broadcast_test DESTINATION bin)

add_executable(getopt_test getopt_test.cpp)
target_link_libraries(getopt_test gnss)
install(TARGETS getopt_test DESTINATION bin)

add_executable(getopt3_test getopt3_test.cpp)
target_link_libraries(getopt3_test gnss)
install(TARGETS getopt3_test DESTINATION bin)

add_executable(rx_obs_test rx_obs_test.cpp)
target_link_libraries(rx_obs_test gnss)
install(TARGETS rx_obs_test DESTINATION bin)

add_executable(spp spp.cpp)
target_link_libraries(spp gnss)
install(TARGETS spp DESTINATION bin)

add_executable(rtk_test rtk_test.cpp)
target_link_libraries(rtk_test gnss)
install(TARGETS rtk_test DESTINATION bin)

add_executable(rtk rtk.cpp)
target_link_libraries(rtk gnss)
install(TARGETS rtk DESTINATION bin)

add_executable(lambda_test lambda_test.cpp)
target_link_libraries(lambda_test gnss)
install(TARGETS lambda_test DESTINATION bin)

add_executable(socket_test socket_test.cpp)
