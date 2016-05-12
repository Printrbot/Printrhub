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
        src/framework/animation/Animation.cpp
        src/framework/animation/Animator.cpp
        src/framework/core/Application.cpp
        src/framework/views/BitmapButton.cpp
        src/framework/layers/BitmapLayer.cpp
    src/Bitmaps.c
        src/framework/views/Button.cpp
    src/ChoosePrintSceneController.cpp
    src/CleanPlasticSceneController.cpp
        src/framework/core/ColorTheme.cpp
        src/fonts/font_AwesomeF000.c
        src/fonts/font_AwesomeF080.c
        src/fonts/font_PT_Sans-Narrow-Web-Regular.c
        src/framework/layers/GapLayer.cpp
        src/framework/views/Icon.cpp
    src/IdleSceneController.cpp
    src/IdleView.cpp
        src/drivers/Display/ILI9341_t3.cpp
        src/framework/views/LabelButton.cpp
        src/framework/views/LabelView.cpp
        src/framework/layers/Layer.cpp
    src/LoadFilamentSceneController.cpp
    src/MachineControlSceneController.cpp
    src/ConfirmSceneController.cpp
    src/PrintStatusSceneController.cpp
    src/framework/layers/TransparentTextLayer.cpp
    src/SettingsSceneController.cpp
    src/main.cpp
    src/MainSceneController.cpp
    src/ModelView.cpp
    src/NavBar.cpp
        src/framework/core/Object.cpp
        src/framework/core/PHDisplay.cpp
        src/framework/views/ProgressBar.cpp
        src/framework/layers/RectangleLayer.cpp
        src/framework/core/SceneController.cpp
        src/framework/layers/SDBitmapLayer.cpp
    src/SidebarSceneController.cpp
        src/framework/layers/TextLayer.cpp
        src/framework/core/Timer.cpp
        src/framework/core/UIElement.cpp
        src/framework/layers/VerticalTextLayer.cpp
        src/framework/views/View.cpp
    src/VirtualKeyboardSceneController.cpp
    src/WiFiSetupSceneController.cpp
    src/drivers/glcdfont.c
    src/drivers/SD/cache_t3.cpp
    src/drivers/SD/card_t3.cpp
    src/drivers/SD/dir_t3.cpp
    src/drivers/SD/fat_t3.cpp
    src/drivers/SD/File.cpp
    src/drivers/SD/file_t3.cpp
    src/drivers/SD/init_t3.cpp
    src/drivers/SD/SD.cpp
    src/drivers/SD/utility/Sd2Card.cpp
    src/drivers/SD/utility/SdFile.cpp
    src/drivers/SD/utility/SdVolume.cpp
        src/framework/core/GraphicsContext.cpp src/framework/core/GraphicsContext.h src/framework/core/ImageBuffer.cpp src/framework/core/ImageBuffer.h)
