// ArchiveWriter.cpp (COMPLETO / REVISADO)
#include "ArchiveWriter.h"
#include "../FoxFS/config.h"

#include <cryptopp/modes.h>
#include <cryptopp/blowfish.h>
#include <cryptopp/crc.h>
#include <cryptopp/osrng.h>

#include <algorithm>
#include <cctype>
#include <cstring>
#include <string>
#include <vector>

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    #include <windows.h>
#else
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/stat.h>
#endif

static inline std::string NormalizePathA(const char* filename)
{
    std::string fn = filename ? filename : "";
    std::replace(fn.begin(), fn.end(), '\\', '/');

    std::transform(fn.begin(), fn.end(), fn.begin(),
        [](unsigned char c) { return (char)std::tolower(c); });

    // fontes: "tahoma:12.fnt" -> "tahoma.fnt"
    const size_t colonPos = fn.find(':');
    if (colonPos != std::string::npos && fn.find(".fnt") != std::string::npos)
        fn = fn.substr(0, colonPos) + ".fnt";

    return fn;
}

ArchiveWriter::ArchiveWriter()
{
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    keys = INVALID_HANDLE_VALUE;
    file = INVALID_HANDLE_VALUE;
#else
    keys = -1;
    file = -1;
#endif
    std::memset(key, 0, sizeof(key));
    std::memset(iv, 0, sizeof(iv));
}

ArchiveWriter::~ArchiveWriter()
{
    close();
}

bool ArchiveWriter::create(const char* filename, const char* keyfile)
{
    close();

    if (!filename || !*filename)
        return false;

    CryptoPP::AutoSeededRandomPool rng;
    FoxFS::TArchiveHeader header;
    std::memset(&header, 0, sizeof(header));

    header.magic = FOXFS_MAGIC;

    rng.GenerateBlock(header.key, 32);
    rng.GenerateBlock(header.iv, 32);

    // usa exatamente o que vai no header
    std::memcpy(this->key, header.key, 32);
    std::memcpy(this->iv, header.iv, 32);

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    file = CreateFileA(filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (file == INVALID_HANDLE_VALUE)
        return false;

    DWORD dwWritten = 0;
    if (!WriteFile(file, &header, (DWORD)sizeof(header), &dwWritten, 0) || dwWritten != sizeof(header))
    {
        close();
        return false;
    }

    if (keyfile && *keyfile)
    {
        keys = CreateFileA(keyfile, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        if (keys != INVALID_HANDLE_VALUE)
        {
            WriteFile(keys, this->key, 32, &dwWritten, 0);
            WriteFile(keys, this->iv, 32, &dwWritten, 0);
        }
    }
#else
    file = ::open(filename, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    if (file == -1)
        return false;

    if (::write(file, &header, sizeof(header)) != (ssize_t)sizeof(header))
    {
        close();
        return false;
    }

    if (keyfile && *keyfile)
    {
        keys = ::open(keyfile, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        if (keys != -1)
        {
            ::write(keys, this->key, 32);
            ::write(keys, this->iv, 32);
        }
    }
#endif

    return true;
}

void ArchiveWriter::close()
{
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    if (keys != INVALID_HANDLE_VALUE)
    {
        CloseHandle(keys);
        keys = INVALID_HANDLE_VALUE;
    }
    if (file != INVALID_HANDLE_VALUE)
    {
        CloseHandle(file);
        file = INVALID_HANDLE_VALUE;
    }
#else
    if (keys != -1)
    {
        ::close(keys);
        keys = -1;
    }
    if (file != -1)
    {
        ::close(file);
        file = -1;
    }
#endif

    std::memset(key, 0, sizeof(key));
    std::memset(iv, 0, sizeof(iv));
}

bool ArchiveWriter::add(const char* filename,
                        unsigned int decompressed,
                        unsigned int compressed,
                        unsigned int hash,
                        const void* data)
{
    if (!filename || !*filename || !data || compressed == 0)
        return false;

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    if (file == INVALID_HANDLE_VALUE)
        return false;
#else
    if (file == -1)
        return false;
#endif

    // 1) Normaliza UMA vez e usa em tudo
    const std::string norm = NormalizePathA(filename);

    // 2) CRC32 do nome normalizado (igual ao que o client vai pedir)
    unsigned int index = 0;
    CryptoPP::CRC32 crc;
    crc.CalculateDigest(reinterpret_cast<unsigned char*>(&index),
                        reinterpret_cast<const unsigned char*>(norm.c_str()),
                        (unsigned int)norm.length());

    // 3) prepara payload encriptado sem mexer no buffer original
    std::vector<unsigned char> enc;
    enc.resize(compressed);
    std::memcpy(&enc[0], data, compressed);

    CryptoPP::CFB_Mode<CryptoPP::Blowfish>::Encryption encoder(this->key, 32, this->iv);
    encoder.ProcessData(&enc[0], &enc[0], compressed);

    // 4) Entry
    FoxFS::TArchiveEntry entry;
    std::memset(&entry, 0, sizeof(entry));
    entry.decompressed = decompressed;
    entry.hash = hash;
    entry.size = compressed;
    entry.name = index;

    // offset aponta para o início do bloco de dados (logo após o entry)
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    LARGE_INTEGER m, p;
    m.QuadPart = 0;
    if (!SetFilePointerEx(file, m, &p, FILE_CURRENT))
        return false;

    entry.offset = (unsigned int)p.QuadPart + (unsigned int)sizeof(entry);

    // 5) keyfile: escreve NOME NORMALIZADO
    DWORD dwWritten = 0;
    const unsigned short namelen = (unsigned short)norm.size();

    if (keys != INVALID_HANDLE_VALUE)
    {
        WriteFile(keys, &namelen, (DWORD)sizeof(namelen), &dwWritten, 0);
        if (namelen)
            WriteFile(keys, norm.c_str(), namelen, &dwWritten, 0);
        WriteFile(keys, &hash, (DWORD)sizeof(hash), &dwWritten, 0);
    }

    // 6) escreve entry + dados
    if (!WriteFile(file, &entry, (DWORD)sizeof(entry), &dwWritten, 0) || dwWritten != sizeof(entry))
        return false;

    if (!WriteFile(file, &enc[0], compressed, &dwWritten, 0) || dwWritten != compressed)
        return false;

#else
    const off_t cur = lseek(file, 0, SEEK_CUR);
    if (cur < 0)
        return false;

    entry.offset = (unsigned int)cur + (unsigned int)sizeof(entry);

    const unsigned short namelen = (unsigned short)norm.size();
    if (keys != -1)
    {
        ::write(keys, &namelen, sizeof(namelen));
        if (namelen)
            ::write(keys, norm.c_str(), namelen);
        ::write(keys, &hash, sizeof(hash));
    }

    if (::write(file, &entry, sizeof(entry)) != (ssize_t)sizeof(entry))
        return false;

    if (::write(file, &enc[0], compressed) != (ssize_t)compressed)
        return false;
#endif

    return true;
}
