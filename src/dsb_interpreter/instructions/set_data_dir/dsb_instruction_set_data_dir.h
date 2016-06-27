#ifndef DSB_INSTRUCTION_SET_DATA_DIR_H
#define DSB_INSTRUCTION_SET_DATA_DIR_H

#include <dsb_interpreter/dsb_instruction.h>

namespace openrayman
{
    enum class dsb_data_directory
    {
        // Special value used to specify that no directory could be provided.
        none,

        // Specifies the directory where engine dlls are located.
        dll,

        // Specifies the root of game data.
        // This seems to be incorrect in Rayman 2?!?
        root,

        // Specifies the directory where data related to the game world is located.
        world,

        // Specifies the directory where levels are located.
        levels,

        // Specifies the directory where sound files are located.
        sound,

        // Specifies the directory where save files are located.
        saves,

        // Specifies the directory where "vignettes" are located.
        // Vignettes are 2D textures that are used in loading screens.
        vignette,

        // Specifies the directory where user options are saved.
        options
    };

    inline std::string data_directory_to_string(dsb_data_directory dir)
    {
    #define VALID_DIRECTORY_TO(name) \
        if(dir == dsb_data_directory::name) \
            return #name;

        VALID_DIRECTORY_TO(dll);
        VALID_DIRECTORY_TO(root);
        VALID_DIRECTORY_TO(world);
        VALID_DIRECTORY_TO(levels);
        VALID_DIRECTORY_TO(sound);
        VALID_DIRECTORY_TO(saves);
        VALID_DIRECTORY_TO(vignette);
        VALID_DIRECTORY_TO(options);

        return "none";
    }

    inline dsb_data_directory string_to_data_directory(const std::string& dir)
    {
    #define VALID_DIRECTORY_FROM(name) \
        if(dir == #name) \
            return dsb_data_directory::name;

        VALID_DIRECTORY_FROM(dll);
        VALID_DIRECTORY_FROM(root);
        VALID_DIRECTORY_FROM(world);
        VALID_DIRECTORY_FROM(levels);
        VALID_DIRECTORY_FROM(sound);
        VALID_DIRECTORY_FROM(saves);
        VALID_DIRECTORY_FROM(vignette);
        VALID_DIRECTORY_FROM(options);

        return dsb_data_directory::none;
    }

    struct dsb_instruction_set_data_dir : public dsb_instruction
    {
        dsb_instruction_set_data_dir(dsb_data_directory directory, const std::string& path) :
            dsb_instruction(dsb_instruction_type::set_data_dir),
            directory(directory), path(path)
        {
        }

        // The data directory to set.
        dsb_data_directory directory;

        // The path which it should be set to.
        std::string path;
    };
}

#endif
