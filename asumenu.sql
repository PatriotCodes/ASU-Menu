USE asumenu;

CREATE TABLE worker (
	workerID int NOT NULL,
	username varchar(255) NOT NULL,
	PRIMARY KEY (workerID)
) CHARACTER SET utf8;

CREATE TABLE actionCategory (
	categoryID int NOT NULL AUTO_INCREMENT,
	categoryName varchar(255) NOT NULL,
	PRIMARY KEY (categoryID)
) CHARACTER SET utf8;

CREATE TABLE workAction (
	actionID int NOT NULL AUTO_INCREMENT,
	actionName varchar(255) NOT NULL,
	actionString varchar(255) NOT NULL,
	actionArguments varchar(255) NULL,
	categoryID int NOT NULL,
	PRIMARY KEY (actionID),
	FOREIGN KEY (categoryID) REFERENCES actionCategory(categoryID)
) CHARACTER SET utf8;

CREATE TABLE appointment (
	appointmentID int NOT NULL AUTO_INCREMENT,
	workerID int NOT NULL,
	actionID int NULL,
	categoryID int NULL,
	fromDate DATE NOT NULL,
	toDate DATE NULL,
	PRIMARY KEY (appointmentID),
	FOREIGN KEY (workerID) REFERENCES worker(workerID),
	FOREIGN KEY (actionID) REFERENCES workAction(actionID),
	FOREIGN KEY (categoryID) REFERENCES actionCategory(categoryID)
) CHARACTER SET utf8;

CREATE USER IF NOT EXISTS 'asuuser'@'localhost' IDENTIFIED BY '123';
GRANT SELECT ON asumenu.* TO 'user'@'localhost';