#ifndef GAME_H
#define GAME_H

#include <string>
#include <platform/backend_specifics.h>
#include <vector>

namespace openrayman
{
    // Info about a game that is specified in the manifest.
    struct game_info
    {
        std::string name, description;
    };

    // A collection of files that combined form a "game".
    // Can depend on other collections of files ("games").
    class game
    {
public:
        game(const backend_specifics& backend_specifics, const std::string& name);

        // Returns true if the game and all of its dependencies are valid (found on disk and contains valid manifest).
        inline bool valid() const
        {
            return m_valid;
        }

        // Returns the location of this game on disk.
        inline const std::string& get_location() const
        {
            return m_location;
        }

        // Returns the info that was specified in the manifest about this game.
        inline const game_info& get_info() const
        {
            return m_info;
        }

        // Returns a list of all games that this game depend on.
        inline const std::vector<game>& get_dependencies() const
        {
            return m_dependencies;
        }

        // If only_in_this is set to false, then return true if the specified file exists in this game or in any dependencies.
        // If only_in_this is set to true, then return true if the specified file exists in this game, and this game only.
        bool has_file(const std::string& file, bool only_in_this = false) const;

        // Returns the location on disk of the specified file.
        // This searches through the game and all of its dependencies, in that order.
        // Dependencies are searched from first to last specified in the manifest.
        // An empty string is returned if the file was not found.
        // File existence can be checked via the function has_file.
        const std::string find_file(const std::string& file) const;
private:
        bool m_valid;
        std::string m_location;
        game_info m_info;
        std::vector<game> m_dependencies;
    };
}

#endif
