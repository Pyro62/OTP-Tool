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
./otp_encrypt.exe -f source.txt keyName.bin encrypted.txt
```

- Key file is generated in directory. 

### 3. Encrypt a Directory with a User-Provided Key
```bash
./otp_encrypt.exe -d sourceDir keyName.bin encryptedDir
```

### 4. Encrypt a Directory without a User-Provided Key
```bash
./otp_encrypt.exe -f -d sourceDir keyName.bin encryptedDir
```

## Requirements

- C++17 or later  
- Compatible with Windows, Linux, and macOS terminals  



## Notes
- Decrypt files by running the program again with the same key using the ciphertext as source file.
