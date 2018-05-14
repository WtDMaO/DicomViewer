This is a DicomViewer by QT, Here is only source code here, You need QT5.10.1 and Dcmtk 3.6.1 with opencv3.4.1 to build it

if wanna communicate with me, contact wutongxiaomo@gmail.com.

-Vision 0.1 
	Can open two DicomFile and Show it to the label
	support change windowcenter and windowwidth
		but can only change staticly(change first and then draw it), can not change it realtime.
		
-Vision 0.2 
	Can open two DicomFile and Show it to the label
	support change windowcenter and windowwidth and change image in realtime.

-Vision 0.2.1 
	not added any new function
	change the picture showing for QLable to QtGraphicViewer
	
-Vision 0.3 
	Adding a Matlab function to enhance the image.
	
-Vision 0.4 
	support define a matlab function called c_matlab used to change enhance function;
	In matlab, name the file "libMyAdd" to generate libMyAdd.h libMyAdd.dll libMyAdd.lib libMyAdd.ctf to put in 
	put dll and ctf in generate target folder. put h and lib in source code folder.