-- Keep a log of any SQL queries you execute as you solve the mystery.

-- description
SELECT description FROM crime_scene_reports WHERE (day = 28 AND month = 7 AND year = 2020 AND street = "Chamberlin Street");
-- We got desciption: Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse.
-- Interviews were conducted today with three witnesses who were present at the time — each of their interview transcripts mentions the courthouse.

-- interview transcripts
SELECT name, transcript FROM interviews WHERE (day = 28 AND month = 7 AND year = 2020);
-- Ruth | thief + car -> driven away from courthouse parking lot and drive away on same day around the time in question
-- Eugene |  thief -> name unknown. withdrawn money from ATM on fifer street same day
-- Raymond | theif was -> call for less than a minute + flight out of Fiftyville tomorrow.

-- driven away from courthouse
SELECT * FROM people WHERE license_plate IN (SELECT license_plate from courthouse_security_logs WHERE day = 28 AND month = 7 AND year = 2020 AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = "exit");

-- withdraw money from fifer street ATM
SELECT * FROM people WHERE id IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number from atm_transactions WHERE (day = 28 AND month = 7 AND year = 2020 AND atm_location = "Fifer Street" AND transaction_type = "withdraw")));

-- call for less than minute
SELECT * FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year = 2020 AND month = 7 AND day = 28 AND duration <= 60 UNION SELECT receiver FROM phone_calls WHERE year = 2020 AND month = 7 AND day = 28 AND duration <= 60);

-- flight out of Fiftyville tomorrow
SELECT * FROM people WHERE passport_number IN (SELECT passport_number FROM passengers WHERE flight_id = ( SELECT id FROM flights WHERE (origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville") AND year = 2020 AND month = 7 AND day = 29)));


-- intersecting all these conditions found the suspect is Ernest
SELECT * FROM people WHERE
license_plate IN (SELECT license_plate from courthouse_security_logs WHERE day = 28 AND month = 7 AND year = 2020 AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = "exit") AND
id IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number from atm_transactions WHERE (day = 28 AND month = 7 AND year = 2020 AND atm_location = "Fifer Street" AND transaction_type = "withdraw"))) AND
phone_number IN (SELECT caller FROM phone_calls WHERE year = 2020 AND month = 7 AND day = 28 AND duration <= 60) AND
passport_number IN (SELECT passport_number FROM passengers WHERE flight_id = (SELECT flights.id FROM flights JOIN airports ON origin_airport_id = airports.id WHERE airports.city = "Fiftyville" AND flights.year = 2020 AND flights.month = 7 AND flights.day = 29 ORDER BY flights.hour LIMIT 1));

-- Finding who Ernest called
SELECT * FROM people WHERE phone_number = (SELECT receiver FROM phone_calls WHERE year = 2020 AND month = 7 AND day = 28 AND duration <= 60 AND caller = (SELECT phone_number FROM people WHERE name = "Ernest"));

-- Checking where both Ernest & Berthold are flying to
SELECT city FROM airports WHERE id = (SELECT destination_airport_id FROM flights WHERE id IN (SELECT flight_id FROM passengers WHERE passport_number = (SELECT passport_number FROM people WHERE name = "Ernest")));
