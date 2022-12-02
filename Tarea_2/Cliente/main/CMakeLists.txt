if(${IDF_TARGET} STREQUAL "linux")
    set(requires esp_stubs)
endif()

if("${CONFIG_EXAMPLE_IPV4}" STREQUAL y)
    set(tcp_client_ip esp_client.c)
else()
    set(tcp_client_ip tcp_client_v6.c)
endif()

idf_component_register(SRCS "esp_client.c" "esp_client_main.c" "${tcp_client_ip}"
                                INCLUDE_DIRS "."
                                REQUIRES ${requires})
