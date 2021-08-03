//--------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "File.h"

namespace Azul
{

	bool File::IsHandleValid(File::Handle fh) noexcept
	{
		DWORD q;

		// get handle information.
		// return true if file is valid.
		return (GetHandleInformation(fh, &q));
	}

	File::Error File::Open(File::Handle& fh, const char* const fileName, File::Mode mode) noexcept
	{
		// Create a file-------
		/*
			// Macro.
			#define CreateFile  CreateFileA

			// function.
			HANDLE CreateFileA(
								LPCSTR                lpFileName, // file name to create or open.
								DWORD                 dwDesiredAccess, // type of access.. read, write or both.
								DWORD                 dwShareMode, // share file with other process?
								LPSECURITY_ATTRIBUTES lpSecurityAttributes, // security values: encrypt file?
								DWORD                 dwCreationDisposition, // creation method: open always or create new?
								DWORD                 dwFlagsAndAttributes, // flags and attributes of file.
								HANDLE                hTemplateFile // reference file to open a new file.
								);
		*/
		
		// file mode to use.
		DWORD fileMode;

		if (mode == File::Mode::READ)
		{
			fileMode = GENERIC_READ;
		}
		else if (mode == File::Mode::WRITE)
		{
			fileMode = GENERIC_WRITE;
		}
		else
		{
			fileMode = GENERIC_READ | GENERIC_WRITE;
		}

		// open or create a new file.
		fh = CreateFile(fileName, fileMode, 0, nullptr, OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, nullptr);

		// Verify file is created. 
		if (fh != INVALID_HANDLE_VALUE)
		{
			// return success.
			return (File::Error::SUCCESS);
		}
		else
		{
			return (File::Error::OPEN_FAIL);
		}
	}

	File::Error File::Close(File::Handle& fh) noexcept
	{
		if (!File::IsHandleValid(fh))
		{
			return (File::Error::CLOSE_FAIL);
		}
		
		// Close the given file handle.
		// verify file is closed.
		if (CloseHandle(fh))
		{
			// return success.
			return (File::Error::SUCCESS);
		}
		else
		{
			// return fail.
			return (File::Error::CLOSE_FAIL);
		}
	}

	File::Error File::Write(File::Handle fh, const void* const buffer, const DWORD inSize) noexcept
	{
		// Read from given file.
		/*
		BOOL WriteFile(
						HANDLE       hFile, // handle of file to write.
						LPCVOID      lpBuffer, // buffer from which data is to be written.
						DWORD        nNumberOfBytesToWrite, // number of bytes to write.
						LPDWORD      lpNumberOfBytesWritten, // count of data written in synchronous mode.
						LPOVERLAPPED lpOverlapped // is the file opened with asynchronous operation?
					  );
		*/

		if (!File::IsHandleValid(fh))
		{
			return (File::Error::WRITE_FAIL);
		}

		// count of data written.
		DWORD bytesWritten = 0;

		// verify file is written.
		if (WriteFile(fh, buffer, inSize, &bytesWritten, nullptr))
		{
			// return success.
			return (File::Error::SUCCESS);
		}
		else
		{
			return (File::Error::WRITE_FAIL);
		}
	}

	File::Error File::Read(File::Handle fh, void* const buffer, const DWORD inSize) noexcept
	{
		// Read from given file.
		/*
		BOOL ReadFile(
					   HANDLE       hFile, // handle of file to read.
					   LPVOID       lpBuffer, // buffer to which data is to be read.
					   DWORD        nNumberOfBytesToRead, //number of bytes to read.
					   LPDWORD      lpNumberOfBytesRead, // count of data read in synchronous mode.
					   LPOVERLAPPED lpOverlapped // is the file opened with asynchronous operation?
					 );
		*/

		if (!File::IsHandleValid(fh))
		{
			return (File::Error::READ_FAIL);
		}

		// count of data read.
		DWORD bytesRead = 0;

		// verify file is read.
		if (ReadFile(fh, buffer, inSize, &bytesRead, nullptr))
		{
			// return success.
			return (File::Error::SUCCESS);
		}
		else
		{
			return (File::Error::READ_FAIL);
		}
	}

	File::Error File::Seek(File::Handle fh, File::Location location, int offset) noexcept
	{
		// set the file pointer.
		/*
		DWORD SetFilePointer(
							 HANDLE hFile, // handle of file to move.
							 LONG   lDistanceToMove, // lower order of bits to move(positive move right, negative move left).
							 PLONG  lpDistanceToMoveHigh, //higher order of bits to move.
							 DWORD  dwMoveMethod // starting point.
							);
		*/

		if (!File::IsHandleValid(fh))
		{
			return (File::Error::SEEK_FAIL);
		}

		// start point of file to move pointer.
		DWORD startPoint;

		if (location == File::Location::BEGIN)
		{
			startPoint = FILE_BEGIN;
		}
		else if (location == File::Location::CURRENT)
		{
			startPoint = FILE_CURRENT;
		}
		else
		{
			startPoint = FILE_END;
		}

		// verify file pointer is set.
		if (SetFilePointer(fh, offset, nullptr, startPoint) != INVALID_SET_FILE_POINTER)
		{
			// return success.
			return (File::Error::SUCCESS);
		}
		else
		{
			return (File::Error::SEEK_FAIL);
		}
	}

	File::Error File::Tell(File::Handle fh, DWORD& offset) noexcept
	{
		if (!File::IsHandleValid(fh))
		{
			return (File::Error::TELL_FAIL);
		}

		// get the file pointer.
		offset = SetFilePointer(fh, 0, nullptr, FILE_CURRENT);

		// verify file pointer is set.
		if (offset != INVALID_SET_FILE_POINTER)
		{
			// return success.
			return (File::Error::SUCCESS);
		}
		else
		{
			return (File::Error::TELL_FAIL);
		}
	}

	File::Error File::Flush(File::Handle fh) noexcept
	{
		if (!File::IsHandleValid(fh))
		{
			return (File::Error::FLUSH_FAIL);
		}

		// verify flush is success.
		if (FlushFileBuffers(fh))
		{
			// return success.
			return (File::Error::SUCCESS);
		}
		else
		{
			return (File::Error::FLUSH_FAIL);
		}
	}

}

// ---  End of File ---------------
