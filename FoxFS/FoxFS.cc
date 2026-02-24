#include <FoxFS.h>
#include "FileSystem.h"

#include <string>
#include <cwchar>   // mbsrtowcs, wcsrtombs
#include <cstdlib>  // size_t

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    #include <windows.h>
#endif

static inline std::wstring AnsiToWide(const char* s)
{
    if (!s) return std::wstring();

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    UINT usedCp = CP_UTF8;
    int n = MultiByteToWideChar(usedCp, 0, s, -1, NULL, 0);
    if (n <= 0) {
        usedCp = CP_ACP;
        n = MultiByteToWideChar(usedCp, 0, s, -1, NULL, 0);
    }
    if (n <= 0) return std::wstring();

    std::wstring out;
    out.resize((size_t)n); // inclui '\0'
    MultiByteToWideChar(usedCp, 0, s, -1, &out[0], n);
    if (!out.empty() && out.back() == L'\0') out.pop_back();
    return out;
#else
    std::mbstate_t st{};
    const char* src = s;
    size_t len = std::mbsrtowcs(NULL, &src, 0, &st);
    if (len == (size_t)-1) return std::wstring();

    std::wstring out;
    out.resize(len);
    st = std::mbstate_t{};
    src = s;
    std::mbsrtowcs(&out[0], &src, len, &st);
    return out;
#endif
}

static inline std::string WideToAnsi(const wchar_t* ws)
{
    if (!ws) return std::string();

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    UINT usedCp = CP_UTF8;
    int n = WideCharToMultiByte(usedCp, 0, ws, -1, NULL, 0, NULL, NULL);
    if (n <= 0) {
        usedCp = CP_ACP;
        n = WideCharToMultiByte(usedCp, 0, ws, -1, NULL, 0, NULL, NULL);
    }
    if (n <= 0) return std::string();

    std::string out;
    out.resize((size_t)n); // inclui '\0'
    WideCharToMultiByte(usedCp, 0, ws, -1, &out[0], n, NULL, NULL);
    if (!out.empty() && out.back() == '\0') out.pop_back();
    return out;
#else
    std::mbstate_t st{};
    const wchar_t* src = ws;
    size_t len = std::wcsrtombs(NULL, &src, 0, &st);
    if (len == (size_t)-1) return std::string();

    std::string out;
    out.resize(len);
    st = std::mbstate_t{};
    src = ws;
    std::wcsrtombs(&out[0], &src, len, &st);
    return out;
#endif
}

extern "C"
{
    PFoxFS FOXFS_EXPORT FOXFS_API FoxFS_Create()
    {
        return reinterpret_cast<PFoxFS>(new FoxFS::FileSystem());
    }

    void FOXFS_EXPORT FOXFS_API FoxFS_Destroy(PFoxFS manager)
    {
        delete reinterpret_cast<FoxFS::FileSystem*>(manager);
    }

    int FOXFS_EXPORT FOXFS_API FoxFS_SetKeyServerA(PFoxFS manager, const char* hostname, unsigned short port)
    {
        (void)manager; (void)hostname; (void)port;
        return 0;
    }

    int FOXFS_EXPORT FOXFS_API FoxFS_SetKeyServerW(PFoxFS manager, const wchar_t* hostname, unsigned short port)
    {
        (void)manager; (void)hostname; (void)port;
        return 0;
    }

    int FOXFS_EXPORT FOXFS_API FoxFS_LoadA(PFoxFS manager, const char* filename)
    {
        if (!manager || !filename) return -1;
        std::wstring w = AnsiToWide(filename);
        return reinterpret_cast<FoxFS::FileSystem*>(manager)->load(w.c_str());
    }

    int FOXFS_EXPORT FOXFS_API FoxFS_LoadW(PFoxFS manager, const wchar_t* filename)
    {
        if (!manager || !filename) return -1;
        return reinterpret_cast<FoxFS::FileSystem*>(manager)->load(filename);
    }

    int FOXFS_EXPORT FOXFS_API FoxFS_UnloadA(PFoxFS manager, const char* filename)
    {
        if (!manager || !filename) return -1;
        std::wstring w = AnsiToWide(filename);
        return reinterpret_cast<FoxFS::FileSystem*>(manager)->unload(w.c_str());
    }

    int FOXFS_EXPORT FOXFS_API FoxFS_UnloadW(PFoxFS manager, const wchar_t* filename)
    {
        if (!manager || !filename) return -1;
        return reinterpret_cast<FoxFS::FileSystem*>(manager)->unload(filename);
    }

    unsigned int FOXFS_EXPORT FOXFS_API FoxFS_SizeA(PFoxFS manager, const char* filename)
    {
        if (!manager || !filename) return 0;
        return reinterpret_cast<FoxFS::FileSystem*>(manager)->size(filename);
    }

    unsigned int FOXFS_EXPORT FOXFS_API FoxFS_SizeW(PFoxFS manager, const wchar_t* filename)
    {
        if (!manager || !filename) return 0;
        std::string a = WideToAnsi(filename);
        return reinterpret_cast<FoxFS::FileSystem*>(manager)->size(a.c_str());
    }

    int FOXFS_EXPORT FOXFS_API FoxFS_ExistsA(PFoxFS manager, const char* filename)
    {
        if (!manager || !filename) return -1;
        return reinterpret_cast<FoxFS::FileSystem*>(manager)->exists(filename);
    }

    int FOXFS_EXPORT FOXFS_API FoxFS_ExistsW(PFoxFS manager, const wchar_t* filename)
    {
        if (!manager || !filename) return -1;
        std::string a = WideToAnsi(filename);
        return reinterpret_cast<FoxFS::FileSystem*>(manager)->exists(a.c_str());
    }

    int FOXFS_EXPORT FOXFS_API FoxFS_GetA(PFoxFS manager, const char* filename, void* buffer, unsigned int maxsize, unsigned int* outsize)
    {
        if (!manager || !filename) return -1;
        return reinterpret_cast<FoxFS::FileSystem*>(manager)->get(filename, buffer, maxsize, outsize);
    }

    int FOXFS_EXPORT FOXFS_API FoxFS_GetW(PFoxFS manager, const wchar_t* filename, void* buffer, unsigned int maxsize, unsigned int* outsize)
    {
        if (!manager || !filename) return -1;
        std::string a = WideToAnsi(filename);
        return reinterpret_cast<FoxFS::FileSystem*>(manager)->get(a.c_str(), buffer, maxsize, outsize);
    }
}
