--We can execute this file by sqlite3. Command: .read filename.sql (.read databaseSchema.sql) 

--V0.1
CREATE TABLE sms_message(
    id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    text TEXT NOT NULL,
    phone_number INTEGER NOT NULL,
    read INTEGER NOT NULL DEFAULT 0, --sqlite don't have boolean variable, so we need store information about that if sms message is read or not in INTEGER 0/1
    sent INTEGER NOT NULL
);
