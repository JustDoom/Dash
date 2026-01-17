# Dash

Dash is a basic Tiling Window Manager written for X11 using XCB. Inspired by [Hypr](https://github.com/hyprwm/Hypr).
This should also function under XLibre.

## Building

Not sure what exactly is required right now. But XCB is probably required.

## Usage

To test this you will need to use Xephyr. And specify a display number

`Xephyr -br -ac -noreset -screen 1280x720 :<number>`

Then run Dash like

`DISPLAY=:<number> ./Dash`

To launch a program inside of it run it like how Dash was

`DISPLAY=:<number> xterm`

It will need to be running under X11 mode to work