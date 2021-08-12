////////////////////////////////////////////////////////////////////////////////
// Super Pac-Man clone
//
// Copyright (c) 2020-2021 Kwena Mashamaite (kwena.mashamaite1@gmail.com)
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

#ifndef SUPERPACMAN_GRIDANIMATION_H
#define SUPERPACMAN_GRIDANIMATION_H

#include <IME/core/animation/Animation.h>
#include <vector>

namespace spm {
    /**
     * @brief Construct flashing grid animations
     *
     * These animations are played every time a level or a bonus
     * stage is completed
     */
    class GridAnimation {
    public:
        /**
         * @brief Constructor
         */
        GridAnimation();

        /**
         * @brief Get all the grid animations
         * @return All grid animations
         */
        const std::vector<ime::Animation::Ptr>& getAll();

    private:
        /**
         * @brief Create a flashing animation
         * @param gridColour Colour of the grid in lower case
         * @param gridFrame Index of the grid frame on the spritesheet
         */
        void createFlashAnimation(const std::string& gridColour, ime::Index gridFrame);

    private:
        std::vector<ime::Animation::Ptr> animations_;
        ime::SpriteSheet spritesheet_;
    };
}

#endif
