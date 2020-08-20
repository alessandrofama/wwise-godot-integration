tar -xf Android.zip
tar -xf iOS.zip
tar -xf Linux.zip
tar -xf macOS.zip
tar -xf Win64.zip

xcopy android\* ..\wwise-gdnative\gdnative-demo\wwise\bin\android /s /y /i
xcopy iOS\* ..\wwise-gdnative\gdnative-demo\wwise\bin\iOS /s /y /i
xcopy linux\* ..\wwise-gdnative\gdnative-demo\wwise\bin\linux /s /y /i
xcopy osx\* ..\wwise-gdnative\gdnative-demo\wwise\bin\osx /s /y /i
xcopy win64\* ..\wwise-gdnative\gdnative-demo\wwise\bin\win64 /s /y /i