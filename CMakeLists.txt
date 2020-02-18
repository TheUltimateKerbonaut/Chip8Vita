cmake_minimum_required(VERSION 2.8)

## Include the Vita toolchain
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

# Name of the project
project(chip8_vita)
# This line adds Vita helper macros, must go after project definition in order
# to build Vita specific artifacts (self/vpk).
include("${VITASDK}/share/vita.cmake" REQUIRED)

## Configuration options for this app
# Display name (under bubble in LiveArea)
set(VITA_APP_NAME "Chip8 Vita")
# Unique ID must be exactly 9 characters. Recommended: XXXXYYYYY where X =
# unique string of developer and Y = a unique number for this app
set(VITA_TITLEID  "LUKA00001")
# Optional version string to show in LiveArea's more info screen
set(VITA_VERSION  "01.00")

## Flags and includes for building
# Note that we make sure not to overwrite previous flags
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -O2")
# Optional. You can specify more param.sfo flags this way.
set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")

# Add any additional include paths here
include_directories(
  ../common # This is for debugScreenPrintf(), you shouldn't need it in your projects
)

# Add any additional library paths here
# ${CMAKE_CURRENT_BINARY_DIR} lets you use any library currently being built
link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  $ENV{VITASDK}/arm-vita-eabi/libvita2d.a
)

## Build and link
# Add all the files needed to compile here
#add_definitions(-DDEBUG_CHIP8)
add_executable(${PROJECT_NAME}
  src/main.cpp
  src/Display.cpp
  src/GameSelect.cpp
  src/ROM.cpp
  src/Chip8.cpp
)

# Library to link to (drop the -l prefix). This will mostly be stubs.
target_link_libraries(${PROJECT_NAME}
  -lvita2d
  -lSceDisplay_stub
  -lSceGxm_stub
  -lSceSysmodule_stub
  -lSceCtrl_stub
  -lScePgf_stub
  -lScePvf_stub
  -lSceCommonDialog_stub
  -lfreetype
  -lpng
  -ljpeg
  -lz
  -lm
  -lc

  -ldebugnet
  -lSceNetCtl_stub
  -lSceNet_stub
)

## Create Vita files
vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})
# The FILE directive lets you add additional files to the VPK, the syntax is
# FILE src_path dst_path_in_vpk. In this case, we add the LiveArea paths.
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
  FILE roms/15Puzzle[RogerIvie]alt.ch8 roms/15Puzzle[RogerIvie]alt.ch8
  FILE roms/15Puzzle[RogerIvie].ch8 roms/15Puzzle[RogerIvie].ch8
  FILE roms/15Puzzle[RogerIvie].txt roms/15Puzzle[RogerIvie].txt
  FILE roms/AdditionProblems[PaulC.Moews].ch8 roms/AdditionProblems[PaulC.Moews].ch8
  FILE roms/Airplane.ch8 roms/Airplane.ch8
  FILE roms/AnimalRace[BrianAstle].ch8 roms/AnimalRace[BrianAstle].ch8
  FILE roms/AnimalRace[BrianAstle].txt roms/AnimalRace[BrianAstle].txt
  FILE roms/AstroDodge[RevivalStudios,2008].ch8 roms/AstroDodge[RevivalStudios,2008].ch8
  FILE roms/AstroDodge[RevivalStudios,2008].txt roms/AstroDodge[RevivalStudios,2008].txt
  FILE roms/Biorhythm[JefWinsor].ch8 roms/Biorhythm[JefWinsor].ch8
  FILE roms/Biorhythm[JefWinsor].txt roms/Biorhythm[JefWinsor].txt
  FILE roms/Blinky[HansChristianEgeberg,1991].ch8 roms/Blinky[HansChristianEgeberg,1991].ch8
  FILE roms/Blinky[HansChristianEgeberg]alt.ch8 roms/Blinky[HansChristianEgeberg]alt.ch8
  FILE roms/Blitz[DavidWinter].ch8 roms/Blitz[DavidWinter].ch8
  FILE roms/Blitz[DavidWinter].txt roms/Blitz[DavidWinter].txt
  FILE roms/Bowling[GooitzenvanderWal].ch8 roms/Bowling[GooitzenvanderWal].ch8
  FILE roms/Bowling[GooitzenvanderWal].txt roms/Bowling[GooitzenvanderWal].txt
  FILE roms/BreakoutBrixhack[DavidWinter,1997].ch8 roms/BreakoutBrixhack[DavidWinter,1997].ch8
  FILE roms/BreakoutBrixhack[DavidWinter,1997].txt roms/BreakoutBrixhack[DavidWinter,1997].txt
  FILE roms/Breakout[CarmeloCortez,1979].ch8 roms/Breakout[CarmeloCortez,1979].ch8
  FILE roms/Breakout[CarmeloCortez,1979].txt roms/Breakout[CarmeloCortez,1979].txt
  FILE roms/BrickBrixhack,1990.ch8 roms/BrickBrixhack,1990.ch8
  FILE roms/BrickBrixhack,1990.txt roms/BrickBrixhack,1990.txt
  FILE roms/Brix[AndreasGustafsson,1990].ch8 roms/Brix[AndreasGustafsson,1990].ch8
  FILE roms/Cave.ch8 roms/Cave.ch8
  FILE roms/CoinFlipping[CarmeloCortez,1978].ch8 roms/CoinFlipping[CarmeloCortez,1978].ch8
  FILE roms/CoinFlipping[CarmeloCortez,1978].txt roms/CoinFlipping[CarmeloCortez,1978].txt
  FILE roms/Connect4[DavidWinter].ch8 roms/Connect4[DavidWinter].ch8
  FILE roms/Connect4[DavidWinter].txt roms/Connect4[DavidWinter].txt
  FILE roms/Craps[CamerloCortez,1978].ch8 roms/Craps[CamerloCortez,1978].ch8
  FILE roms/Craps[CamerloCortez,1978].txt roms/Craps[CamerloCortez,1978].txt
  FILE roms/Deflection[JohnFort].ch8 roms/Deflection[JohnFort].ch8
  FILE roms/Deflection[JohnFort].txt roms/Deflection[JohnFort].txt
  FILE roms/Figures.ch8 roms/Figures.ch8
  FILE roms/Filter.ch8 roms/Filter.ch8
  FILE roms/Guess[DavidWinter]alt.ch8 roms/Guess[DavidWinter]alt.ch8
  FILE roms/Guess[DavidWinter]alt.txt roms/Guess[DavidWinter]alt.txt
  FILE roms/Guess[DavidWinter].ch8 roms/Guess[DavidWinter].ch8
  FILE roms/Guess[DavidWinter].txt roms/Guess[DavidWinter].txt
  FILE roms/Hi-Lo[JefWinsor,1978].ch8 roms/Hi-Lo[JefWinsor,1978].ch8
  FILE roms/Hi-Lo[JefWinsor,1978].txt roms/Hi-Lo[JefWinsor,1978].txt
  FILE roms/Hidden[DavidWinter,1996].ch8 roms/Hidden[DavidWinter,1996].ch8
  FILE roms/Hidden[DavidWinter,1996].txt roms/Hidden[DavidWinter,1996].txt
  FILE roms/Kaleidoscope[JosephWeisbecker,1978].ch8 roms/Kaleidoscope[JosephWeisbecker,1978].ch8
  FILE roms/Kaleidoscope[JosephWeisbecker,1978].txt roms/Kaleidoscope[JosephWeisbecker,1978].txt
  FILE roms/Landing.ch8 roms/Landing.ch8
  FILE roms/LunarLanderUdoPernisz,1979.ch8 roms/LunarLanderUdoPernisz,1979.ch8
  FILE roms/LunarLander[UdoPernisz,1979].txt roms/LunarLander[UdoPernisz,1979].txt
  FILE roms/MastermindFourRowRobertLindley,1978.ch8 roms/MastermindFourRowRobertLindley,1978.ch8
  FILE roms/MastermindFourRowRobertLindley,1978.txt roms/MastermindFourRowRobertLindley,1978.txt
  FILE roms/Merlin[DavidWinter].ch8 roms/Merlin[DavidWinter].ch8
  FILE roms/Merlin[DavidWinter].txt roms/Merlin[DavidWinter].txt
  FILE roms/Missile[DavidWinter].ch8 roms/Missile[DavidWinter].ch8
  FILE roms/MostDangerousGame[PeterMaruhnic].ch8 roms/MostDangerousGame[PeterMaruhnic].ch8
  FILE roms/MostDangerousGame[PeterMaruhnic].txt roms/MostDangerousGame[PeterMaruhnic].txt
  FILE roms/Nim[CarmeloCortez,1978].ch8 roms/Nim[CarmeloCortez,1978].ch8
  FILE roms/Nim[CarmeloCortez,1978].txt roms/Nim[CarmeloCortez,1978].txt
  FILE roms/Paddles.ch8 roms/Paddles.ch8
  FILE roms/Pong1player.ch8 roms/Pong1player.ch8
  FILE roms/Pongalt.ch8 roms/Pongalt.ch8
  FILE roms/Pong2Ponghack[DavidWinter,1997].ch8 roms/Pong2Ponghack[DavidWinter,1997].ch8
  FILE roms/Pong[PaulVervalin,1990].ch8 roms/Pong[PaulVervalin,1990].ch8
  FILE roms/Pong[PaulVervalin,1990].txt roms/Pong[PaulVervalin,1990].txt
  FILE roms/ProgrammableSpacefighters[JefWinsor].ch8 roms/ProgrammableSpacefighters[JefWinsor].ch8
  FILE roms/ProgrammableSpacefighters[JefWinsor].txt roms/ProgrammableSpacefighters[JefWinsor].txt
  FILE roms/Puzzle.ch8 roms/Puzzle.ch8
  FILE roms/Reversi[PhilipBaltzer].ch8 roms/Reversi[PhilipBaltzer].ch8
  FILE roms/Reversi[PhilipBaltzer].txt roms/Reversi[PhilipBaltzer].txt
  FILE roms/RocketLauncher.ch8 roms/RocketLauncher.ch8
  FILE roms/RocketLaunch[JonasLindstedt].ch8 roms/RocketLaunch[JonasLindstedt].ch8
  FILE roms/Rocket[JosephWeisbecker,1978].ch8 roms/Rocket[JosephWeisbecker,1978].ch8
  FILE roms/RushHour[Hap,2006]alt.ch8 roms/RushHour[Hap,2006]alt.ch8
  FILE roms/RushHour[Hap,2006].ch8 roms/RushHour[Hap,2006].ch8
  FILE roms/RushHour[Hap,2006].txt roms/RushHour[Hap,2006].txt
  FILE roms/RussianRoulette[CarmeloCortez,1978].ch8 roms/RussianRoulette[CarmeloCortez,1978].ch8
  FILE roms/RussianRoulette[CarmeloCortez,1978].txt roms/RussianRoulette[CarmeloCortez,1978].txt
  FILE roms/SequenceShoot[JoyceWeisbecker].ch8 roms/SequenceShoot[JoyceWeisbecker].ch8
  FILE roms/SequenceShoot[JoyceWeisbecker].txt roms/SequenceShoot[JoyceWeisbecker].txt
  FILE roms/ShootingStars[PhilipBaltzer,1978].ch8 roms/ShootingStars[PhilipBaltzer,1978].ch8
  FILE roms/ShootingStars[PhilipBaltzer,1978].txt roms/ShootingStars[PhilipBaltzer,1978].txt
  FILE roms/Slide[JoyceWeisbecker].ch8 roms/Slide[JoyceWeisbecker].ch8
  FILE roms/Slide[JoyceWeisbecker].txt roms/Slide[JoyceWeisbecker].txt
  FILE roms/Soccer.ch8 roms/Soccer.ch8
  FILE roms/SpaceFlight.ch8 roms/SpaceFlight.ch8
  FILE roms/SpaceIntercept[JosephWeisbecker,1978].ch8 roms/SpaceIntercept[JosephWeisbecker,1978].ch8
  FILE roms/SpaceIntercept[JosephWeisbecker,1978].txt roms/SpaceIntercept[JosephWeisbecker,1978].txt
  FILE roms/SpaceInvaders[DavidWinter]alt.ch8 roms/SpaceInvaders[DavidWinter]alt.ch8
  FILE roms/SpaceInvaders[DavidWinter]alt.txt roms/SpaceInvaders[DavidWinter]alt.txt
  FILE roms/SpaceInvaders[DavidWinter].ch8 roms/SpaceInvaders[DavidWinter].ch8
  FILE roms/SpaceInvaders[DavidWinter].txt roms/SpaceInvaders[DavidWinter].txt
  FILE roms/SpookySpot[JosephWeisbecker,1978].ch8 roms/SpookySpot[JosephWeisbecker,1978].ch8
  FILE roms/SpookySpot[JosephWeisbecker,1978].txt roms/SpookySpot[JosephWeisbecker,1978].txt
  FILE roms/Squash[DavidWinter].ch8 roms/Squash[DavidWinter].ch8
  FILE roms/Submarine[CarmeloCortez,1978].ch8 roms/Submarine[CarmeloCortez,1978].ch8
  FILE roms/Submarine[CarmeloCortez,1978].txt roms/Submarine[CarmeloCortez,1978].txt
  FILE roms/SumFun[JoyceWeisbecker].ch8 roms/SumFun[JoyceWeisbecker].ch8
  FILE roms/SumFun[JoyceWeisbecker].txt roms/SumFun[JoyceWeisbecker].txt
  FILE roms/Syzygy[RoyTrevino,1990].ch8 roms/Syzygy[RoyTrevino,1990].ch8
  FILE roms/Syzygy[RoyTrevino,1990].txt roms/Syzygy[RoyTrevino,1990].txt
  FILE roms/Tank.ch8 roms/Tank.ch8
  FILE roms/Tank.txt roms/Tank.txt
  FILE roms/Tapeworm[JDR,1999].ch8 roms/Tapeworm[JDR,1999].ch8
  FILE roms/Tetris[FranDachille,1991].ch8 roms/Tetris[FranDachille,1991].ch8
  FILE roms/Tetris[FranDachille,1991].txt roms/Tetris[FranDachille,1991].txt
  FILE roms/Tic-Tac-Toe[DavidWinter].ch8 roms/Tic-Tac-Toe[DavidWinter].ch8
  FILE roms/Timebomb.ch8 roms/Timebomb.ch8
  FILE roms/Tron.ch8 roms/Tron.ch8
  FILE roms/UFO[LutzV,1992].ch8 roms/UFO[LutzV,1992].ch8
  FILE roms/UFO[LutzV,1992].txt roms/UFO[LutzV,1992].txt
  FILE roms/Vers[JMN,1991].ch8 roms/Vers[JMN,1991].ch8
  FILE roms/VerticalBrix[PaulRobson,1996].ch8 roms/VerticalBrix[PaulRobson,1996].ch8
  FILE roms/Wall[DavidWinter].ch8 roms/Wall[DavidWinter].ch8
  FILE roms/WipeOff[JosephWeisbecker].ch8 roms/WipeOff[JosephWeisbecker].ch8
  FILE roms/WormV4[RB-RevivalStudios,2007].ch8 roms/WormV4[RB-RevivalStudios,2007].ch8
  FILE roms/WormV4[RB-RevivalStudios,2007].txt roms/WormV4[RB-RevivalStudios,2007].txt
  FILE roms/X-Mirror.ch8 roms/X-Mirror.ch8
  FILE roms/ZeroPong[zeroZshadow,2007].ch8 roms/ZeroPong[zeroZshadow,2007].ch8
)
