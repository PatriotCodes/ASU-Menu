USE asumenu;
SET NAMES utf8;

INSERT INTO worker (workerID, username) 
	VALUES (1,'Alex');
INSERT INTO worker (workerID, username) 
	VALUES (2,'John');

INSERT INTO actionCategory (categoryName) 
	VALUES ('Категория 1');
INSERT INTO actionCategory (categoryName) 
	VALUES ('Категория 2');
	
INSERT INTO action (actionName, actionString, categoryID) 
	VALUES ('открыть проводник','C:\Windows\explorer.exe',1);
INSERT INTO action (actionName, actionString, actionArguments, categoryID) 
	VALUES ('открыть проводник с параметрами','/select,C:\Windows\py.exe','C:\Windows\explorer.exe',2);
	
INSERT INTO appointment (workerID, actionID, fromDate)
	VALUES (1,2,'2017-06-15');
INSERT INTO appointment (workerID, categoryID, fromDate)
	VALUES (2,1,'2017-06-15');