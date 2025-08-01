# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. The list of available CPACK_xxx variables and their associated
# documentation may be obtained using
#  cpack --help-variable-list
#
# Some variables are common to all generators (e.g. CPACK_PACKAGE_NAME)
# and some are specific to a generator
# (e.g. CPACK_NSIS_EXTRA_INSTALL_COMMANDS). The generator specific variables
# usually begin with CPACK_<GENNAME>_xxxx.


set(CPACK_BINARY_DEB "OFF")
set(CPACK_BINARY_FREEBSD "OFF")
set(CPACK_BINARY_IFW "OFF")
set(CPACK_BINARY_NSIS "OFF")
set(CPACK_BINARY_RPM "OFF")
set(CPACK_BINARY_STGZ "ON")
set(CPACK_BINARY_TBZ2 "OFF")
set(CPACK_BINARY_TGZ "ON")
set(CPACK_BINARY_TXZ "OFF")
set(CPACK_BINARY_TZ "ON")
set(CPACK_BUILD_SOURCE_DIRS "/home/luis/github/Bulletin/Server/C-remake/monolithic;/home/luis/github/Bulletin/build")
set(CPACK_CMAKE_GENERATOR "Ninja")
set(CPACK_COMPONENTS_ALL "")
set(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
set(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_FILE "/usr/share/cmake/Templates/CPack.GenericDescription.txt")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_SUMMARY "MyCProject built using CMake")
set(CPACK_GENERATOR "TBZ2;TGZ;TXZ;TZ")
set(CPACK_IGNORE_FILES "/home/luis/github/Bulletin/Server/C-remake/monolithic/external/json-c/build;/home/luis/github/Bulletin/Server/C-remake/monolithic/external/json-c/cmake-build-debug;/home/luis/github/Bulletin/Server/C-remake/monolithic/external/json-c/pack;/home/luis/github/Bulletin/Server/C-remake/monolithic/external/json-c/.idea;/home/luis/github/Bulletin/Server/C-remake/monolithic/external/json-c/.DS_Store;/home/luis/github/Bulletin/Server/C-remake/monolithic/external/json-c/.git;/home/luis/github/Bulletin/Server/C-remake/monolithic/external/json-c/.vscode")
set(CPACK_INNOSETUP_ARCHITECTURE "x64")
set(CPACK_INSTALLED_DIRECTORIES "/home/luis/github/Bulletin/Server/C-remake/monolithic;/")
set(CPACK_INSTALL_CMAKE_PROJECTS "")
set(CPACK_INSTALL_PREFIX "/usr/local")
set(CPACK_MODULE_PATH "")
set(CPACK_NSIS_DISPLAY_NAME "json-c 0.18")
set(CPACK_NSIS_INSTALLER_ICON_CODE "")
set(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
set(CPACK_NSIS_PACKAGE_NAME "json-c 0.18")
set(CPACK_NSIS_UNINSTALL_NAME "Uninstall")
set(CPACK_OBJCOPY_EXECUTABLE "/sbin/objcopy")
set(CPACK_OBJDUMP_EXECUTABLE "/sbin/objdump")
set(CPACK_OUTPUT_CONFIG_FILE "/home/luis/github/Bulletin/build/CPackConfig.cmake")
set(CPACK_PACKAGE_DEFAULT_LOCATION "/")
set(CPACK_PACKAGE_DESCRIPTION_FILE "/usr/share/cmake/Templates/CPack.GenericDescription.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MyCProject built using CMake")
set(CPACK_PACKAGE_FILE_NAME "json-c-0.18-Source")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "json-c 0.18")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "json-c 0.18")
set(CPACK_PACKAGE_NAME "json-c")
set(CPACK_PACKAGE_RELOCATABLE "true")
set(CPACK_PACKAGE_VENDOR "Humanity")
set(CPACK_PACKAGE_VERSION "0.18")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "18")
set(CPACK_PACKAGE_VERSION_PATCH "")
set(CPACK_READELF_EXECUTABLE "/sbin/readelf")
set(CPACK_RESOURCE_FILE_LICENSE "/usr/share/cmake/Templates/CPack.GenericLicense.txt")
set(CPACK_RESOURCE_FILE_README "/usr/share/cmake/Templates/CPack.GenericDescription.txt")
set(CPACK_RESOURCE_FILE_WELCOME "/usr/share/cmake/Templates/CPack.GenericWelcome.txt")
set(CPACK_RPM_PACKAGE_SOURCES "ON")
set(CPACK_SET_DESTDIR "OFF")
set(CPACK_SOURCE_GENERATOR "TBZ2;TGZ;TXZ;TZ")
set(CPACK_SOURCE_IGNORE_FILES "/home/luis/github/Bulletin/Server/C-remake/monolithic/external/json-c/build;/home/luis/github/Bulletin/Server/C-remake/monolithic/external/json-c/cmake-build-debug;/home/luis/github/Bulletin/Server/C-remake/monolithic/external/json-c/pack;/home/luis/github/Bulletin/Server/C-remake/monolithic/external/json-c/.idea;/home/luis/github/Bulletin/Server/C-remake/monolithic/external/json-c/.DS_Store;/home/luis/github/Bulletin/Server/C-remake/monolithic/external/json-c/.git;/home/luis/github/Bulletin/Server/C-remake/monolithic/external/json-c/.vscode")
set(CPACK_SOURCE_INSTALLED_DIRECTORIES "/home/luis/github/Bulletin/Server/C-remake/monolithic;/")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "/home/luis/github/Bulletin/build/CPackSourceConfig.cmake")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "json-c-0.18-Source")
set(CPACK_SOURCE_RPM "OFF")
set(CPACK_SOURCE_TBZ2 "ON")
set(CPACK_SOURCE_TGZ "ON")
set(CPACK_SOURCE_TOPLEVEL_TAG "Linux-Source")
set(CPACK_SOURCE_TXZ "ON")
set(CPACK_SOURCE_TZ "ON")
set(CPACK_SOURCE_ZIP "OFF")
set(CPACK_STRIP_FILES "")
set(CPACK_SYSTEM_NAME "Linux")
set(CPACK_THREADS "1")
set(CPACK_TOPLEVEL_TAG "Linux-Source")
set(CPACK_WIX_SIZEOF_VOID_P "8")

if(NOT CPACK_PROPERTIES_FILE)
  set(CPACK_PROPERTIES_FILE "/home/luis/github/Bulletin/build/CPackProperties.cmake")
endif()

if(EXISTS ${CPACK_PROPERTIES_FILE})
  include(${CPACK_PROPERTIES_FILE})
endif()
