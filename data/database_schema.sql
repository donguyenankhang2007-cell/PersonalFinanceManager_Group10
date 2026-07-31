CREATE TABLE IF NOT EXISTS Account
(
    id INTEGER PRIMARY KEY AUTOINCREMENT,

    name TEXT NOT NULL,

    balance REAL DEFAULT 0,

    description TEXT
);

CREATE TABLE IF NOT EXISTS Category
(
    id INTEGER PRIMARY KEY AUTOINCREMENT,

    name TEXT NOT NULL,

    type TEXT NOT NULL,

    color TEXT,

    icon TEXT
);

CREATE TABLE IF NOT EXISTS Transactions
(
    id INTEGER PRIMARY KEY AUTOINCREMENT,

    accountId INTEGER NOT NULL,

    categoryId INTEGER NOT NULL,

    amount REAL NOT NULL,

    transactionDate TEXT NOT NULL,

    note TEXT,

    type TEXT NOT NULL DEFAULT 'expense',

    FOREIGN KEY(accountId)
        REFERENCES Account(id),

    FOREIGN KEY(categoryId)
        REFERENCES Category(id)
);

CREATE TABLE IF NOT EXISTS Budget
(
    id INTEGER PRIMARY KEY AUTOINCREMENT,

    categoryId INTEGER NOT NULL,

    amount REAL NOT NULL,

    month INTEGER NOT NULL,

    year INTEGER NOT NULL,

    FOREIGN KEY(categoryId)
        REFERENCES Category(id)
);

INSERT OR IGNORE INTO Account(id, name, balance, description)
    VALUES(1, 'Cash', 0, 'Tien mat');

INSERT OR IGNORE INTO Account(id, name, balance, description)
    VALUES(2, 'Bank', 0, 'Tai khoan ngan hang');