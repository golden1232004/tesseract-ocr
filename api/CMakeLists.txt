option (BUILD_APP_CHARACTER_RECOGNITION "" ON)
option (BUILD_APP_BILL_CLASSIFICATION "" OFF)

## build static library
if (NOT DEFINED BUILD_SHARED_LIBS)
  set (BUILD_SHARED_LIBS ON)
endif ()

if (BUILD_SHARED_LIBS)
  set(LIB_TYPE SHARED)
else()
  set(LIB_TYPE STATIC)
endif ()

if (BUILD_APP_CHARACTER_RECOGNITION)
  add_subdirectory(character-recognition)
endif ()
if (BUILD_APP_BILL_CLASSIFICATION)
  add_subdirectory(bill-classification)
endif ()
