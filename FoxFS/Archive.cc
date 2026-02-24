// Archive.cc (COMPLETO)
#include <FoxFS.h>
#include "Archive.h"

#include <cryptopp/modes.h>
#include <cryptopp/blowfish.h>
#include <cryptopp/crc.h>

#include "../xxhash/xxhash.h"
#include "../lz4/lz4.h"
#include "../lz4/lz4hc.h"

#include <algorithm>
#include <string>
#include <vector>

namespace FoxFS
{
#ifdef MIN
#undef MIN
#endif
	template <typename A, typename B> static inline A MIN(A a, B b) { return a < b ? a : (A)b; }

	// Normalização igual ao teu FileSystem.cc:
	// - lowercase
	// - '\' -> '/'
	// - strip "/ymir work/"
	// - fonts: "tahoma:12.fnt" -> "tahoma.fnt"
	static inline void NormalizeFoxPath(std::string& fn)
	{
		std::transform(fn.begin(), fn.end(), fn.begin(), ::tolower);
		for (size_t i = 0; i < fn.size(); ++i)
			if (fn[i] == '\\') fn[i] = '/';

		const std::string ymir = "/ymir work/";
		size_t pos = fn.find(ymir);
		if (pos != std::string::npos)
		{
			fn = fn.substr(pos + ymir.size());
		}
		else
		{
			const std::string ymir2 = "ymir work/";
			if (fn.rfind(ymir2, 0) == 0)
				fn = fn.substr(ymir2.size());
		}

		while (fn.rfind("./", 0) == 0) fn = fn.substr(2);
		while (!fn.empty() && fn[0] == '/') fn.erase(fn.begin());

		size_t colonPos = fn.find(':');
		if (colonPos != std::string::npos && fn.find(".fnt") != std::string::npos)
			fn = fn.substr(0, colonPos) + ".fnt";
	}

	unsigned int Archive::generateFilenameIndex(const char* filename)
	{
		if (!filename) return 0;

		std::string fn = filename;
		NormalizeFoxPath(fn);

		unsigned int index = 0;
		CryptoPP::CRC32 crc;
		crc.CalculateDigest(reinterpret_cast<unsigned char*>(&index),
		                    reinterpret_cast<const unsigned char*>(fn.c_str()),
		                    static_cast<unsigned int>(fn.length()));
		return index;
	}

	Archive::Archive() :
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
		file(INVALID_HANDLE_VALUE)
#else
		file(-1)
#endif
	{
		memset(this->key, 0, sizeof(this->key));
		memset(this->iv, 0, sizeof(this->iv));
		memset(&header, 0, sizeof(header));
		memset(filename, 0, sizeof(filename));
	}

	Archive::~Archive() { unload(); }

	const wchar_t* Archive::getFilename() const { return filename; }

	int Archive::exists(const char* filename_) const
	{
		unsigned int index = Archive::generateFilenameIndex(filename_);

		std::map<unsigned int, FileListEntry>::const_iterator iter = files.find(index);
		return (iter != files.end()) ? ERROR_OK : ERROR_FILE_WAS_NOT_FOUND;
	}

	unsigned int Archive::size(const char* filename_) const
	{
		unsigned int index = Archive::generateFilenameIndex(filename_);

		std::map<unsigned int, FileListEntry>::const_iterator iter = files.find(index);
		return (iter != files.end()) ? iter->second.decompressed : 0;
	}

	int Archive::get(const char* filename_, void* buffer, unsigned int maxsize, unsigned int* outsize) const
	{
		unsigned int index = Archive::generateFilenameIndex(filename_);

		std::map<unsigned int, FileListEntry>::const_iterator iter = files.find(index);
		if (iter == files.end())
			return ERROR_FILE_WAS_NOT_FOUND;

		const FileListEntry& e = iter->second;

		// sem compressão
		if (e.size == e.decompressed)
		{
			unsigned int len = MIN(maxsize, e.size);

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
			DWORD dwRead = 0;
			LARGE_INTEGER m;
			m.QuadPart = e.offset;
			SetFilePointerEx(file, m, 0, FILE_BEGIN);
			ReadFile(file, buffer, len, &dwRead, 0);
#else
			lseek(file, e.offset, SEEK_SET);
			read(file, buffer, len);
#endif
			try
			{
				CryptoPP::CFB_Mode<CryptoPP::Blowfish>::Decryption dec(this->key, 32, this->iv);
				dec.ProcessData(reinterpret_cast<unsigned char*>(buffer),
				                reinterpret_cast<unsigned char*>(buffer),
				                len);
			}
			catch (...)
			{
				return ERROR_DECRYPTION_FAILED;
			}

			if (len == e.size && XXH32(reinterpret_cast<char*>(buffer), len, FOXFS_MAGIC) != e.hash)
				return ERROR_CORRUPTED_FILE;

			if (outsize) *outsize = len;
			return ERROR_OK;
		}

		// com compressão
		std::vector<unsigned char> tmp(e.size + e.decompressed);

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
		DWORD dwRead = 0;
		LARGE_INTEGER m;
		m.QuadPart = e.offset;
		SetFilePointerEx(file, m, 0, FILE_BEGIN);
		ReadFile(file, &tmp[0], e.size, &dwRead, 0);
#else
		lseek(file, e.offset, SEEK_SET);
		read(file, &tmp[0], e.size);
#endif

		try
		{
			CryptoPP::CFB_Mode<CryptoPP::Blowfish>::Decryption dec(this->key, 32, this->iv);
			dec.ProcessData(&tmp[0], &tmp[0], e.size);
		}
		catch (...)
		{
			return ERROR_DECRYPTION_FAILED;
		}

		if (LZ4_decompress_fast(reinterpret_cast<char*>(&tmp[0]),
		                        reinterpret_cast<char*>(&tmp[e.size]),
		                        e.decompressed) != (int)e.size)
			return ERROR_DECOMPRESSION_FAILED;

		if (XXH32(reinterpret_cast<char*>(&tmp[e.size]), e.decompressed, FOXFS_MAGIC) != e.hash)
			return ERROR_CORRUPTED_FILE;

		unsigned int outLen = MIN(maxsize, e.decompressed);
		memcpy(buffer, &tmp[e.size], outLen);
		if (outsize) *outsize = outLen;

		return ERROR_OK;
	}

	int Archive::load(const wchar_t* filename_, const void* key_, const void* iv_)
	{
		unload();

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
		file = CreateFileW(filename_, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (file == INVALID_HANDLE_VALUE)
		{
			switch (GetLastError())
			{
			case ERROR_ACCESS_DENIED: return ERROR_ARCHIVE_ACCESS_DENIED;
			case ERROR_FILE_NOT_FOUND: return ERROR_ARCHIVE_NOT_FOUND;
			case ERROR_TOO_MANY_OPEN_FILES: return ERROR_ARCHIVE_NOT_READABLE;
			default: return ERROR_ARCHIVE_ACCESS_DENIED;
			}
		}

		wcscpy(this->filename, filename_);
		DWORD dwRead = 0;
		ReadFile(file, &header, sizeof(header), &dwRead, 0);
		DWORD size = GetFileSize(file, 0);
#else
		char tmpname[PATH_MAX + 1] = { 0 };
		wcstombs(tmpname, filename_, wcslen(filename_));
		file = ::open(tmpname, O_RDONLY | O_LARGEFILE);
		if (file == -1) return ERROR_ARCHIVE_NOT_FOUND;

		wcscpy(this->filename, filename_);
		read(file, &header, sizeof(header));
		struct stat st;
		fstat(file, &st);
		unsigned int size = st.st_size;
#endif

		if (header.magic != FOXFS_MAGIC)
		{
			unload();
			return ERROR_ARCHIVE_INVALID;
		}

		memcpy(this->key, header.key, 32);
		memcpy(this->iv, header.iv, 32);

		// XOR opcional com key/iv externos (se usares keyserver)
		if (key_)
		{
			for (int i = 0; i < 4; ++i)
				reinterpret_cast<unsigned long long*>(this->key)[i] ^=
					reinterpret_cast<const unsigned long long*>(key_)[i];
		}
		if (iv_)
		{
			for (int i = 0; i < 4; ++i)
				reinterpret_cast<unsigned long long*>(this->iv)[i] ^=
					reinterpret_cast<const unsigned long long*>(iv_)[i];
		}

		unsigned int offset = sizeof(header);
		while (offset < size)
		{
			TArchiveEntry entry;

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
			LARGE_INTEGER m;
			m.QuadPart = offset;
			SetFilePointerEx(file, m, 0, FILE_BEGIN);
			ReadFile(file, &entry, sizeof(entry), &dwRead, 0);
#else
			lseek(file, offset, SEEK_SET);
			read(file, &entry, sizeof(entry));
#endif
			offset = (unsigned int)(entry.offset + entry.size);

			FileListEntry le;
			le.offset = entry.offset;
			le.size = entry.size;
			le.decompressed = entry.decompressed;
			le.hash = entry.hash;
			le.name = entry.name;

			files.insert(std::map<unsigned int, FileListEntry>::value_type(le.name, le));
		}

		return ERROR_OK;
	}

	void Archive::unload()
	{
		files.clear();
		memset(filename, 0, sizeof(filename));
		memset(&header, 0, sizeof(header));
		memset(key, 0, sizeof(key));
		memset(iv, 0, sizeof(iv));

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
		if (file != INVALID_HANDLE_VALUE)
		{
			CloseHandle(file);
			file = INVALID_HANDLE_VALUE;
		}
#else
		if (file != -1)
		{
			close(file);
			file = -1;
		}
#endif
	}
}