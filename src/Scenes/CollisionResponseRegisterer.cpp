////////////////////////////////////////////////////////////////////////////////
// Pac-Man clone
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

#include "CollisionResponseRegisterer.h"
#include "Scenes/GameplayScene.h"
#include "GameObjects/GameObjects.h"
#include "Common/Constants.h"
#include "LevelStartScene.h"
#include "Utils/Utils.h"
#include "AI/ghost/EatenState.h"
#include <IME/core/engine/Engine.h>
#include <cassert>


namespace spm {
    ///////////////////////////////////////////////////////////////
    CollisionResponseRegisterer::CollisionResponseRegisterer(GameplayScene &game) : game_{game}
    {}

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithFruit(ime::GameObject *gameObject) {
        gameObject->onCollision(std::bind(&CollisionResponseRegisterer::resolveFruitCollision, this, std::placeholders::_2));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithKey(ime::GameObject *gameObject) {
        gameObject->onCollision(std::bind(&CollisionResponseRegisterer::resolveKeyCollision, this, std::placeholders::_2));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithDoor(ime::GameObject *gameObject) {
        gameObject->onCollision(std::bind(&CollisionResponseRegisterer::resolveDoorCollision, this, std::placeholders::_2, std::placeholders::_1));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithPowerPellet(ime::GameObject *gameObject) {
        gameObject->onCollision(std::bind(&CollisionResponseRegisterer::resolvePowerPelletCollision, this, std::placeholders::_2));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithSuperPellet(ime::GameObject *gameObject) {
        gameObject->onCollision(std::bind(&CollisionResponseRegisterer::resolveSuperPelletCollision, this, std::placeholders::_2));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithPacMan(ime::GameObject *gameObject) {
        gameObject->onCollision(std::bind(&CollisionResponseRegisterer::resolvePacmanCollision, this, std::placeholders::_2, std::placeholders::_1));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithGhost(ime::GameObject *gameObject) {
        gameObject->onCollision(std::bind(&CollisionResponseRegisterer::resolveGhostCollision, this, std::placeholders::_2, std::placeholders::_1));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithStar(ime::GameObject *gameObject) {
        gameObject->onCollision(std::bind(&CollisionResponseRegisterer::resolveStarCollision, this, std::placeholders::_2, std::placeholders::_1));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithTeleportationSensor(ime::GameObject *gameObject) {
        gameObject->onCollision(std::bind(&CollisionResponseRegisterer::resolveTeleportationSensorCollision, this, std::placeholders::_2, std::placeholders::_1));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::registerCollisionWithSlowDownSensor(ime::GameObject *gameObject) {
        gameObject->onCollision(std::bind(&CollisionResponseRegisterer::resolveSlowDownSensorCollision, this, std::placeholders::_2, std::placeholders::_1));
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveFruitCollision(ime::GameObject* fruit) {
        if (fruit->getClassName() != "Fruit")
            return;

        fruit->setActive(false);
        game_.updateScore(Constants::Points::FRUIT * game_.currentLevel_);
        game_.numFruitsEaten_++;
        game_.audio().play(ime::audio::Type::Sfx, "WakkaWakka.wav");
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveKeyCollision(ime::GameObject *key) {
        if (key->getClassName() == "Key") {
            // Attempt to unlock a door with the collected key
            game_.gameObjects().forEachInGroup("Door",[key](ime::GameObject* gameObject) {
                auto* door = static_cast<Door*>(gameObject);
                door->unlock(*static_cast<Key*>(key));

                if (!door->isLocked())
                    door->setActive(false);
            });

            key->setActive(false);
            game_.updateScore(Constants::Points::KEY);
            game_.audio().play(ime::audio::Type::Sfx, "keyEaten.wav");
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolvePowerPelletCollision(ime::GameObject *pellet) {
        if (pellet->getClassName() == "Pellet" && pellet->getTag() == "power") {
            pellet->setActive(false);

            game_.pauseGhostAITimer();
            game_.updateScore(Constants::Points::POWER_PELLET);

            if (!game_.isBonusStage_) {
                game_.configureTimer(game_.powerModeTimer_, game_.getFrightenedModeDuration(), [this] {
                    game_.pointsMultiplier_ = 1;

                    if (!game_.superModeTimer_.isRunning())
                        game_.resumeGhostAITimer();

                    game_.emit(GameEvent::FrightenedModeEnd);
                });
            }

            // Extend super mode duration by power mode duration
            if (game_.superModeTimer_.isRunning())
                game_.superModeTimer_.setInterval(game_.superModeTimer_.getRemainingDuration() + game_.getFrightenedModeDuration());

            game_.numPelletsEaten_++;
            game_.audio().play(ime::audio::Type::Sfx, "powerPelletEaten.wav");
            game_.emit(GameEvent::FrightenedModeBegin);
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveSuperPelletCollision(ime::GameObject *pellet) {
        if (pellet->getClassName() == "Pellet" && pellet->getTag() == "super") {
            pellet->setActive(false);

            game_.pauseGhostAITimer();
            game_.updateScore(Constants::Points::SUPER_PELLET);

            if (!game_.isBonusStage_) {
                game_.configureTimer(game_.superModeTimer_, game_.getSuperModeDuration(), [this] {
                    game_.emit(GameEvent::SuperModeEnd);
                    game_.resumeGhostAITimer();
                });
            }

            game_.numPelletsEaten_++;
            game_.audio().play(ime::audio::Type::Sfx, "superPelletEaten.wav");
            game_.emit(GameEvent::SuperModeBegin);
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolvePacmanCollision(ime::GameObject* pacman, ime::GameObject* otherGameObject) {
        if (pacman->getClassName() == "PacMan" && static_cast<PacMan*>(pacman)->getState() != PacMan::State::Super) {
            auto* ghost = dynamic_cast<Ghost*>(otherGameObject);
            if (ghost && (ghost->getState() == Ghost::State::Frightened || ghost->getState() == Ghost::State::Eaten))
                return;

            game_.despawnStar();
            game_.audio().stopAll();
            game_.stopAllTimers();

            auto pac = static_cast<PacMan*>(pacman);
            pac->setState(PacMan::State::Dying);
            pac->getSprite().getAnimator().startAnimation("dying");
            pac->setLivesCount(pac->getLivesCount() - 1);
            game_.cache().setValue("PLAYER_LIVES", pac->getLivesCount());
            game_.view_.removeLife();

            game_.gameObjects().forEachInGroup("Ghost", [](ime::GameObject* ghost) {
                ghost->getSprite().setVisible(false);
                ghost->getGridMover()->setMovementFreeze(true);
            });

            auto deathAnimDuration = pacman->getSprite().getAnimator().getAnimation("dying")->getDuration();
            game_.timer().setTimeout(deathAnimDuration + ime::milliseconds(400), [this, pacman] {
                if (static_cast<PacMan*>(pacman)->getLivesCount() <= 0) {
                    game_.gameObjects().remove(pacman);
                    game_.endGameplay();
                } else
                    game_.engine().pushScene(std::make_unique<LevelStartScene>());
            });

            game_.audio().play(ime::audio::Type::Sfx, "pacmanDying.wav");
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveGhostCollision(ime::GameObject *ghost, ime::GameObject *otherGameObject) {
        if (ghost->getClassName() == "Ghost" && static_cast<Ghost*>(ghost)->getState() == Ghost::State::Frightened) {
            game_.powerModeTimer_.pause();

            if (game_.superModeTimer_.isRunning())
                game_.superModeTimer_.pause();

            setMovementFreeze(true);
            game_.updateScore(Constants::Points::GHOST * game_.pointsMultiplier_);
            replaceWithScoreTexture(ghost, otherGameObject);
            game_.updatePointsMultiplier();

            game_.timer().setTimeout(ime::seconds(1), [=] {
                setMovementFreeze(false);
                otherGameObject->getSprite().setVisible(true);
                game_.powerModeTimer_.start();

                if (game_.superModeTimer_.isPaused())
                    game_.superModeTimer_.start();

                static_cast<Ghost*>(ghost)->setState(std::make_unique<EatenState>(Ghost::State::Scatter));
            });

            game_.audio().play(ime::audio::Type::Sfx, "ghostEaten.wav");
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveStarCollision(ime::GameObject *star, ime::GameObject *otherGameObject) {
        if (star->getClassName() == "Star") {
            game_.starTimer_.stop();

            if (game_.ghostAITimer_.isRunning())
                game_.ghostAITimer_.pause();

            if (game_.powerModeTimer_.isRunning())
                game_.powerModeTimer_.pause();

            if (game_.superModeTimer_.isRunning())
                game_.superModeTimer_.pause();

            if (game_.bonusStageTimer_.isRunning())
                game_.bonusStageTimer_.pause();

            setMovementFreeze(true);
            star->getSprite().getAnimator().stop();

            ime::Time freezeDuration = ime::seconds(1);
            ime::AnimationFrame* leftFruitFrame = game_.gameObjects().findByTag("leftBonusFruit")->getSprite().getAnimator().getCurrentFrame();
            ime::AnimationFrame* rightFruitFrame = game_.gameObjects().findByTag("rightBonusFruit")->getSprite().getAnimator().getCurrentFrame();
            if (leftFruitFrame->getIndex() == rightFruitFrame->getIndex()) {
                otherGameObject->getSprite().setVisible(false);
                star->getSprite().setTexture("spritesheet.png");

                if (leftFruitFrame->getName() == utils::getFruitName(game_.currentLevel_))
                {
                    game_.updateScore(Constants::Points::MATCHING_BONUS_FRUIT_AND_LEVEL_FRUIT);
                    star->getSprite().setTextureRect({408, 142, 32, 16}); // 5000
                } else
                {
                    game_.updateScore(Constants::Points::MATCHING_BONUS_FRUIT);
                    star->getSprite().setTextureRect({441, 142, 32, 16}); // 2000
                }

                star->resetSpriteOrigin();
                freezeDuration = ime::seconds(3);
            } else {
                game_.updateScore(Constants::Points::GHOST * game_.pointsMultiplier_);
                replaceWithScoreTexture(star, otherGameObject);
            }

            game_.gameObjects().findByTag("leftBonusFruit")->getSprite().getAnimator().stop();
            game_.gameObjects().findByTag("rightBonusFruit")->getSprite().getAnimator().stop();

            game_.timer().setTimeout(freezeDuration, [this, otherGameObject] {
                setMovementFreeze(false);
                otherGameObject->getSprite().setVisible(true);
                game_.despawnStar();

                if (game_.ghostAITimer_.isPaused())
                    game_.ghostAITimer_.start();

                if (game_.powerModeTimer_.isPaused())
                    game_.powerModeTimer_.start();

                if (game_.superModeTimer_.isPaused())
                    game_.superModeTimer_.start();

                if (game_.bonusStageTimer_.isPaused())
                    game_.bonusStageTimer_.start();
            });
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveDoorCollision(ime::GameObject *door, ime::GameObject *otherGameObject) {
        if (door->getClassName() == "Door") {
            auto* pacman = dynamic_cast<PacMan*>(otherGameObject);
            if (pacman && pacman->getState() == PacMan::State::Super) {
                static_cast<Door *>(door)->burst();
                pacman->getGridMover()->requestDirectionChange(pacman->getDirection());
                game_.updateScore(Constants::Points::BROKEN_DOOR);
                game_.audio().play(ime::audio::Type::Sfx, "doorBroken.wav");
            }
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveSlowDownSensorCollision(ime::GameObject *sensor, ime::GameObject *objectOnSensor) {
        if (sensor->getClassName() == "Sensor" && sensor->getTag().find("slowDownSensor") != std::string::npos) {
            char sensorNum = sensor->getTag().back();

            float speed;
            if (game_.currentLevel_ == 1)
                speed = 0.40 * Constants::Constants::PacManNormalSpeed;
            else if (game_.currentLevel_ >= 2 && game_.currentLevel_ <= 4)
                speed = 0.45f * Constants::Constants::PacManNormalSpeed;
            else
                speed = 0.50f * Constants::Constants::PacManNormalSpeed;

            ime::Direction dir = objectOnSensor->getGridMover()->getDirection();
            if (((sensorNum == '2' || sensorNum == '4') && dir == ime::Right) ||
                ((sensorNum == '1' || sensorNum == '3') && dir == ime::Left) ||
                (sensorNum == '5' && dir == ime::Up))
            {
                objectOnSensor->getGridMover()->setMaxLinearSpeed(ime::Vector2f{speed, speed});
            }
            else
                objectOnSensor->getGridMover()->setMaxLinearSpeed(ime::Vector2f{Constants::Constants::PacManNormalSpeed, Constants::Constants::PacManNormalSpeed});
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::resolveTeleportationSensorCollision(ime::GameObject* sensor, ime::GameObject* objectOnSensor) {
        if (sensor->getClassName() == "Sensor" && sensor->getTag() == "teleportationSensor") {
            ime::GridMover* gridMover = objectOnSensor->getGridMover();
            ime::TileMap& grid = gridMover->getGrid();
            const ime::Tile& currentTile = grid.getTileOccupiedByChild(objectOnSensor);
            grid.removeChild(objectOnSensor);

            if (currentTile.getIndex().colm == 0) { // Triggered the left-hand side sensor
                grid.addChild(objectOnSensor,ime::Index{currentTile.getIndex().row, static_cast<int>(grid.getSizeInTiles().x - 1)});
            } else
                grid.addChild(objectOnSensor, {currentTile.getIndex().row, 0});

            gridMover->resetTargetTile();
            gridMover->requestDirectionChange(gridMover->getDirection());
        }
    }

    ///////////////////////////////////////////////////////////////
    void CollisionResponseRegisterer::replaceWithScoreTexture(ime::GameObject* ghost, ime::GameObject* otherGameObject) const {
        otherGameObject->getSprite().setVisible(false);
        static const ime::SpriteSheet numbers{"spritesheet.png", ime::Vector2u{16, 16}, ime::Vector2u{1, 1}, ime::UIntRect{306, 141, 69, 18}};
        ghost->getSprite().setTexture(numbers.getTexture());

        if (game_.pointsMultiplier_ == 1)
            ghost->getSprite().setTextureRect(*numbers.getFrame(ime::Index{0, 0})); // 100
        else if (game_.pointsMultiplier_ == 2)
            ghost->getSprite().setTextureRect(*numbers.getFrame(ime::Index{0, 1})); // 200
        else if (game_.pointsMultiplier_ == 4)
            ghost->getSprite().setTextureRect(*numbers.getFrame(ime::Index{0, 2})); // 800
        else
            ghost->getSprite().setTextureRect(*numbers.getFrame(ime::Index{0, 3})); // 1600
    }

    void CollisionResponseRegisterer::setMovementFreeze(bool freeze) {
        game_.grid_->forEachGameObject([freeze](ime::GameObject* gameObject) {
            if (gameObject->getGridMover()) { // Movable object
                gameObject->getSprite().getAnimator().setTimescale(freeze ? 0.0f : 1.0f);
                gameObject->getGridMover()->setMovementFreeze(freeze);
            }
        });
    }
}