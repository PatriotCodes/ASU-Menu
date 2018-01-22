USE asumenu;

CREATE TABLE worker (
	workerID int NOT NULL,
	username varchar(255) NOT NULL,
	password varchar(128) NOT NULL,
	PRIMARY KEY (workerID)
);

CREATE TABLE asumenu (
	asumenuId int NOT NULL,
	workerID int NOT NULL,
	PRIMARY KEY (asumenuId),
	FOREIGN KEY (workerID) REFERENCES worker(workerID)
);

CREATE TABLE menuCategory (
	categoryID int NOT NULL,
	categoryName varchar(255) NOT NULL,
	asumenuId int NOT NULL,
	PRIMARY KEY (categoryID),
	FOREIGN KEY (asumenuId) REFERENCES asumenu(asumenuId)
);

CREATE TABLE categoryButton (
	itemID int NOT NULL,
	buttonName varchar(255) NOT NULL,
	actionString varchar(255) NOT NULL,
	actionArguments varchar(255),
	categoryID int NOT NULL,
	PRIMARY KEY (itemID),
	FOREIGN KEY (categoryID) REFERENCES menuCategory(categoryID)
);
	