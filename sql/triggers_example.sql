-- With Timestamps
DROP DATABASE hr;
CREATE DATABASE hr;
USE hr;
CREATE TABLE account (acct_num INT, amount INT);
CREATE TABLE account_audit (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  time_stamp TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  modification_type CHAR(16),
  acct_num INT, amount INT);

delimiter |

CREATE TRIGGER updatetrigger
BEFORE UPDATE ON account
FOR EACH ROW
  BEGIN
    INSERT INTO account_audit (modification_type, acct_num, amount) VALUES ("UPDATE", NEW.acct_num, NEW.amount);
END;
|

CREATE TRIGGER inserttrigger
BEFORE INSERT ON account
FOR EACH ROW
  BEGIN
    INSERT INTO account_audit (modification_type, acct_num, amount) VALUES ("INSERT", NEW.acct_num, NEW.amount);
END;
|

delimiter ;
INSERT INTO account VALUES (1,100);

UPDATE account
SET amount = 5
WHERE acct_num = 1;

UPDATE account
SET amount = 10
WHERE acct_num = 1;

SELECT * FROM account_audit;
