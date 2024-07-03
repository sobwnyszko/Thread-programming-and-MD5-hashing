# Multi-threaded Password Cracker

This C program is designed to crack hashed passwords using multiple threads and the MD5 encryption algorithm. 
It reads hashed passwords from a file (`hasla`) and attempts to find matching plaintext passwords using a dictionary file (`slownik`).

## Overview

The program leverages multithreading to expedite the password cracking process. Each thread is responsible for generating candidate passwords, 
hashing them using MD5, and comparing them against the provided hashes. The goal is to discover passwords that match the hashes stored in the `hasla` file.

## Key Features

- **Multithreaded Approach**: Uses multiple threads to concurrently generate and check password candidates.
- **MD5 Hashing**: Utilizes OpenSSL's MD5 hashing function (`bytes2md5`) for computing hashes of candidate passwords.
- **Password Generation**: Includes combinations of lowercase letters, uppercase letters, and numeric digits to cover a wide range of possible passwords.
- **Data Management**: Stores cracked passwords along with associated user information in the `deshyfr` array.
- **Hash Manipulation**: Demonstrates functionality to modify and restore hashes in the `deshyfr` array for testing purposes.

## Implementation Details

The program initializes by reading the `hasla` file to obtain hashed passwords and associated user data. It then loads a dictionary (`slownik`) 
containing potential passwords for cracking. Each thread operates independently, iterating through portions of the dictionary, applying various 
password generation techniques, and verifying against the hashed passwords.

## Requirements

To compile and run the program, ensure your system meets the following requirements:

- OpenSSL library (`libcrypto`)
- POSIX threads library (`libpthread`)
- C compiler (e.g., GCC)
