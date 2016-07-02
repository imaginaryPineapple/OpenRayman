#include <platform/file.h>
#ifdef _WIN32
#include <windows.h>
#include <shlwapi.h>
#include <shellapi.h>
#include <locale>
#include <codecvt>
#else
#include <unistd.h>
#include <limits.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ftw.h>
#include <cstdio>
#endif

namespace openrayman
{
    bool file::exists(const std::string& path)
    {
#ifdef _WIN32
        DWORD attrib = GetFileAttributes(fix_string(path).c_str());
        return attrib != INVALID_FILE_ATTRIBUTES;
#else
        return access(fix_string(path).c_str(), F_OK | R_OK) != -1;
#endif
    }

    const std::string file::fix_string(const std::string& string)
    {
        std::string new_str(string);
        for(size_t n = 0; n < new_str.length(); n++)
        {
            if(new_str[n] == '/' || new_str[n] == '\\')
                new_str.replace(n, 1, 1, path_separator);
        }
        return new_str;
    }

    void file::create_directory(const std::string& path)
    {
#ifdef _WIN32
        CreateDirectory(fix_string(path).c_str(), nullptr);
#else
        mkdir(fix_string(path).c_str(), 0775);
#endif
    }

#ifndef _WIN32
    // http://stackoverflow.com/questions/3184445/how-to-clear-directory-contents-in-c-on-linux-basically-i-want-to-do-rm-rf/3184915#3184915
    int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
    {
        int rv = remove(fpath);
        if (rv)
            perror(fpath);
        return rv;
    }
#endif

    void file::delete_directory(const std::string& path)
    {
#ifdef _WIN32
        SHFILEOPSTRUCT fop;
        fop.hwnd = nullptr;
        fop.wFunc = FO_DELETE;
        fop.pFrom = std::string(path + "\0").c_str();
        fop.pTo = nullptr;
        fop.fFlags = FOF_NOCONFIRMATION | FOF_SILENT | FOF_ALLOWUNDO;
        fop.fAnyOperationsAborted = false;
        fop.lpszProgressTitle = nullptr;
        fop.hNameMappings = nullptr;
        SHFileOperation(&fop);
#else
        nftw(path.c_str(), &unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
#endif
    }
}
