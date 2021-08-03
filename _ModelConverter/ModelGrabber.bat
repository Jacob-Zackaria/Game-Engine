REM 
REM	convert models from Models folder to game engine format
REM


REM change directory

	cd Debug
REM call model converter.exe

ModelConverter -n JACKZ -v 2.34 ..\\..\\Models\\space_frigate.tga ..\\..\\Models\\space_frigate.fbx ..\\..\\Models\\spaceModel2.azul

ModelConverter ..\\..\\Models\\astroboy.tga ..\\..\\Models\\astroboy_binary.fbx ..\\..\\Models\\astroModel2.azul

ModelConverter ..\\..\\Models\\MilitaryTruck.tga ..\\..\\Models\\MilitaryTruck.fbx ..\\..\\Models\\militaryModel2.azul

ModelConverter ..\\..\\Models\\warbear.tga ..\\..\\Models\\warbear_binary.fbx ..\\..\\Models\\warbearModel2.azul