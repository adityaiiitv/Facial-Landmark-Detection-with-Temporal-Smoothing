Please rename the video to lop.mp4 to run it.
Note: In some frames the landmarks are detected incorrectly.

Commands to build:
In powershell, follow the commands given exactly, in cmd, replace ` with ^
Goto the folder directory
cd build
cmake -G "Visual Studio 14 2015 Win64" ..
cmake --build . --config Release
cd ..
.\build\Release\facialLandmarkDetector.exe