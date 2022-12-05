if(${IDF_TARGET} STREQUAL "linux")
    set(requires esp_stubs)
endif()

if("${CONFIG_EXAMPLE_IPV4}" STREQUAL y)
    set(tcp_client_ip tcp_init.c)
else()
    set(tcp_client_ip tcp_client_v6.c)
endif()

idf_component_register(SRCS "tcp_init.c" "sensors.c" "packeting.c" "TCP_clients.c" "UDP_clients.c" "${tcp_client_ip}"
                                INCLUDE_DIRS "."
                                REQUIRES ${requires})
