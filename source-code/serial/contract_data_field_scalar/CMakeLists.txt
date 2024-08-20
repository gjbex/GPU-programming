if (ARMADILLO_FOUND)
    add_executable(contract_data_field_scalar.exe
        contract_data_field_scalar.cpp
    )
    target_link_libraries(contract_data_field_scalar.exe
        ${ARMADILLO_LIBRARIES}
    )
    target_include_directories(contract_data_field_scalar.exe
        PRIVATE ${ARMADILLO_INCLUDE_DIRS}
    )
else()
    message(WARNING "Armadillo not found, contract_data_field_scalar.exe will not be built")
endif()
