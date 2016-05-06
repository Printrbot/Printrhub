cmake_minimum_required(VERSION 3.2)
project(Teensy)

include(CMakeListsPrivate.txt)

add_definitions(-DF_CPU=72000000L)
add_definitions(-DUSB_SERIAL)
add_definitions(-DLAYOUT_US_ENGLISH)
add_definitions(-D__MK20DX256__)
add_definitions(-DARDUINO=10600)
add_definitions(-DTEENSYDUINO=128)
add_definitions(-DPLATFORMIO=020901)
add_definitions(-DKINETISK)

add_custom_target(
    PLATFORMIO_BUILD ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_UPLOAD ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run --target upload
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_CLEAN ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run --target clean
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_PROGRAM ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run --target program
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_UPLOADFS ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run --target uploadfs
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_UPDATE_ALL ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion update
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(Teensy
    src/Animation.cpp
    src/Animator.cpp
    src/Application.cpp
    src/BitmapButton.cpp
    src/BitmapLayer.cpp
    src/Bitmaps.c
    src/cache_t3.cpp
    src/card_t3.cpp
    src/ChoosePrintSceneController.cpp
    src/ColorTheme.cpp
    src/dir_t3.cpp
    src/fat_t3.cpp
    src/File.cpp
    src/file_t3.cpp
    src/font_AwesomeF000.c
    src/font_AwesomeF080.c
    src/font_PT_Sans-Narrow-Web-Regular.c
    src/GapLayer.cpp
    src/glcdfont.c
    src/Icon.cpp
    src/IdleSceneController.cpp
    src/IdleView.cpp
    src/ILI9341_t3.cpp
    src/init_t3.cpp
    src/LabelButton.cpp
    src/LabelView.cpp
    src/Layer.cpp
    src/MachineControlSceneController.cpp
    src/main.cpp
    src/MainSceneController.cpp
    src/ModelView.cpp
    src/NavBar.cpp
    src/Object.cpp
    src/PHDisplay.cpp
    src/ProgressBar.cpp
    src/RectangleLayer.cpp
    src/SceneController.cpp
    src/SD.cpp
    src/Sd2Card.cpp
    src/SDBitmapLayer.cpp
    src/SdFile.cpp
    src/SdVolume.cpp
    src/SidebarSceneController.cpp
    src/TextLayer.cpp
    src/Timer.cpp
    src/UIElement.cpp
    src/View.cpp
    src/VirtualKeyboardSceneController.cpp
    src/WiFiSetupSceneController.cpp
    src/VerticalTextLayer.cpp
    src/LoadFilamentSceneController.cpp src/LoadFilamentSceneController.h
    src/CleanPlasticSceneController.cpp src/CleanPlasticSceneController.h)
