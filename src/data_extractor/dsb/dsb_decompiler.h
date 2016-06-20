#ifndef DSB_DECOMPILER_H
#define DSB_DECOMPILER_H

#include <data_extractor/data_decoder.h>
#include <string>
#include <iostream>
#include <fstream>

namespace openrayman
{
    enum class dsb_format
    {
        // The default format.
        // This is a more human readable/editable format than the binary format used by Rayman 2.
        // This can be interpreted by the dsb engine.
        // Uses the extension .odsb
        openrayman,

        // The format used by the Rayman 2: The Great Escape engine, decoded.
        // This is a binary format.
        // This can not be interpreted by OpenRayman.
        // The only operation that is performed by the dsb decompiler when this format is specified
        // is to decode the dsb.
        // Uses the extension .rdsb
        rayman2_decoded
    };

    // Decodes and optionally decompiles DSB files.
    // This is all heavily adapted from Rayman2Lib's DSBDecompiler
    class dsb_decompiler
    {
public:
        // Decodes and optionally decompiles the specified DSB file into the target file.
        // This function assumes that the source file exists and that the target location is accessible.
        // Returns true if the function succeeded.
        bool decompile_dsb(const std::string& source, const std::string& target, dsb_format fmt);
private:
        void decompile_sections(char* source, std::size_t source_length, std::ofstream& target);
        void decompile_alloc(std::istream& source, std::ofstream& target);
        void decompile_lvl_list(std::istream& source, std::ofstream& target);
        void decompile_data_directories(std::istream& source, std::ofstream& target);
        void decompile_unknown_blob_0x20(std::istream& source, std::ofstream& target);
        void decompile_vignette(std::istream& source, std::ofstream& target);

        data_decoder m_decoder;
    };
}

#endif
