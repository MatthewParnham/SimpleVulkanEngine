#include "snake.hpp"
#include "Windows.h"
#include <stdlib.h>
#include <time.h>

//std
#include <iostream>

namespace lve {

    Snake::~Snake() {
        for(int i = 0; i < gridSize; i++) {
            delete grid[i];
        }
        delete grid;
    }

    void Snake::Start() {

		squareModel = GenPrimitives::createSquareModel(lveDevice, {0.0f, 0.0f}); 
        circleModel = GenPrimitives::createCircleModel(lveDevice, 12);




        grid = new glm::vec2*[gridSize];
		for (int i = 0; i < gridSize; i++) {
            grid[i] = new glm::vec2[gridSize];
			for (int j = 0; j < gridSize; j++) {
                grid[i][j] = {
					-1.0f + (i + 0.5f) * 2.0f / gridSize,
					-1.0f + (j + 0.5f) * 2.0f / gridSize};
			}
		}

        int half = (int)(gridSize / 2);
        glm::vec2 spawnPoint = grid[half][half];
        currPos = {half,half};
        auto snake = LveGameObject::createGameObject();
        snake.transform2d.scale = glm::vec2(0.035f);
        snake.transform2d.translation = spawnPoint;
        snake.color = {.1f,.8f,.1f};
        snake.model = squareModel;
        snakeBody.push_back(std::move(snake));
        spawnNewFood();
    }

    void Snake::Update() {
        if(isMovingHorizontal) {
            if(GetKeyState('W') & 0x8000/*Check if high-order bit is set (1 << 15)*/)
            {
                direction = {0,-1};
                isMovingHorizontal = false;
            }
            if(GetKeyState('S') & 0x8000/*Check if high-order bit is set (1 << 15)*/)
            {
                direction = {0,1};
                isMovingHorizontal = false;
            }
        } else {
            if(GetKeyState('A') & 0x8000/*Check if high-order bit is set (1 << 15)*/)
            {
                direction = {-1,0};
                isMovingHorizontal = true;
            }
            
            if(GetKeyState('D') & 0x8000/*Check if high-order bit is set (1 << 15)*/)
            {
                direction = {1,0};
                isMovingHorizontal = true;
            }
        }
        
        

        //check if snake eats
        if((int)currPos.x == (int)foodPos.x && (int)currPos.y == (int)foodPos.y) {
            food.pop_back();
            score++;
            if(score > 50) {
                std::cout << "Winner" << std::endl;
                reset();
                return;
            }
            spawnNewFood();

            //add new bit to end of snake

            auto snake = LveGameObject::createGameObject();
            snake.transform2d.scale = glm::vec2(0.035f);
            snake.transform2d.translation =  snakeBody[snakeBody.size()-1].transform2d.translation;
            snake.color = {.1f,.8f,.1f};
            snake.model = squareModel;
            snakeBody.push_back(std::move(snake));
        }


        //framerate dependent but whatever
        static int frame = 0;
        frame = (frame + 1) % (1000 - (score * 20));
        if(frame == 0) {
            currPos.x = (int)(currPos.x + direction.x);
            if(currPos.x < 0 || currPos.x > gridSize - 1) {
                std::cout << "loser" << std::endl;
                reset(); 
                return;
            }
            currPos.y = (int)(currPos.y + direction.y);
            if(currPos.y < 0 || currPos.y > gridSize - 1) {
                std::cout << "loser" << std::endl;
                reset(); 
                return;
            }
            //std:: cout << currPos.x << ',' << currPos.y << std:: endl;
            
            for(int i = snakeBody.size() - 1; i > 0; i--) {
                snakeBody[i].transform2d.translation = snakeBody[i-1].transform2d.translation;
            }
            snakeBody[0].transform2d.translation = grid[(int)currPos.x][(int)currPos.y];

            if(snakeBody.size() > 1) {
                for(int i = 1; i < snakeBody.size(); i++) {
                    if(
                        snakeBody[0].transform2d.translation.x == snakeBody[i].transform2d.translation.x &&
                        snakeBody[0].transform2d.translation.y == snakeBody[i].transform2d.translation.y
                    ) {
                        std::cout << "loser" << std::endl;
                        reset();
                        return;
                    }
                }
            }
        }
        
        
    }

    void Snake::spawnNewFood() {
        srand(time(NULL));
        int x = rand() % gridSize;
        int y = rand() % gridSize;
        foodPos = {x,y};
        auto f = LveGameObject::createGameObject();
        f.transform2d.scale = glm::vec2(0.015f);
        f.transform2d.translation = grid[x][y];
        f.color = {1.0f,1.0f,1.0f};
        f.model = circleModel;
        food.push_back(std::move(f));
    }

    void Snake::reset() {
        score = 0;
        food.clear();
        spawnNewFood();
        snakeBody.clear();
        int half = (int)(gridSize / 2);
        glm::vec2 spawnPoint = grid[half][half];
        currPos = {half,half};
        auto snake = LveGameObject::createGameObject();
        snake.transform2d.scale = glm::vec2(0.035f);
        snake.transform2d.translation = spawnPoint;
        snake.color = {.1f,.8f,.1f};
        snake.model = squareModel;
        snakeBody.push_back(std::move(snake));
        isMovingHorizontal = true;
        direction = {1,0};
    }
}

/*

auto vf = LveGameObject::createGameObject();
				vf.transform2d.scale = glm::vec2(0.035f);
				vf.transform2d.translation = grid[i][j];
				vf.color = {.1f,.8f,.1f};
				vf.model = squareModel;
				gameObjects.push_back(std::move(vf));

*/