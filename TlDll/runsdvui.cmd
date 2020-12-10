cd /d "D:\work\tlbb\TlDll\TlDll" &msbuild "TlDll.vcxproj" /t:sdvViewer /p:configuration="Debug" /p:platform="Win32" /p:SolutionDir="D:\work\tlbb\TlDll" 
exit %errorlevel% 