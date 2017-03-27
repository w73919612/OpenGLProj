1. Copy example folder from QT 5.8.

2. Navigate to root folder of copied example

3. Run the following to make a visual studio project: (using VS 2015)

C:\Qt58\Qt5.8.0\5.8\msvc2015_64\bin\qmake -tp vc cube.pro

in this case cube.pro was the QT project File I copied.

4. Build the solution in Visual Studio. Try running it and you will find it will give you "can't find DLL error(s)".

5. Navigate to the debug folder and run the following to create all the necessary links in the debug folder:

C:\Qt58\Qt5.8.0\5.8\msvc2015_64\bin\windeployqt cube.exe

6. Run program in Visual Studio.

7. Code/Modify away!

