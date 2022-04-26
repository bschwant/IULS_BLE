# IULS_BLE
## Implemented Functions

### 1. Time Set: sent as "ts,11,35,30"
- Represents hour (0-23), minute (0-59) and second (0-59).
- Right now it expects each of the values to be 2 characters, so it is checking for 11 total characters. 
-- Ex: can not send (ts,1,1,1), however, I will try to update this when i get a chance.
### 2. Date Set: sent as "ds,04,26,22"
- Represents month, day, year
- Like time set, each value must be 2 chars, 11 total
-- Ex: April -> 04 not 4, will change this as well.
### 3. Data: sent as "data"
- Sends each data record to app that is currently stored in flash as notification in string format (utf-8).
- Note: Must be listening for notifications to be able to receive.
- Note: Must be samples in flash, or nothing will be received.
### 4. TSL236: sent as "tsl237"
- Gets current frequency from light sensor, and sends as notification to app in string format (utf-8).
### 5. Sample: sent as "sample"
- Takes sample and stores in flash, no notifications sent
### 6. Time Get: sent as "tg"
- Returns current time of board as notification to app, in format string (utf-8).
### 7. Date Get: sent as "dg"
- Returns current date of the board as notification, also in format string (utf-8).
### 8. Write Log: sent as "log,usr_msg"
- msg can be up to 8 characters long

### 9. Read Logs:  sent as "rlog"
- sends all logs from flash as notifications 
- functions pretty much the same as "data"
