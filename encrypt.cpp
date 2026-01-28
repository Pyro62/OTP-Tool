#include "encrypt.h"
#include <algorithm>



void Encrypt::encrypt(const std::string& srcFilename,const std::string& keyFilename,const std::string& dstFilename)
{
    std::filesystem::path keyPath{keyFilename};
    auto keySize = std::filesystem::file_size(keyPath);

    std::filesystem::path srcPath{srcFilename};
    auto srcSize = std::filesystem::file_size(srcPath);

    if(keySize - keyOffset < srcSize) throw std::runtime_error("Invalid Key! Key is smaller than source file.");

    std::ifstream src{srcFilename, std::ios::binary};
    std::ifstream key{keyFilename, std::ios::binary};
    std::ifstream check{dstFilename};
    if(check) throw std::runtime_error("Destination file already exists! Try again");

    
    if(!key) throw std::runtime_error("Could not open key");
    if(!src) throw std::runtime_error("Could not open source");
    key.seekg(keyOffset,std::ios::beg); // doesnt change anything for regular files, but for directory it prevents two time pad vulnerability
    std::ofstream dst{dstFilename, std::ios::binary};
    if(!dst) throw std::runtime_error("Could not open destination file"); // dst as final check, so on fail no creation
    pad(src,key,dst);

}



void Encrypt::pad(std::ifstream& src, std::ifstream& key, std::ofstream& dst)
{
    char keyBuffer[BUFFER_SIZE];
    char srcBuffer[BUFFER_SIZE];
    size_t keyByteQty;
    size_t srcByteQty;
    while(true){
        src.read(srcBuffer,BUFFER_SIZE);
        srcByteQty = src.gcount();
        key.read(keyBuffer,srcByteQty); // read srcbyteqty instead of buffer size so they usually match
        keyByteQty = key.gcount();
        if(srcByteQty > keyByteQty) throw std::runtime_error("Invalid key, too short for file");
        if(srcByteQty==0) break;
        for (size_t i{0}; i < srcByteQty; i++)
        {
            srcBuffer[i] = srcBuffer[i]^keyBuffer[i];
        }
        keyOffset+=srcByteQty; // add bytes processed
        dst.write(srcBuffer, srcByteQty);
    }
}



void Encrypt::encWithKey(const std::string &srcFilename, const std::string& keyName, const std::string &dstFilename) // house made keys
{
    std::ifstream src{srcFilename, std::ios::binary};
    std::ifstream check{dstFilename};
    if(check) throw std::runtime_error("Destination file already exists! Try again");
    
    if(!src) throw std::runtime_error("Could not open source");
    if(std::filesystem::exists(keyName)) throw std::runtime_error("Key file name already exists, please wait a second"); //if exists kick out
    std::ofstream dst{dstFilename, std::ios::binary};
    if(!dst) throw std::runtime_error("Could not open destination file");// dst as final check, so on fail no creation
    createKeyFile(srcFilename, keyName); // generate key first
    std::ifstream key{keyName, std::ios::binary};
    if(!key) throw std::runtime_error("Could not open key after generation");   
    pad(src,key,dst); // then pad with key

}

void Encrypt::directoryEncrypt(const std::string &srcDir, const std::string &keyName, const std::string &dstDir)
{
    // todo: iterate through everything and encrypt with original encrypt function, then return a copy with the same directory // 1/23 done
    // IDEA: ENCRYPT AND RECREATE AS WE ITERATE, SO WE OPEN A FOLDER===> SIGNALS TO CREATE ONE // done
    //bugfix needed: if max size of file > keyfile size, throw err
    std::filesystem::path rootDirectory{srcDir};
    std::filesystem::path keyPath{keyName};
    uintmax_t keySize = std::filesystem::file_size(keyPath);
    uintmax_t totalSize{0};

    std::vector<std::filesystem::path> allPaths;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(rootDirectory)) {
        allPaths.push_back(entry.path());
        if(entry.is_regular_file()){
            totalSize+=entry.file_size();
        }
    }
    std::sort(allPaths.begin(), allPaths.end());

    if(totalSize>keySize) throw std::runtime_error("Invalid key: too short for directory encryption");

    if(!std::filesystem::exists(rootDirectory) || !std::filesystem::is_directory(rootDirectory)){
    throw std::runtime_error("Source directory does not exist or is not a directory");
    }

    for (const auto& path : allPaths) {
        std::filesystem::directory_entry entry{path};
        if(entry.is_directory()){
            std::filesystem::create_directories(std::filesystem::path(dstDir) / std::filesystem::relative(entry.path(), rootDirectory)); // create mirror dir in dst
        } else if(entry.is_regular_file()){
            // encrypt file and place in new dir
            std::filesystem::path srcPath{entry.path()};
            std::filesystem::path dstPath = std::filesystem::path(dstDir) / std::filesystem::relative(srcPath, rootDirectory);
            std::filesystem::path encFileName{srcPath.stem().string() + "_encrypted" + srcPath.extension().string()};
            std::filesystem::path finalDstPath{dstPath.parent_path() / encFileName};
            std::filesystem::create_directories(finalDstPath.parent_path());
            encrypt(srcPath.string(),keyName,finalDstPath.string());
        } else{
            std::cout << "Skipping non-regular file: " << entry.path() << "\n";
        }
    }
}

void Encrypt::encDirWithKey(const std::string &srcDir, const std::string &keyName, const std::string &dstDir)
{
    createKeyFile(srcDir, keyName);
    directoryEncrypt(srcDir,keyName,dstDir);
}

void Encrypt::dryRun(const std::string& prog, const std::string &src, const std::string &key, const std::string &dst)
{
    std::filesystem::path progName{prog};
    std::filesystem::path srcPath{src};
    std::filesystem::path keyPath{key};
    std::filesystem::path dstPath{dst};
    std::filesystem::directory_entry entry{srcPath};
    uintmax_t keySize = std::filesystem::file_size(keyPath);
    uintmax_t totalSize{0};

    std::vector<std::filesystem::path> allPaths;
    std::vector<std::filesystem::directory_entry> skippedFiles;

    if(entry.is_directory()) {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(srcPath)) {
            allPaths.push_back(entry.path());
            if(entry.is_regular_file()){
                totalSize+=entry.file_size();
            }
        }
        std::sort(allPaths.begin(), allPaths.end());
        if(totalSize>keySize){
            std::cout << "Dry Run Result: \n   Invalid Key. Key size is smaller than total size of files in directory.\n"; // case for invalid key
            std::cout << "Generate proper key with command:\n    ./" << progName.filename().string() << " -f " << src << " " << key << "\n";
        } else{
            std::cout <<"Dry Run Result: \n   Valid Key:\n     Total size of files in directory: " << totalSize << " bytes\n     Key size: " << keySize << " bytes\n\n"; // case for success
            std::cout << "   MODE: Directory Encryption\n       Source Directory: " << srcPath.string() << "\n       Destination Directory: " << dstPath.string() << "\n\n";
            std::cout << "   File(s) to be Encrypted with " << key << ":\n";
            for(const auto& path : allPaths){
                std::filesystem::directory_entry entry{path};
                if(entry.is_regular_file()){
                    std::cout << "       " << entry.path().string()<< "- " << std::filesystem::file_size(path) << " bytes\n";
                } else if(entry.is_directory()){
                    std::cout << "       " << entry.path().string() << " [DIR]\n";
                } else{
                    skippedFiles.push_back(entry);
                }
            }
            if(!skippedFiles.empty()){
                std::cout << "\n   Skipped Non-Regular Files:\n";
                for(const auto& skipped : skippedFiles){
                    std::cout << "       " << skipped.path().string() << "\n";
                }
            }
            std::cout << "\n" << "   Proceed to Encyption by Running:\n       ./" << progName.filename().string() << " -d " << src << " " << key << " " << dst << "\n";
        }
    } else if(entry.is_regular_file()){
        totalSize = std::filesystem::file_size(entry);
        if(totalSize > keySize){
            std::cout << "Dry Run Result: \n   Invalid Key. Key size is smaller than source file size. \n";
            std::cout << "Generate proper key with command:\n    ./" << progName.filename().string() << " -f " << src << " " << key << "\n";
        } else{
            std::cout <<"Dry Run Result: \n   Valid Key:\n     Source file size: " << totalSize << " bytes\n     Key size: " << keySize << " bytes\n\n";
            std::cout << "   MODE: File Encryption\n       Source File: " << srcPath.string() << "\n       Destination File: " << dstPath.string() << "\n\n";
            std::cout << "   File to be Encrypted with " << key << ":\n       " << srcPath.string() << "\n";
            std::cout << "\n" << "   Proceed to Encyption by Running:\n       ./" << progName.filename().string()<<" "  << src << " " << key << " " << dst << "\n";
        }
    } else{
        throw std::runtime_error("Source is neither a file nor a directory");
    }

}
void Encrypt::createKeyFile(const std::string &srcDir, const std::string &keyName)
{
    if(std::filesystem::exists(keyName)) throw std::runtime_error("Key file name already exists!"); //if exists kick out

    if(!std::filesystem::exists(srcDir)) throw std::runtime_error("Source directory does not exist");  
    
    if(std::filesystem::is_regular_file(srcDir)){
        uintmax_t fileSize{std::filesystem::file_size(srcDir)};
        generateKey(fileSize, keyName);
    } else if(std::filesystem::is_directory(srcDir)){
        uintmax_t totalDirSize{0};
        std::filesystem::path rootDirectory{srcDir};
        for (const auto& entry : std::filesystem::recursive_directory_iterator(rootDirectory)) {
            if(entry.is_regular_file()){
                totalDirSize+=entry.file_size(); // get total size of file
            }
        }   
        generateKey(totalDirSize, keyName);
    } else{
        throw std::runtime_error("Source is neither a file nor a directory");
    }
}

void Encrypt::generateKey(uintmax_t size, const std::string &keyName)
{
    std::random_device rd;
    std::mt19937_64 rng(rd());                // seed ONCE
    std::uniform_int_distribution<uint8_t> dist(0, 255);

    std::ofstream key{keyName, std::ios::binary};
    if(!key) throw std::runtime_error("Could not generate key");
    char keyBuffer[BUFFER_SIZE]; 
    size*=2; // double size for safety
    while (size > 0) {

        size_t chunkSize = std::min<uintmax_t>(BUFFER_SIZE, size); // checks which is smaller

        for (size_t i = 0; i < chunkSize; i++) {
            keyBuffer[i] = dist(rng);         // generate sequential bytes
        }
        key.write(keyBuffer, chunkSize);

        size -= chunkSize;
    }
}

