#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <platform/backend_specifics.h>

namespace openrayman
{
    // Config file using JSON internally.
    // This will not be used for save files.
    class config
    {
public:
        config(const backend_specifics& backend_specifics);
        ~config();

        // Reloads the config file from the disk.
        // Returns true if the config was reloaded successfully.
        // This discards all changes done since last reload.
        // This is done automatically in the constructor.
        bool reload();

        // Saves the config file and all changes to the disk.
        // Returns true if the config was saved successfully.
        // This is done automatically in the destructor.
        bool save() const;

        // If the game should be synchronized to the vertical blank of the screen.
        // This might introduce additional input lag (though we do use glFinish to minimize it).
    	bool vsync;

        // If the game should run in fullscreen mode.
        bool fullscreen;

        // The maximum fps that the game is allowed to run at.
        // If this is set to 0 or less then no limit will be applied.
        double max_fps;

        // The user selected game to load.
        // Defaults to the game "rayman2_openrayman".
        std::string game;
private:
        const backend_specifics& m_backend_specifics;
    };
}

#endif
