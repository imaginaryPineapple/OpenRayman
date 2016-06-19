#ifndef DSB_DECOMPILER_H
#define DSB_DECOMPILER_H

#include <data_extractor/data_decoder.h>
#include <string>
#include <iostream>
#include <fstream>

namespace openrayman
{
    // Decodes and decompiles DSB files into database files.
    class dsb_decompiler
    {
public:
        // Decompiles the specified DSB file into the target file.
        // This function assumes that the source file exists and that the target location is accessible.
        // Returns true if the decompilation succeeded.
        bool decompile_dsb(const std::string& source, const std::string& target);
private:
        void decompile_ids(char* source, std::size_t source_length, std::ofstream& target);
        void decompile_alloc(std::istream& source, std::ofstream& target);
        void decompile_lvl_list(std::istream& source, std::ofstream& target);
        void decompile_data_directories(std::istream& source, std::ofstream& target);

        data_decoder m_decoder;
    };
}

#endif
