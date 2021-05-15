
#include "lve_model.hpp"
#include "lve_game_object.hpp"
#include "lve_device.hpp"
#include "gen_primitives.hpp"

#include <queue>

namespace lve {

    class Snake {
        public:

            Snake(LveDevice& device) : lveDevice{device} {}
            ~Snake();

            void Start();
            void Update();
            glm::vec2 currPos;
            glm::vec2 foodPos;
            int score{0};
            std::vector<LveGameObject> gameObjects{};
            std::vector<LveGameObject> food{};
            std::vector<LveGameObject> snakeBody{};
        private:
            LveDevice& lveDevice;

            int gridSize{40};
            glm::vec2 **grid;
            std::shared_ptr<LveModel> squareModel;
            std::shared_ptr<LveModel> circleModel;
            glm::vec2 direction = {1,0};

            std::queue<glm::vec2> positionQueue;
            bool isMovingHorizontal{true};
            void reset();
            void spawnNewFood();
    };

}