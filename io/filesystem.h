#ifndef _STH_IO_FILESYSTEM_H_
#define _STH_IO_FILESYSTEM_H_

#ifdef __cplusplus
extern "C" {
#endif

int sth_io_file_exists(const char *path);

int sth_io_rename(const char *old_path, const char *new_path);

int sth_io_mkdir_if_not_exists(const char *path);

#ifdef __cplusplus
}
#endif

#endif // _STH_IO_FILESYSTEM_H_
