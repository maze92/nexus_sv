// FileSystem.cc (REVISADO)
#include <FoxFS.h>
#include "FileSystem.h"

#include <algorithm>
#include <string>
#include <cwctype>   // towlower
#include <cctype>    // std::tolower

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
LRESULT CALLBACK SplashScreenProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}
#endif

namespace FoxFS
{
    static inline void NormalizeFoxPath(std::string& fn)
    {
        std::transform(fn.begin(), fn.end(), fn.begin(),
            [](unsigned char c) { return (char)std::tolower(c); });

        for (size_t i = 0; i < fn.length(); ++i)
        {
            if (fn[i] == '\\') fn[i] = '/';
        }

        const std::string ymir = "/ymir work/";
        size_t pos = fn.find(ymir);
        if (pos != std::string::npos)
        {
            fn = fn.substr(pos + ymir.length());
        }
        else
        {
            const std::string ymir2 = "ymir work/";
            if (fn.rfind(ymir2, 0) == 0)
                fn = fn.substr(ymir2.length());
        }

        while (fn.rfind("./", 0) == 0) fn = fn.substr(2);
        while (!fn.empty() && fn[0] == '/') fn.erase(fn.begin());

        size_t colonPos = fn.find(':');
        if (colonPos != std::string::npos && fn.find(".fnt") != std::string::npos)
        {
            fn = fn.substr(0, colonPos) + ".fnt";
        }
    }

    FileSystem::FileSystem()
    {
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
        InitializeCriticalSection(&mutex);
#else
        pthread_mutex_init(&mutex, 0);
#endif
    }

    FileSystem::~FileSystem()
    {
        for (std::map<std::basic_string<wchar_t>, Archive*>::iterator iter = archives.begin(); iter != archives.end(); ++iter)
        {
            delete iter->second;
        }

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
        DeleteCriticalSection(&mutex);
#else
        pthread_mutex_destroy(&mutex);
#endif
    }

    int FileSystem::load(const wchar_t* filename)
    {
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
        EnterCriticalSection(&mutex);
#else
        pthread_mutex_lock(&mutex);
#endif

        int r = Archive::ERROR_OK;

        std::basic_string<wchar_t> fn = filename ? filename : L"";
        std::transform(fn.begin(), fn.end(), fn.begin(), towlower);

        std::map<std::basic_string<wchar_t>, Archive*>::iterator iter = archives.find(fn);
        if (iter == archives.end())
        {
            Archive* a = new Archive();
            if (a)
            {
                char tmpname[
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
                    MAX_PATH + 1
#else
                    PATH_MAX + 1
#endif
                ] = { 0 };

                if (filename)
                    wcstombs(tmpname, filename, wcslen(filename));

                std::map<std::string, KeyInfo>::iterator keyiter = keys.find(tmpname);
                void* key = 0, * iv = 0;
                if (keyiter != keys.end())
                {
                    key = keyiter->second.key;
                    iv  = keyiter->second.iv;
                }

                if ((r = a->load(filename, key, iv)) == Archive::ERROR_OK)
                {
                    archives.insert(std::map<std::basic_string<wchar_t>, Archive*>::value_type(fn, a));
                }
                else
                {
                    delete a;
                }
            }
        }

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
        LeaveCriticalSection(&mutex);
#else
        pthread_mutex_unlock(&mutex);
#endif
        return r;
    }

    int FileSystem::unload(const wchar_t* filename)
    {
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
        EnterCriticalSection(&mutex);
#else
        pthread_mutex_lock(&mutex);
#endif

        std::basic_string<wchar_t> fn = filename ? filename : L"";
        std::transform(fn.begin(), fn.end(), fn.begin(), towlower);

        std::map<std::basic_string<wchar_t>, Archive*>::iterator iter = archives.find(fn);

        int r = Archive::ERROR_ARCHIVE_NOT_FOUND;
        if (iter != archives.end())
        {
            delete iter->second;
            archives.erase(iter);
            r = Archive::ERROR_OK;
        }

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
        LeaveCriticalSection(&mutex);
#else
        pthread_mutex_unlock(&mutex);
#endif
        return r;
    }

    unsigned int FileSystem::size(const char* filename) const
    {
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
        EnterCriticalSection(&mutex);
#else
        pthread_mutex_lock(&mutex);
#endif

        std::string fn = filename ? filename : "";
        NormalizeFoxPath(fn);

        unsigned int r = 0;
        for (std::map<std::basic_string<wchar_t>, Archive*>::const_iterator iter = archives.begin(); iter != archives.end(); ++iter)
        {
            if ((r = iter->second->size(fn.c_str())) > 0)
                break;
        }

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
        LeaveCriticalSection(&mutex);
#else
        pthread_mutex_unlock(&mutex);
#endif
        return r;
    }

    int FileSystem::exists(const char* filename) const
    {
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
        EnterCriticalSection(&mutex);
#else
        pthread_mutex_lock(&mutex);
#endif

        std::string fn = filename ? filename : "";
        NormalizeFoxPath(fn);

        int r = Archive::ERROR_FILE_WAS_NOT_FOUND;
        for (std::map<std::basic_string<wchar_t>, Archive*>::const_iterator iter = archives.begin(); iter != archives.end(); ++iter)
        {
            if ((r = iter->second->exists(fn.c_str())) == Archive::ERROR_OK)
                break;
        }

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
        LeaveCriticalSection(&mutex);
#else
        pthread_mutex_unlock(&mutex);
#endif
        return r;
    }

    int FileSystem::get(const char* filename, void* buffer, unsigned int maxsize, unsigned int* outsize) const
    {
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
        EnterCriticalSection(&mutex);
#else
        pthread_mutex_lock(&mutex);
#endif

        std::string fn = filename ? filename : "";
        NormalizeFoxPath(fn);

        int r = Archive::ERROR_FILE_WAS_NOT_FOUND;
        for (std::map<std::basic_string<wchar_t>, Archive*>::const_iterator iter = archives.begin(); iter != archives.end(); ++iter)
        {
            if ((r = iter->second->get(fn.c_str(), buffer, maxsize, outsize)) == Archive::ERROR_OK)
                break;
        }

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
        LeaveCriticalSection(&mutex);
#else
        pthread_mutex_unlock(&mutex);
#endif
        return r;
    }
}
