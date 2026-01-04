# One-Time Pad File Encryption Tool

A secure One-Time Pad (OTP) file encryption tool in C++. This tool encrypts and decrypts files using either a **user-provided key** or an **auto-generated key**.



## Features

- **Encryption & decryption** of files using OTP.  
- Supports both **user-provided keys** or if a key is not provided, will generate one.
- Handles **large files efficiently** with buffered read/write operations.  
- Random generated keys are **non-deterministic**.
- Compatible with all file types.
- Encrypted files are near unbreakable.



## User Guide

### 1. Encrypt a File with a User-Provided Key

```bash
./otp_encrypt.exe source.txt key.bin encrypted.txt
```
### 2. Encrypt a File without a User-Provided Key
```bash
./otp_encrypt.exe source.txt encrypted.txt
```

- Key file is generated in directory. 

## Requirements

- C++17 or later  
- Compatible with Windows, Linux, and macOS terminals  



## Notes/Known Issues
- Due to the file name generation implementation method, attempting to encrypt a file twice within 1 second without a user provided key throws an error. This will be fixed.
- genKey creates a new std::random_device per call, will slow down encryption of larger files. 
- Would like to try to add greater variety to the key names, rather than just numbers.
