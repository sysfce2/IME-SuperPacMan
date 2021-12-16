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

#include "Utils.h"
#include <cassert>

namespace spm::utils {
    ///////////////////////////////////////////////////////////////
    std::string convertToString(ime::Direction direction) {
        if (direction == ime::Left)
            return "Left";
        else if (direction == ime::Right)
            return "Right";
        else if (direction == ime::Up)
            return "Up";
        else if (direction == ime::Down)
            return "Down";
        else
            return "Unknown";
    }

    ///////////////////////////////////////////////////////////////
    std::string getFruitName(int level) {
        auto static fruitName = std::vector{
            "apple", "banana", "donut", "hamburger", "egg", "corn", "shoe", "cake", "peach",
            "melon", "coffee", "mushroom", "bell", "clover", "galaxian", "gift"
        };

        if (level <= fruitName.size())
            return fruitName.at(level - 1);

        assert(false && (std::string("Level \"") + std::to_string(level) + "\" is unsupported").c_str());
        return ""; // Silence warning: Non-void function does not return a value in all control paths
    }

} // namespace spm
