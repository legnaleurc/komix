cmake_minimum_required(VERSION 2.8)

project(KomiX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
include(KomiXUtilities)

set(QT_USE_QTSVG TRUE)
find_package(Qt4 4.6 REQUIRED)
include(${QT_USE_FILE})

find_package(Boost REQUIRED signals)
include_directories(${Boost_INCLUDE_DIRS})

set(KOMIX_VERSION_MAJOR 0)
set(KOMIX_VERSION_MINOR 1)
set(KOMIX_VERSION_PATCH 80)
set(KOMIX_VERSION "${KOMIX_VERSION_MAJOR}.${KOMIX_VERSION_MINOR}.${KOMIX_VERSION_PATCH}")
add_definitions(-DKOMIX_VERSION=${KOMIX_VERSION} -DKOMIX_VERSION_MAJOR=${KOMIX_VERSION_MAJOR} -DKOMIX_VERSION_MINOR=${KOMIX_VERSION_MINOR} -DKOMIX_VERSION_PATCH=${KOMIX_VERSION_PATCH})

include_directories(. src/model src/utility src/widget)
include_directories(${CMAKE_BINARY_DIR})
file(GLOB_RECURSE KOMIX_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} src/*.cpp)
file(GLOB_RECURSE KOMIX_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} src/*.hpp)
file(GLOB_RECURSE KOMIX_FORMS RELATIVE ${CMAKE_SOURCE_DIR} src/*.ui)
file(GLOB_RECURSE KOMIX_TRANSLATIONS RELATIVE ${CMAKE_SOURCE_DIR} translations/*.ts)
set(KOMIX_RESOURCES komix.qrc)
find_moc_headers(KOMIX_MOC_HEADERS ${KOMIX_HEADERS})

qt4_wrap_cpp(KOMIX_MOC_SOURCES ${KOMIX_MOC_HEADERS})
qt4_add_resources(KOMIX_RCC_RESOURCES ${KOMIX_RESOURCES})
qt4_wrap_ui(KOMIX_UIC_HEADERS ${KOMIX_FORMS})
qt4_add_translation(KOMIX_QM_FILES ${KOMIX_TRANSLATIONS})

group_sources("${CMAKE_SOURCE_DIR}/src")
source_group("Generated Files" FILES ${KOMIX_MOC_SOURCES} ${KOMIX_RCC_RESOURCES} ${KOMIX_UIC_HEADERS} ${KOMIX_QM_FILES})
source_group("Resource Files" REGULAR_EXPRESSION .*\\.rc)

set_source_files_properties("image/logo.icns" PROPERTIES
	MACOSX_PACKAGE_LOCATION "Resources")
set(MACOSX_BUNDLE_ICON_FILE "logo.icns")

set(KOMIX_COMPILE_UNITS ${KOMIX_SOURCES} ${KOMIX_HEADERS} ${KOMIX_MOC_SOURCES} ${KOMIX_RCC_RESOURCES} ${KOMIX_QM_FILES} ${KOMIX_UIC_HEADERS})

if(WIN32)
	add_executable(komix WIN32 komix.rc ${KOMIX_HEADERS} ${KOMIX_COMPILE_UNITS})
	set(KOMIX_EXTRA_LIBRARIES ${KOMIX_EXTRA_LIBRARIES} ${QT_QTMAIN_LIBRARY})
	set_target_properties(komix PROPERTIES COMPILE_FLAGS "/MP /Zc:wchar_t-")
elseif(APPLE)
	add_executable(komix MACOSX_BUNDLE "image/logo.icns" ${KOMIX_COMPILE_UNITS})
	set_target_properties(komix PROPERTIES COMPILE_FLAGS "-std=c++0x -stdlib=libc++" LINK_FLAGS "-stdlib=libc++")
else()
	add_executable(komix ${KOMIX_COMPILE_UNITS})
	set_target_properties(komix PROPERTIES COMPILE_FLAGS "-std=c++0x")
endif()

target_link_libraries(komix ${KOMIX_EXTRA_LIBRARIES} ${QT_LIBRARIES} ${Boost_LIBRARIES})

# install
include(InstallRequiredSystemLibraries)
install(TARGETS komix
	COMPONENT "Runtime"
	RUNTIME DESTINATION "bin"
	BUNDLE DESTINATION ".")
if(APPLE)
	set(KOMIX_PLUGINS_INSTALL_DIR "komix.app/Contents/PlugIns")
elseif(WIN32)
	set(KOMIX_PLUGINS_INSTALL_DIR "plugins")
endif()
if(APPLE OR WIN32)
	foreach(f "GIF" "ICO" "JPEG" "MNG" "SVG" "TGA" "TIFF")
		list(APPEND KOMIX_QT_PLUGINS_IMAGEFORMATS_DEBUG "${QT_Q${f}_PLUGIN_DEBUG}")
		list(APPEND KOMIX_QT_PLUGINS_IMAGEFORMATS_RELEASE "${QT_Q${f}_PLUGIN_RELEASE}")
	endforeach()
	install(FILES ${KOMIX_QT_PLUGINS_IMAGEFORMATS_DEBUG} DESTINATION "${KOMIX_PLUGINS_INSTALL_DIR}/imageformats" CONFIGURATIONS "Debug" "RelWithDebInfo" COMPONENT "Runtime")
	install(FILES ${KOMIX_QT_PLUGINS_IMAGEFORMATS_RELEASE} DESTINATION "${KOMIX_PLUGINS_INSTALL_DIR}/imageformats" CONFIGURATIONS "Release" "MinSizeRel" COMPONENT "Runtime")

	configure_file("${CMAKE_SOURCE_DIR}/cmake/modules/install.cmake.in" "${CMAKE_BINARY_DIR}/install.cmake" @ONLY)
	install(SCRIPT "${CMAKE_BINARY_DIR}/install.cmake" COMPONENT Runtime)
endif()

# package
set(CPACK_PACKAGE_VERSION_MAJOR ${KOMIX_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${KOMIX_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${KOMIX_VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT "Wei Cheng Pan <legnaleurc@gmail.com>")
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
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
