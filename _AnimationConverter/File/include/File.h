//--------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef FILE_H
#define FILE_H

#ifdef FILELIBRARY_EXPORTS
#define FILELIBRARY_API __declspec(dllexport)
#else
#define FILELIBRARY_API __declspec(dllimport)
#endif

class File
{
public:

	typedef void* Handle;

	enum class Mode
	{
		READ = 0x7A000000,
		WRITE,
		READ_WRITE
	};

	enum class Location
	{
		BEGIN = 0x7B000000,
		CURRENT,
		END
	};

	enum class Error
	{
		SUCCESS = 0x7C000000,
		OPEN_FAIL,
		CLOSE_FAIL,
		WRITE_FAIL,
		READ_FAIL,
		SEEK_FAIL,
		TELL_FAIL,
		FLUSH_FAIL,
		SEARCH_FAIL,
		UNDEFINED
	};

public:

	FILELIBRARY_API static File::Error Open(File::Handle& fh, const char* const fileName, File::Mode mode) noexcept;
	FILELIBRARY_API static File::Error Close(File::Handle& fh) noexcept;
	FILELIBRARY_API static File::Error Write(File::Handle fh, const void* const buffer, const DWORD inSize) noexcept;
	FILELIBRARY_API static File::Error Read(File::Handle fh, void* const _buffer, const DWORD _size) noexcept;
	FILELIBRARY_API static File::Error Seek(File::Handle fh, File::Location location, int offset) noexcept;
	FILELIBRARY_API static File::Error Tell(File::Handle fh, DWORD& offset) noexcept;
	FILELIBRARY_API static File::Error Flush(File::Handle fh) noexcept;

	FILELIBRARY_API static bool IsHandleValid(File::Handle fh) noexcept;
	FILELIBRARY_API static unsigned int FindHash(char* buffer, unsigned int buffLen) noexcept;
	FILELIBRARY_API static File::Error SearchDirectory(const char* searchString, const char*& outNameArray) noexcept;
};

#endif

// ---  End of File ---------------
