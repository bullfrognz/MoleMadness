mkdir GDE
mkdir GDE\GDE
mkdir GDE\Debug
mkdir GDE\Release
mkdir GDE\VLD
mkdir Runtime

call :UpdateLibrary Core
call :UpdateLibrary Renderer
call :UpdateLibrary Input
call :UpdateLibrary DataParser
call :UpdateDLL HGERenderer
call :UpdateDLL hge
call :UpdateVLD
call :UpdateFont CourierNew

pause
goto end

:UpdateFont
copy /Y ..\..\..\GDE\Tests\RuntimeFolder\%1.fnt Runtime\
copy /Y ..\..\..\GDE\Tests\RuntimeFolder\%1.png Runtime\
goto end

:UpdateVLD
rem copy /Y ..\..\..\VLD\src\vld.h GDE\VLD
copy /Y ..\..\..\GDE\VLD\vld.h GDE\VLD
rem copy /Y ..\..\..\VLD\lib\vld.lib GDE\Debug
copy /Y ..\..\..\VLD\bin\*.dll Runtime\
copy /Y ..\..\..\VLD\bin\*.manifest Runtime\
goto end

:UpdateLibrary
mkdir GDE\GDE\%1
copy /Y ..\..\..\GDE\GDE\%1\*.h GDE\GDE\%1
copy /Y ..\..\..\GDE\Debug\%1.lib GDE\Debug\
copy /Y ..\..\..\GDE\GDE\%1\Debug\vc90.pdb GDE\Debug\%1.pdb
copy /Y ..\..\..\GDE\Release\%1.lib GDE\Release\
copy /Y ..\..\..\GDE\GDE\%1\Release\vc90.pdb GDE\Release\%1.pdb
goto end

:UpdateDLL
copy /Y ..\..\..\GDE\Tests\RuntimeFolder\%1_d.dll Runtime\
copy /Y ..\..\..\GDE\Tests\RuntimeFolder\%1_d.pdb Runtime\
copy /Y ..\..\..\GDE\Tests\RuntimeFolder\%1.dll Runtime\
copy /Y ..\..\..\GDE\Tests\RuntimeFolder\%1.pdb Runtime\
goto end

:end
