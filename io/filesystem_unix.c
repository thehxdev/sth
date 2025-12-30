#ifdef __cplusplus
extern "C" {
#endif

int sth_io_file_exists(const char *path) {
    struct stat statbuf;
    if (stat(file_path, &statbuf) < 0) {
        return STH_FAILED;
    }
    return STH_OK;
}

int sth_io_rename(const char *old_path, const char *new_path) {
    if (rename(old_path, new_path) < 0) {
        return STH_FAILED;
    }
    return STH_OK;
}

int sth_io_mkdir_if_not_exists(const char *path) {
    int res = mkdir(path, 0755);
    if (res < 0) {
        if (errno == EEXIST)
            return STH_OK;
        return STH_FAILED;
    }
    return STH_OK;
}

#ifdef __cplusplus
}
#endif
