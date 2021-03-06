#pragma once

#include "mn/Exports.h"
#include "mn/Stream.h"
#include "mn/Str.h"

namespace mn
{
	typedef struct IFile* File;

	// just forward declaration because this language require this kind of thing
	MN_EXPORT int64_t
	file_cursor_pos(File handle);

	MN_EXPORT bool
	file_cursor_move(File handle, int64_t offset);

	MN_EXPORT bool
	file_cursor_set(File handle, int64_t absolute);

	MN_EXPORT bool
	file_cursor_move_to_start(File handle);

	MN_EXPORT bool
	file_cursor_move_to_end(File handle);

	struct IFile final: IStream
	{
		union
		{
			void* winos_handle;
			int linux_handle;
			int macos_handle;
		};

		MN_EXPORT virtual void
		dispose() override;

		MN_EXPORT virtual size_t
		read(Block data) override;

		MN_EXPORT virtual size_t
		write(Block data) override;

		MN_EXPORT virtual int64_t
		size() override;

		virtual int64_t
		cursor_operation(STREAM_CURSOR_OP op, int64_t arg) override
		{
			switch (op)
			{
			case STREAM_CURSOR_GET:
				return file_cursor_pos(this);
			case STREAM_CURSOR_MOVE:
				file_cursor_move(this, arg);
				return file_cursor_pos(this);
			case STREAM_CURSOR_SET:
				file_cursor_set(this, arg);
				return file_cursor_pos(this);
			case STREAM_CURSOR_START:
				file_cursor_move_to_start(this);
				return 0;
			case STREAM_CURSOR_END:
				file_cursor_move_to_end(this);
				return file_cursor_pos(this);
			default:
				assert(false && "unreachable");
				return STREAM_CURSOR_ERROR;
			}
		}
	};


	/**
	 * @brief      Converts a string from utf-8 to os-specific encoding
	 *
	 * @param[in]  utf8  The utf 8
	 *
	 * @return     A Block of memory containing the encoded string
	 */
	MN_EXPORT Block
	to_os_encoding(const Str& utf8, Allocator allocator = memory::tmp());

	/**
	 * @brief      Converts a string from utf-8 to os-specific encoding
	 *
	 * @param[in]  utf8  The utf 8
	 *
	 * @return     A Block of memory containing the encoded string
	 */
	MN_EXPORT Block
	to_os_encoding(const char* utf8, Allocator allocator = memory::tmp());

	/**
	 * @brief      Converts from os-specific encoding to utf-8
	 *
	 * @param[in]  os_str  The operating system string
	 *
	 * @return     A String containing the encoded string
	 */
	MN_EXPORT Str
	from_os_encoding(Block os_str, Allocator allocator = memory::tmp());

	/**
	 * @brief      OPEN_MODE enum
	 *
	 * - **CREATE_ONLY**: creates the file if it doesn't exist. if it exists it fails.
	 * - **CREATE_OVERWRITE**: creates the file if it doesn't exist. if it exists it overwrite it.
	 * - **CREATE_APPEND**: creates the file if it doesn't exist. if it exists it appends to it.
	 * - **OPEN_ONLY**: opens the file if it exists. fails otherwise.
	 * - **OPEN_OVERWRITE**: opens the file if it exist and overwrite its content. if it doesn't exist it fails.
	 * - **OPEN_APPEND**: opens the file it it exists and append to its content. if it doesn't exist it fails.
	 */
	enum class OPEN_MODE
	{
		CREATE_ONLY,
		CREATE_OVERWRITE,
		CREATE_APPEND,
		OPEN_ONLY,
		OPEN_OVERWRITE,
		OPEN_APPEND
	};

	enum SHARE_MODE
	{
		SHARE_MODE_READ,
		SHARE_MODE_WRITE,
		SHARE_MODE_DELETE,
		SHARE_MODE_READ_WRITE,
		SHARE_MODE_READ_DELETE,
		SHARE_MODE_WRITE_DELETE,
		SHARE_MODE_ALL,
		SHARE_MODE_NONE,
	};

	/**
	 * @brief      IO_MODE enum
	 * 
	 * - **READ**: only performs reads to the file
	 * - **WRITE**: only performs writes to the file
	 * - **READ_WRITE**: performs both reads and writes to the file
	 */
	enum class IO_MODE
	{
		READ,
		WRITE,
		READ_WRITE
	};

	/**
	 * @brief      Returns a file handle pointing to the standard output
	 */
	MN_EXPORT File
	file_stdout();

	/**
	 * @brief      Returns a file handle pointing to the standard error
	 */
	MN_EXPORT File
	file_stderr();

	/**
	 * @brief      Returns a file handle pointing to the standard input
	 */
	MN_EXPORT File
	file_stdin();

	/**
	 * @brief      Opens a file
	 *
	 * @param[in]  filename   The filename
	 * @param[in]  io_mode    The i/o mode
	 * @param[in]  open_mode  The open mode
	 */
	MN_EXPORT File
	file_open(const char* filename, IO_MODE io_mode, OPEN_MODE open_mode, SHARE_MODE share_mode = SHARE_MODE_ALL);

	inline static File
	file_open(const Str& filename, IO_MODE io_mode, OPEN_MODE open_mode, SHARE_MODE share_mode = SHARE_MODE_ALL)
	{
		return file_open(filename.ptr, io_mode, open_mode, share_mode);
	}

	/**
	 * @brief      Closes a file
	 */
	MN_EXPORT void
	file_close(File handle);

	/**
	 * @brief      Validates a file handle
	 */
	MN_EXPORT bool
	file_valid(File handle);

	/**
	 * @brief      Writes a block of bytes into a file
	 *
	 * @param[in]  handle  The file handle
	 * @param[in]  data    The data
	 * 
	 * @return     The written size in bytes
	 */
	MN_EXPORT size_t
	file_write(File handle, Block data);

	/**
	 * @brief      Reads a block of bytes from a file
	 *
	 * @param[in]  handle  The file handle
	 * @param[in]  data    The data
	 *
	 * @return     The read size in bytes
	 */
	MN_EXPORT size_t
	file_read(File handle, Block data);

	/**
	 * @brief      Returns the size in bytes of a given file
	 */
	MN_EXPORT int64_t
	file_size(File handle);

	/**
	 * @brief      Returns the cursor position of the given file
	 */
	MN_EXPORT int64_t
	file_cursor_pos(File handle);

	/**
	 * @brief      Moves the file cursor by the given offset
	 *
	 * @param[in]  handle  The file handle
	 * @param[in]  offset  The offset
	 */
	MN_EXPORT bool
	file_cursor_move(File handle, int64_t offset);

	/**
	 * @brief      Set the file cursor by the given offset
	 *
	 * @param[in]  handle  The file handle
	 * @param[in]  absolute The offset
	 */
	MN_EXPORT bool
	file_cursor_set(File handle, int64_t absolute);

	/**
	 * @brief      Sets the file cursor to the start
	 */
	MN_EXPORT bool
	file_cursor_move_to_start(File handle);

	/**
	 * @brief      Sets the file cursor to the end
	 */
	MN_EXPORT bool
	file_cursor_move_to_end(File handle);

	// specify a region of the file to be locked
	// locks can't overlap -> file_lock will fail
	// you can lock a region beyond EOF to coordinate record addition to a file
	MN_EXPORT bool
	file_write_try_lock(File handle, int64_t offset, int64_t size);

	MN_EXPORT void
	file_write_lock(File handle, int64_t offset, int64_t size);

	MN_EXPORT bool
	file_write_unlock(File handle, int64_t offset, int64_t size);

	// specify a region of the file to be locked
	// read locks allow multiple readers
	MN_EXPORT bool
	file_read_try_lock(File handle, int64_t offset, int64_t size);

	MN_EXPORT void
	file_read_lock(File handle, int64_t offset, int64_t size);

	MN_EXPORT bool
	file_read_unlock(File handle, int64_t offset, int64_t size);
}