cmake_minimum_required(VERSION 3.0)

project(KomiX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
include(KomiXUtilities)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(KOMIX_VERSION_MAJOR 1)
set(KOMIX_VERSION_MINOR 0)
set(KOMIX_VERSION_PATCH 0)
set(KOMIX_VERSION "${KOMIX_VERSION_MAJOR}.${KOMIX_VERSION_MINOR}.${KOMIX_VERSION_PATCH}")
add_definitions(-DKOMIX_VERSION=${KOMIX_VERSION} -DKOMIX_VERSION_MAJOR=${KOMIX_VERSION_MAJOR} -DKOMIX_VERSION_MINOR=${KOMIX_VERSION_MINOR} -DKOMIX_VERSION_PATCH=${KOMIX_VERSION_PATCH})

include_directories(
	.
	src/model
	src/utility
	src/widget
	"${CMAKE_BINARY_DIR}")
file(GLOB_RECURSE KOMIX_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} src/*.cpp)
file(GLOB_RECURSE KOMIX_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} src/*.hpp)
file(GLOB_RECURSE KOMIX_FORMS RELATIVE ${CMAKE_SOURCE_DIR} src/*.ui)
set(KOMIX_RESOURCES "${CMAKE_SOURCE_DIR}/komix.qrc")

group_sources("${CMAKE_SOURCE_DIR}/src")

set_source_files_properties("image/logo.icns" PROPERTIES
	MACOSX_PACKAGE_LOCATION "Resources")
set(MACOSX_BUNDLE_ICON_FILE "logo.icns")

set(KOMIX_COMPILE_UNITS ${KOMIX_SOURCES} ${KOMIX_HEADERS} ${KOMIX_RESOURCES})

if(WIN32)
	add_executable(komix WIN32 "${CMAKE_SOURCE_DIR}/komix.rc" ${KOMIX_COMPILE_UNITS})
	set(KOMIX_EXTRA_LIBRARIES ${KOMIX_EXTRA_LIBRARIES} ${QT_QTMAIN_LIBRARY})
	set_target_properties(komix PROPERTIES
		COMPILE_FLAGS "/MP /Zc:wchar_t-")
elseif(APPLE)
	add_executable(komix MACOSX_BUNDLE "image/logo.icns" ${KOMIX_COMPILE_UNITS})
	set_target_properties(komix PROPERTIES
		MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/cmake/modules/MacOSXBundleInfo.plist.in")
else()
	add_executable(komix ${KOMIX_COMPILE_UNITS})
endif()

set_target_properties(komix PROPERTIES CXX_STANDARD 11)
target_link_libraries(komix ${KOMIX_EXTRA_LIBRARIES} ${Boost_LIBRARIES} Qt5::Core Qt5::Widgets)

# install
include(InstallRequiredSystemLibraries)
install(TARGETS komix
	RUNTIME DESTINATION "bin" COMPONENT "Runtime"
	BUNDLE DESTINATION "." COMPONENT "Runtime")
if(APPLE)
	set(KOMIX_PLUGINS_INSTALL_DIR "komix.app/Contents/PlugIns")
elseif(WIN32)
	set(KOMIX_PLUGINS_INSTALL_DIR "plugins")
endif()
if(APPLE OR WIN32)
	foreach(plugin ${Qt5Gui_PLUGINS})
		get_target_property(location ${plugin} LOCATION)
		if("${location}" MATCHES ".*/imageformats/.*")
			list(APPEND KOMIX_QT_PLUGINS_IMAGEFORMATS_DEBUG "${location}")
			list(APPEND KOMIX_QT_PLUGINS_IMAGEFORMATS_RELEASE "${location}")
		elseif("${location}" MATCHES ".*/platforms/.*")
			list(APPEND KOMIX_QT_PLUGINS_PLATFORMS_DEBUG "${location}")
			list(APPEND KOMIX_QT_PLUGINS_PLATFORMS_RELEASE "${location}")
		endif()
	endforeach()
	install(
		FILES ${KOMIX_QT_PLUGINS_IMAGEFORMATS_DEBUG}
		DESTINATION "${KOMIX_PLUGINS_INSTALL_DIR}/imageformats"
		CONFIGURATIONS "Debug" "RelWithDebInfo"
		COMPONENT "Runtime")
	install(
		FILES ${KOMIX_QT_PLUGINS_IMAGEFORMATS_RELEASE}
		DESTINATION "${KOMIX_PLUGINS_INSTALL_DIR}/imageformats"
		CONFIGURATIONS "Release" "MinSizeRel"
		COMPONENT "Runtime")
	install(
		FILES ${KOMIX_QT_PLUGINS_PLATFORMS_DEBUG}
		DESTINATION "${KOMIX_PLUGINS_INSTALL_DIR}/platforms"
		CONFIGURATIONS "Debug" "RelWithDebInfo"
		COMPONENT "Runtime")
	install(
		FILES ${KOMIX_QT_PLUGINS_PLATFORMS_RELEASE}
		DESTINATION "${KOMIX_PLUGINS_INSTALL_DIR}/platforms"
		CONFIGURATIONS "Release" "MinSizeRel"
		COMPONENT "Runtime")

	configure_file("${CMAKE_SOURCE_DIR}/cmake/modules/install.cmake.in" "${CMAKE_BINARY_DIR}/install.cmake" @ONLY)
	install(SCRIPT "${CMAKE_BINARY_DIR}/install.cmake" COMPONENT Runtime)
endif()

# package
set(CPACK_PACKAGE_VERSION_MAJOR ${KOMIX_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${KOMIX_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${KOMIX_VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT "Wei-Cheng Pan <legnaleurc@gmail.com>")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
if(APPLE)
	set(CPACK_GENERATOR DragNDrop)
elseif(WIN32)
	set(CPACK_PACKAGE_INSTALL_DIRECTORY "KomiX")
	set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
  CreateShortCut \\\"\\\$SMPROGRAMS\\\\\\\$STARTMENU_FOLDER\\\\KomiX.lnk\\\" \\\"\\\$INSTDIR\\\\bin\\\\KomiX.exe\\\"
  CreateShortCut \\\"\\\$DESKTOP\\\\KomiX.lnk\\\" \\\"\\\$INSTDIR\\\\bin\\\\KomiX.exe\\\"
	")
	set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
  Delete \\\"\\\$DESKTOP\\\\KomiX.lnk\\\"
  !insertmacro MUI_STARTMENU_GETFOLDER Application \\\$MUI_TEMP
  Delete \\\"\\\$SMPROGRAMS\\\\\\\$MUI_TEMP\\\\KomiX.lnk\\\"
	")
endif()

include(CPack)
