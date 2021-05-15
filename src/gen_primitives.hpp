#pragma once

#include "lve_model.hpp"
#include "lve_game_object.hpp"
#include "lve_device.hpp"

// std
#include <memory>

namespace lve {

    class GenPrimitives {
    public:
        static std::unique_ptr<LveModel> createCircleModel(LveDevice& device, unsigned int numSides);
        static std::unique_ptr<LveModel> createSquareModel(LveDevice& device, glm::vec2 offset);
    private:

    };


}
