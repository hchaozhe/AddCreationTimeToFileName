# AddCreationTimeToFileName
Videos made by GoPro or drones don't have a name that indicates the creation time of the video. 
This is particularly annoying if you also have some data taken along with the video for multiple time, and you want to align the video time with the data time. 

This simple c++ code works on windows. 
Given the path of a directory, it acquires the creation time of each file under the given directory and add it to the name.
This is by no means the most elegent solution, but it works for me. 

To use the exe file open a cmd in Windows go to the folder where the exe is at, type in 



> AddCreationTimeToFileName \<directory name\>

If you only type

> AddCreationTimeToFileName

the usage will be shown as well

> Usage: AddCreationTimeToFileName.exe \<directory name\>

Suggestions of any kind are welcomed!
