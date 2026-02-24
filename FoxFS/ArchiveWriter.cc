#include "ArchiveWriter.h"
#include "../FoxFS/Config.h"

#include <cryptopp/modes.h>
#include <cryptopp/blowfish.h>
#include <cryptopp/crc.h>
#include <cryptopp/osrng.h>

#include <algorithm>
#include <cctype>
#include <cstring>
#include <string>
#include <vector>

static inline std::string NormalizePathA(const char* filename)
{
    std::string fn = filename ? filename : "";

    // \ -> /
    std::replace(fn.begin(), fn.end(), '\\', '/');

    // lowercase
    std::transform(fn.begin(), fn.end(), fn.begin(),
        [](unsigned char c) { return (char)std::tolower(c); });

    // fontes: "tahoma:12.fnt" -> "tahoma.fnt"
    size_t colonPos = fn.find(':');
    if (colonPos != std::string::npos && fn.find(".fnt") != std::string::npos)
        fn = fn.substr(0, colonPos) + ".fnt";

    return fn;
}

ArchiveWriter::ArchiveWriter()
{
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    InitializeCriticalSection(&mutex);
    keys = INVALID_HANDLE_VALUE;
    file = INVALID_HANDLE_VALUE;
#else
    pthread_mutex_init(&mutex, 0);
    keys = -1;
    file = -1;
#endif
    std::memset(key, 0, sizeof(key));
    std::memset(iv, 0, sizeof(iv));
}

ArchiveWriter::~ArchiveWriter()
{
    close();
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    DeleteCriticalSection(&mutex);
#else
    pthread_mutex_destroy(&mutex);
#endif
}

bool ArchiveWriter::create(const char* filename, const char* keyfile)
{
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    EnterCriticalSection(&mutex);
#else
    pthread_mutex_lock(&mutex);
#endif

    close();

    if (!filename || !*filename)
    {
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
        LeaveCriticalSection(&mutex);
#else
        pthread_mutex_unlock(&mutex);
#endif
        return false;
    }

    CryptoPP::AutoSeededRandomPool rng;
    FoxFS::TArchiveHeader header;
    header.magic = FOXFS_MAGIC;

    rng.GenerateBlock(header.key, 32);
    rng.GenerateBlock(header.iv, 32);

    // usa exatamente o que vai no header
    std::memcpy(this->key, header.key, 32);
    std::memcpy(this->iv, header.iv, 32);

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    file = CreateFileA(filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (file == INVALID_HANDLE_VALUE)
    {
        LeaveCriticalSection(&mutex);
        return false;
    }

    DWORD dwWritten = 0;
    WriteFile(file, &header, sizeof(header), &dwWritten, 0);

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
    {
        pthread_mutex_unlock(&mutex);
        return false;
    }

    ::write(file, &header, sizeof(header));

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

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    LeaveCriticalSection(&mutex);
#else
    pthread_mutex_unlock(&mutex);
#endif
    return true;
}

void ArchiveWriter::close()
{
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    // Nota: close() pode ser chamado dentro de create() com mutex já segurado.
    // Aqui não voltamos a lockar para evitar deadlock.
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
    EnterCriticalSection(&mutex);
#else
    pthread_mutex_lock(&mutex);
#endif

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    const bool fileOk = (file != INVALID_HANDLE_VALUE);
#else
    const bool fileOk = (file != -1);
#endif
    if (!fileOk)
    {
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
        LeaveCriticalSection(&mutex);
#else
        pthread_mutex_unlock(&mutex);
#endif
        return false;
    }

    // 1) Normaliza UMA vez e usa em tudo
    const std::string norm = NormalizePathA(filename);

    // 2) CRC32 do nome normalizado
    unsigned int index = 0;
    CryptoPP::CRC32 crc;
    crc.CalculateDigest(reinterpret_cast<unsigned char*>(&index),
                        reinterpret_cast<const unsigned char*>(norm.c_str()),
                        (unsigned int)norm.length());

    // 3) Entry
    FoxFS::TArchiveEntry entry;
    entry.decompressed = decompressed;
    entry.hash = hash;
    entry.offset = sizeof(entry); // depois somamos a posição atual
    entry.size = compressed;
    entry.name = index;

    // 4) Encripta para buffer temporário (evita mexer em memória const)
    std::vector<unsigned char> enc;
    enc.resize(compressed);
    std::memcpy(enc.data(), data, compressed);

    CryptoPP::CFB_Mode<CryptoPP::Blowfish>::Encryption encoder(this->key, 32, this->iv);
    encoder.ProcessData(enc.data(), enc.data(), compressed);

    // 5) Keyfile: escreve nome NORMALIZADO + hash (se existir)
    const unsigned short namelen = (unsigned short)norm.size();

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    DWORD dwWritten = 0;

    if (keys != INVALID_HANDLE_VALUE)
    {
        WriteFile(keys, &namelen, sizeof(namelen), &dwWritten, 0);
        WriteFile(keys, norm.c_str(), namelen, &dwWritten, 0);
        WriteFile(keys, &hash, sizeof(hash), &dwWritten, 0);
    }

    LARGE_INTEGER m, p;
    m.QuadPart = 0;
    SetFilePointerEx(file, m, &p, FILE_CURRENT);

    entry.offset += (unsigned int)p.QuadPart;

    WriteFile(file, &entry, sizeof(entry), &dwWritten, 0);
    WriteFile(file, enc.data(), compressed, &dwWritten, 0);
#else
    if (keys != -1)
    {
        ::write(keys, &namelen, sizeof(namelen));
        ::write(keys, norm.c_str(), namelen);
        ::write(keys, &hash, sizeof(hash));
    }

    entry.offset += (unsigned int)::lseek(file, 0, SEEK_CUR);

    ::write(file, &entry, sizeof(entry));
    ::write(file, enc.data(), compressed);
#endif

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    LeaveCriticalSection(&mutex);
#else
    pthread_mutex_unlock(&mutex);
#endif
    return true;
}
