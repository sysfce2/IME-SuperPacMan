////////////////////////////////////////////////////////////////////////////////
// Super Pac-Man clone
//
// Copyright (c) 2021 Kwena Mashamaite (kwena.mashamaite1@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "Scoreboard/Scoreboard.h"
#include "Scenes/StartUpScene.h"
#include "Common/Constants.h"

namespace spm {
    ///////////////////////////////////////////////////////////////
    Game::Game() :
        settingsFilename_("res/TextFiles/Settings.txt"),
        engine_{"Super Pac-Man", settingsFilename_}
    {}

    ///////////////////////////////////////////////////////////////
    void Game::initialize() {
        // Make game window unresizable
        engine_.getWindow().setStyle(ime::WindowStyle::Close);

        // Initialize the game engine
        engine_.initialize();

        // Initialize data that should be accessible in all states
        auto scoreboard = std::make_shared<Scoreboard>("res/TextFiles/Highscores.txt");
        scoreboard->load();

        engine_.getPersistentData().addProperty({"SETTINGS_FILENAME", settingsFilename_});
        engine_.getPersistentData().addProperty({"SCOREBOARD", scoreboard});
        engine_.getPersistentData().addProperty({"HIGH_SCORE", scoreboard->getTopScore().getValue()});
        engine_.getPersistentData().addProperty({"CURRENT_LEVEL", 1});
        engine_.getPersistentData().addProperty({"CURRENT_SCORE", 0});
        engine_.getPersistentData().addProperty({"PLAYER_LIVES", Constants::PacManLives});
        engine_.getPersistentData().addProperty({"NUM_EXTRA_LIVES_WON", 0});
        engine_.getPersistentData().addProperty({"MASTER_VOLUME", 100.0f});
        engine_.getPersistentData().addProperty({"PLAYER_WON_GAME", false});
        engine_.getPersistentData().addProperty({"GHOSTS_FRIGHTENED_MODE_DURATION", ime::seconds(Constants::POWER_MODE_DURATION)});
        engine_.getPersistentData().addProperty({"PACMAN_SUPER_MODE_DURATION", ime::seconds(Constants::SUPER_MODE_DURATION)});

        // If not found, player will be prompted for name in StartUpScene
        if (engine_.getConfigs().hasPref("PLAYER_NAME"))
            engine_.getPersistentData().addProperty({"PLAYER_NAME",engine_.getConfigs().getPref("PLAYER_NAME").getValue<std::string>()});

        engine_.pushScene(std::make_unique<StartUpScene>());
    }

    ///////////////////////////////////////////////////////////////
    void Game::start() {
        engine_.run();
    }

} // namespace spm
