#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <utility>
#include "cpoint.h"
#include "cmap.h"
#include "cenemy.h"
#include "ctower.h"
#include "cbullet.h"

using namespace std;
using namespace sf;

// Test
vector<Vector2f> convertPath(cpoint* path, int maxStep) {
    vector<Vector2f> result;
    for (int i = 0; i < maxStep; i++)
        result.push_back(Vector2f(path[i].getPixelX(), path[i].getPixelY()));
    return result;
}

struct Bullet
{
    CircleShape shape;
    int targetEnemyIdx; // Index of the enemy this bullet is tracking
    bool active = true;
};
vector<Bullet> bullets;

struct Enemy
{
    Sprite sprite;
    int currentTarget = 1;
    bool reachedEnd = false;
    int hitCount = 0;
};

struct Explosion {
    Sprite sprite;
    float timer = 0.f;
};
vector<Explosion> explosions;

int main() {
    // Load textures
    Texture backgroundTexture, towerTexture, enemyTexture, maintowerTexture, explosionTexture;
    if (!backgroundTexture.loadFromFile("background.png") ||
        !towerTexture.loadFromFile("tower.png") ||
        !enemyTexture.loadFromFile("blockbott4.png") ||
        !maintowerTexture.loadFromFile("maintower.png")
        || !explosionTexture.loadFromFile("explosion.png")) {
        cerr << "Failed to load textures!\n";
        return -1;
    }

    // Create window
    Vector2u textureSize = backgroundTexture.getSize();
    RenderWindow window(VideoMode(textureSize.x, textureSize.y), "Tower Defense SFML");
    Sprite backgroundSprite(backgroundTexture);

    // Map & Path
    cmap map;
    cenemy& ce = map.getEnemy();
    cpoint* pathData = ce.getP();
    int pathLen = ce.getPathLength();
    vector<Vector2f> path = convertPath(pathData, pathLen);

    // Enemy setup
    vector<Enemy> enemies;
    for (int i = 0; i < 10; i++) {
        Sprite sprite;
        sprite.setTexture(enemyTexture);
        sprite.setOrigin(enemyTexture.getSize().x / 2.f, enemyTexture.getSize().y / 2.f);
        sprite.setScale(0.5f, 0.5f);
        sprite.setPosition(path[0].x - i * 80.f, path[0].y);
        enemies.push_back({ sprite, 1, false, 0 });
    }

    // Main Tower setup
    Sprite mainTowerSprite;
    mainTowerSprite.setTexture(maintowerTexture);
    mainTowerSprite.setOrigin(maintowerTexture.getSize().x / 2.f, maintowerTexture.getSize().y / 2.f);
    mainTowerSprite.setScale(0.3f, 0.3f);
    mainTowerSprite.setPosition(path.back());

    int enemyReachCount = 0;
    const int MAX_REACH = 3;
    bool gameOver = false;

    // Tower and Bullet
    float speed = 120.f;
    vector<Sprite> towers;
    cbullet bulletLogic;
    Clock clock;
    vector<Bullet> bullets;


    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (!gameOver && event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;
                cpoint clicked = cpoint::fromXYToRowCol(mouseX, mouseY);
                int r = clicked.getRow(), c = clicked.getCol();
                if (r >= 0 && r < cpoint::MAP_ROW && c >= 0 && c < cpoint::MAP_COL && towers.size() < 3) {
                    if (map.getMap()[r][c].getC() == -1) {
                        Sprite towerSprite;
                        towerSprite.setTexture(towerTexture);
                        towerSprite.setOrigin(towerTexture.getSize().x / 2.f, towerTexture.getSize().y / 2.f);
                        towerSprite.setPosition(map.getMap()[r][c].getPixelX(), map.getMap()[r][c].getPixelY());
                        towers.push_back(towerSprite);
                    }
                }
            }
        }

        if (!gameOver) {
            // Enemy move
            for (auto& e : enemies) {
                if (!e.reachedEnd && e.currentTarget < (int)path.size()) {
                    Vector2f pos = e.sprite.getPosition();
                    Vector2f target = path[e.currentTarget];
                    Vector2f dir = target - pos;
                    float len = sqrt(dir.x * dir.x + dir.y * dir.y);
                    if (len < 1.f) {
                        e.currentTarget++;
                        if (e.currentTarget >= (int)path.size()) {
                            e.reachedEnd = true;
                            // Check if collide with main tower
                            if (e.sprite.getGlobalBounds().intersects(mainTowerSprite.getGlobalBounds())) {
                                enemyReachCount++;
                                cout << "Enemy reached tower: " << enemyReachCount << "/" << MAX_REACH << "\n";
                                if (enemyReachCount >= MAX_REACH)
                                    gameOver = true;
                            }
                        }
                    }
                    else {
                        dir /= len;
                        e.sprite.move(dir * speed * deltaTime);
                    }
                }
            }

            // Shooting
            static float shootTimer = 0.f;
            shootTimer += deltaTime;

            if (!towers.empty() && shootTimer > 1.f && !enemies.empty()) {
                shootTimer = 0.f;

                for (auto& tw : towers) {
                    Vector2f towerPos = tw.getPosition();
                    cpoint towerPoint = cpoint::fromXYToRowCol(towerPos.x, towerPos.y);
                    int nPath = bulletLogic.calcPathBullet(towerPoint);

                    if (nPath > 0) {
                        int targetIdx = -1;
                        float minDist = 150.f;

                        // Chọn enemy gần nhất trong phạm vi
                        for (size_t i = 0; i < enemies.size(); ++i) {
                            if (!enemies[i].reachedEnd) {
                                Vector2f enemyPos = enemies[i].sprite.getPosition();
                                float dist = sqrt(pow(enemyPos.x - towerPos.x, 2) + pow(enemyPos.y - towerPos.y, 2));
                                if (dist <= minDist) {
                                    minDist = dist;
                                    targetIdx = static_cast<int>(i);
                                }
                            }
                        }

                        if (targetIdx != -1) {
                            Bullet b;
                            b.shape = CircleShape(5.f);
                            b.shape.setFillColor(Color::Red);
                            b.shape.setOrigin(5.f, 5.f);
                            b.shape.setPosition(bulletLogic.getP()[0].getPixelX(), bulletLogic.getP()[0].getPixelY());
                            b.targetEnemyIdx = targetIdx;
                            b.active = true;
                            bullets.push_back(b);
                        }
                    }
                }
            }



            // Bullet move
            for (auto& b : bullets) {
                if (!b.active) continue;
                if (b.targetEnemyIdx < 0 || b.targetEnemyIdx >= (int)enemies.size()) {
                    b.active = false;
                    continue;
                }
                Enemy& target = enemies[b.targetEnemyIdx];
                if (target.reachedEnd) {
                    b.active = false;
                    continue;
                }
                Vector2f bp = b.shape.getPosition();
                Vector2f ep = target.sprite.getPosition();
                Vector2f dir = ep - bp;
                float len = sqrt(dir.x * dir.x + dir.y * dir.y);
                if (len < 10.f) {
                    b.active = false;
                    target.hitCount++;

                    // Add collision
                    Sprite exp;
                    exp.setTexture(explosionTexture);
                    exp.setOrigin(explosionTexture.getSize().x / 2.f, explosionTexture.getSize().y / 2.f);
                    exp.setPosition(ep);
                    exp.setScale(0.05f, 0.05f); 
                    explosions.push_back({ exp, 0.f });
                    if (target.hitCount >= 3)
                        enemies.erase(enemies.begin() + b.targetEnemyIdx);
                    continue;
                }
                if (len > 0.1f) {
                    dir /= len;
                    float moveSpeed = bulletLogic.getSpeed() * deltaTime * 60.f;
                    b.shape.move(dir * moveSpeed);
                }
            }

            // Remove inactive bullets
            bullets.erase(remove_if(bullets.begin(), bullets.end(),
                [](const Bullet& b) { return !b.active; }), bullets.end());
        }

        // Draw
        window.clear();
        window.draw(backgroundSprite);
        window.draw(mainTowerSprite);
        for (const auto& t : towers) window.draw(t);
        for (const auto& e : enemies) window.draw(e.sprite);
        for (const auto& b : bullets) window.draw(b.shape);
        for (auto& e : explosions)
            window.draw(e.sprite);

        // Cập nhật thời gian và loại bỏ explosion sau 0.3s
        for (auto& e : explosions)
            e.timer += deltaTime;
        explosions.erase(
            remove_if(explosions.begin(), explosions.end(), [](const Explosion& e) {
                return e.timer > 0.3f;
                }),
            explosions.end()
        );
        window.display();
    }

    return 0;
}





