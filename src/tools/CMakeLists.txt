include_directories(..)

# Executables

## opencc
add_executable(opencc CommandLine.cpp)
target_link_libraries(opencc libopencc)

## opencc_dict
add_executable(opencc_dict DictConverter.cpp)
target_link_libraries(opencc_dict libopencc)

## opencc_phrase_extract
add_executable(opencc_phrase_extract PhraseExtract.cpp)
target_link_libraries(opencc_phrase_extract libopencc)

find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})
add_library(opencc_jni SHARED OpenCC_Jni.cpp)
target_link_libraries(opencc_jni libopencc)

# Installation

install(
  TARGETS opencc opencc_dict opencc_phrase_extract
  RUNTIME DESTINATION bin
)
