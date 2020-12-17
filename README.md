# Space Invaders

**About the game:**

Space Invaders is a fixed one person shooter style video game. The player controls a laser cannon by moving it horizontally across the bottom of the screen and firing at the aliens descending toward the cannon from the top of the screen. There are aliens descending towards the cannon and the player's main goal is to defeat an alien and earn points by shooting it with the laser cannon and destroying it. As more aliens are defeated, the aliens' movement speeds up. The alien invasion is declared successful and the game ends when the aliens have successfully reached the bottom. The final score of total kills is projected after the game ends. The mp3 decoder connected to the speaker will play sound effects required. The objective of the game is to shoot the invaders with a laser cannon controlled by joystick while avoiding their bullets and preventing an invasion. Amassing a high score is a further objective and one that must be prioritized against your continued survival. Each game screen starts with 3 rows of 4 invaders. 

----
## Build and flash the project

**Build command:** scons --project=space_invaders
**Flash command:** python nxp-programmer/flash.py -i _build_space_invaders/space_invaders.bin                                                             

----
## SJ2-C Development Environment

An `SJ2` board is used at San Jose State University (SJSU) to teach Embedded Systems' courses. Part of this Git repository also includes development environment for not just an ARM controller, but also support to compile and test software on your host machine such as Windows, Mac etc.

Project highlights:

- Fully implemented in C
- Minimalistic design with little to no abstractions
- Follows good coding principles (such as YAGNI and DRY)

Infrastructure highlights:

- Supports Mac, Linux, Windows out of the box
- Version controlled toolchains and other supplementary tools
- No VMs
- No WSL dependency on Windows

----

## Build System

We use [SCons](https://scons.org/) as our build platform. The developers of SJ2-C applied experience of diverse set of build systems acquired over many years, and resorted to this one. The candidates were:

- SCons (used at Tesla, and many other companies)
- Bazel (used at Google, Zoox and many other companies)
- Make

SCons discusses the advantages and disadvantages of other existing build systems: [Build System Comparison](https://github.com/SCons/scons/wiki/sconsvsotherbuildtools)

From experience, we can state that `bazel` is really cool, but hard to extend and problematic in Windows. SCons dependencies are tricky, but it is based on python and easy to understand and extend. SCons takes advantage of a Python interpreter making it portable across multiple platforms (Mac, Linux, Windows).

----



