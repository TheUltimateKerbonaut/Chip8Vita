# Chip8Vita
Chip8 Emulator for the PS Vita. **The VPK includes all the ROMs you'll ever need.** Runs at full speed.

## Installing
Install [the VPK](https://github.com/TheUltimateKerbonaut/Chip8Vita/releases/download/1.0.0/chip8_vita.vpk) under releases just like any other homebrew app, under ux0.

## Performance
The emulator can push about 125,000FPS if left uncapped! I've limited the FPS to 250, and it runs perfectly. If you would like to change the FPS, you can change the `targetFramerateInMilliseconds` variable in Chip8.cpp.

## Contributing
Please feel free to contribute to the project if you want to. I'll welcome any changes :)

## Authors
* **Luka Warren** - *Initial work* - [GitHub](https://github.com/TheUltimateKerbonaut/)

## Building
Make sure you have [VitaSDK](https://github.com/vitasdk) installed, then run the following:
```
mkdir build
cd build
cmake ..
cmake --build .
```

## License
This project is licensed under the Mozilla Public License 2.0  License - see the [LICENSE](LICENSE) file for details

## Acknowledgements
* [VitaSDK](https://vitasdk.org/) - Toolchain
* [Vita2d](https://github.com/xerpi/libvita2d) - Used for graphics and screen output
* [Debugnet](https://github.com/psxdev/debugnet) - Currently disabled, though still exists in code for debugging purposes.
