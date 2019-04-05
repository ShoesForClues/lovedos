# Building LoveDOS
Although LoveDOS provides precompiled binaries on the
[releases page](https://github.com/rxi/lovedos/releases), you may still wish to
build the project's source if you want to make changes to the project.


## Requirements
LoveDOS depends on the following being installed before building:
* **[Lua 5.1-5.3](https://www.lua.org/)** is required by LuLABI, the build
  system
* **[DJGPP cross compiler](https://github.com/andrewwutw/build-djgpp)** 
  is required to compile the source files
* **[LuLABI](https://github.com/ShoesForClues/lulabi)** 
  is required to compile the source files


## Building
To compile you should clone the git repository or
[download the .zip](https://github.com/rxi/lovedos/archive/master.zip) of it.
Once this is done you should follow the build instructions for [LuLABI](https://github.com/ShoesForClues/lulabi)

Assuming the COMPILER variable is correctly set the command should be run:
```
lulabi build -f <path_to_love_source>
```
 The command will output the following line when it starts:
```
Building: love_dos
```
Within a minute the script should finish and display the following line:
```
Finished.
```
There should now be a file named "love.exe" in the "bin/" directory
