#ifndef DSB_INSTRUCTION_H
#define DSB_INSTRUCTION_H

#include <cstdlib>
#include <string>

namespace openrayman
{
    enum class dsb_instruction_type
    {
        // Special value used to specify that no instruction could be provided.
        none,

        // This instruction implies that the DSB could not be interpreted for some reason.
        // It is paried with a dsb_instruction_invalid_dsb, which specifies a string that describes what went
        // wrong during interpretation, and at what line / column.
        // No instructions should follow or precede this one.
        invalid_dsb,

        // This instruction specifies that a comment was specified in the DSB.
        // It is paried with a dsb_instruction_comment, which specifies the comment encountered.
        comment,

        // This instruction implies the start of a new section in the DSB.
        // It is paired with a dsb_instruction_begin_section, which specifies what
        // type of section is starting.
        // An instruction of this type that starts a "none" section implies
        // the end of the instruction array.
        begin_section,

        // This instruction sets a data directory to a path.
        // It is paried with a dsb_instruction_set_data_dir, which specifies what data directory to set
        // and which path to set it to.
        set_data_dir,

        // This instruction sets a texture file id to an archive.
        // It is paried with a dsb_instruction_set_texture_file, which specifies what texture file id to set
        // and which archive to set it to.
        set_texture_file,

        // This instruction loads a background image into the vignette.
        // This can contain "random" expression, in the format "Random<img, ?, ?, ...>".
        // We don't currently know how to interpret this, so we just use the first image.
        // Additionally, names can contain .bmp. Ignore this.
        // TODO: is this ok?!? probably
        // It is paired with a dsb_instruction_vignette_load_img, which specifies image file to load.
        vignette_load_img,

        // This instruction sets one of several color values.
        // It is paried with a dsb_instruction_vignette_set_color, which specifies what id to set
        // and what color(s) to set.
        vignette_set_color,

        // This instruction creates a bar with the current specifed colors.
        // It is paried with a dsb_instruction_vignette_create_bar, which specifies the position and size of
        // the vignette.
        vignette_create_bar,

        // This instruction adds a bar to the vignette.
        // It is not paired with a class, as it doesn't need any additional values.
        vignette_add_bar,

        // This instruction displays the vignette on the screen.
        // It is not paired with a class, as it doesn't need any additional values.
        vignette_display,

        // This instruction adds a level.
        // It is paired with a dsb_instruction_level_add, which specifies what level to add.
        level_add
    };

    inline std::string instruction_to_string(dsb_instruction_type instruction)
    {
    #define VALID_INSTRUCTION_TO(name) \
        if(instruction == dsb_instruction_type::name) \
            return #name;

        VALID_INSTRUCTION_TO(invalid_dsb);
        VALID_INSTRUCTION_TO(comment);
        VALID_INSTRUCTION_TO(begin_section);
        VALID_INSTRUCTION_TO(set_data_dir);
        VALID_INSTRUCTION_TO(set_texture_file);
        VALID_INSTRUCTION_TO(vignette_load_img);
        VALID_INSTRUCTION_TO(vignette_set_color);
        VALID_INSTRUCTION_TO(vignette_create_bar);
        VALID_INSTRUCTION_TO(vignette_add_bar);
        VALID_INSTRUCTION_TO(vignette_display);
        VALID_INSTRUCTION_TO(level_add);

        return "none";
    }

    inline dsb_instruction_type string_to_instruction(const std::string& instruction)
    {
    #define VALID_INSTRUCTION_FROM(name) \
        if(instruction == #name) \
            return dsb_instruction_type::name;

        VALID_INSTRUCTION_FROM(invalid_dsb);
        VALID_INSTRUCTION_FROM(comment);
        VALID_INSTRUCTION_FROM(begin_section);
        VALID_INSTRUCTION_FROM(set_data_dir);
        VALID_INSTRUCTION_FROM(set_texture_file);
        VALID_INSTRUCTION_FROM(vignette_load_img);
        VALID_INSTRUCTION_FROM(vignette_set_color);
        VALID_INSTRUCTION_FROM(vignette_create_bar);
        VALID_INSTRUCTION_FROM(vignette_add_bar);
        VALID_INSTRUCTION_FROM(vignette_display);
        VALID_INSTRUCTION_FROM(level_add);

        return dsb_instruction_type::none;
    }

    enum class dsb_section_type
    {
        // Special value used to specify that no section could be provided.
        none,

        // Section to allocate values into slots.
        // This section is not present in a raw Rayman 2: The Great Escape .pgb.
        // Unused.
        alloc,

        // Section to set data directories.
        // This is only used by the data extractor to find game data in a
        // valid Rayman 2: The Great Escape installation.
        data_directories,

        // Section to set texture archives.
        // This is only used by the data extractor to find game data in a
        // valid Rayman 2: The Great Escape installation.
        texture_files,

        // Section that builds a vignette.
        vignette,

        // Unknown section.
        // Unused.
        unknown_blob_0x6e,

        // Unknown section.
        // Unused.
        unknown_blob_0x20,

        // Section that specifies what game option files to use.
        // Unused.
        game_options,

        // Sections that adds levels.
        // This is only used by the data extractor to find game data in a
        // valid Rayman 2: The Great Escape installation.
        levels,

        // Unknown section (for now).
        // Unused.
        sound_banks,

        // Unknown section (for now).
        // Unused.
        load_sound_banks
    };

    inline std::string section_to_string(dsb_section_type section)
    {
    #define VALID_SECTION_TO(name) \
        if(section == dsb_section_type::name) \
            return #name;

        VALID_SECTION_TO(alloc);
        VALID_SECTION_TO(data_directories);
        VALID_SECTION_TO(texture_files);
        VALID_SECTION_TO(vignette);
        VALID_SECTION_TO(unknown_blob_0x6e);
        VALID_SECTION_TO(unknown_blob_0x20);
        VALID_SECTION_TO(game_options);
        VALID_SECTION_TO(levels);
        VALID_SECTION_TO(sound_banks);
        VALID_SECTION_TO(load_sound_banks);

        return "none";
    }

    inline dsb_section_type string_to_section(const std::string& section)
    {
    #define VALID_SECTION_FROM(name) \
        if(section == #name) \
            return dsb_section_type::name;

        VALID_SECTION_FROM(alloc);
        VALID_SECTION_FROM(data_directories);
        VALID_SECTION_FROM(texture_files);
        VALID_SECTION_FROM(vignette);
        VALID_SECTION_FROM(unknown_blob_0x6e);
        VALID_SECTION_FROM(unknown_blob_0x20);
        VALID_SECTION_FROM(game_options);
        VALID_SECTION_FROM(levels);
        VALID_SECTION_FROM(sound_banks);
        VALID_SECTION_FROM(load_sound_banks);

        return dsb_section_type::none;
    }

    struct dsb_instruction
    {
        dsb_instruction(dsb_instruction_type type) :
            type(type)
        {

        }

        // The type of instruction that this class holds.
        // The class should be cast via static_cast<dsb_instruction_?> or similar.
        dsb_instruction_type type;
    };
}

#endif
