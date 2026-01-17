add_subdirectory(util)

target_sources(Dash PRIVATE
        main.cpp Events.cpp Events.h Window.cpp Window.h WindowManager.cpp WindowManager.h)