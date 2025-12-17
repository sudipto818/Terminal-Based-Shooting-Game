#include <iostream>
#include <vector>
#include <chrono> //Helps in controlling time utilities
#include <thread> //To simulate FPS
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <functional>
#include "Entity.h"
#include "Terminal.h" 

using namespace std;

int main() {
    srand(time(0));
    initTermios(); //Set raw input mode in the terminal

    //Dimensions of the window terminal
    const int widthOfScreen = 50, heightOfScreen = 20;

    //Initial values and position of the launcher
    string launcherSprite = " !^! ";
    int launcherPos = widthOfScreen / 2 - launcherSprite.size();
    int score = 0, health = 3, explosivesAmmo = 0;
    int level = 1;
    bool paused = false;
    int bulletAmmo = -1;
    
    //Vector of entities
    vector<Entity> bullets, planes, missiles, coins, potions, explosives;

    //Boss entity
    Entity boss;
    bool bossActive = false;
    int bossDir = 1; // 1 = moving right, -1 = moving left
    const int BOSS_MAX_HP = 20;

    //Tracking of the times of entities
    auto lastPlane = chrono::steady_clock::now();
    auto lastPotion = chrono::steady_clock::now();
    auto lastCoin = chrono::steady_clock::now(); 
    auto lastExplosiveDrop = chrono::steady_clock::now();
    auto lastPlaneMove = chrono::steady_clock::now();
    auto lastMissileMove = chrono::steady_clock::now();
    auto lastItemMove = chrono::steady_clock::now();

    bool running = true;
    while (running) {
        auto now = chrono::steady_clock::now();

        // Handle of keyboard inputs
        if (kbhit()) {
            char ch = getchar();

            //Move launcher left
            if ((ch == 'a' || ch=='A') && !paused && launcherPos > 0) launcherPos--;
            //Move launcher Riught
            if ((ch == 'd' || ch=='D') && !paused && launcherPos < widthOfScreen - (int)launcherSprite.size()) launcherPos++;
            //Shoot bullets
            if ((ch == 'w' || ch=='W') && !paused) {
                if (bulletAmmo != 0) {  // either infinite or positive ammo
                    bullets.push_back({launcherPos+1, heightOfScreen-2, "|", 1, '|'});
                    if (bulletAmmo > 0) bulletAmmo--; // consume ammo (specifically to level 4)
                }
            }
            //Shoot explosives
            if ((ch == 'e' || ch=='E') && !paused && explosivesAmmo > 0) {
                bullets.push_back({launcherPos, heightOfScreen-2, "(***)", 1, '*'}); explosivesAmmo--;
            }
            //To quit the game
            if (ch == 'q' || ch=='Q') running = false;
            //To pause the game
            if (ch == 'p' || ch=='P') paused = !paused;
        }

        if (!paused) {
            // Level rules
            int planeSpawnDelay = 1500;
            int missileChance = 20;
           
            bool coinsActive = true, potionsActive = false, explosivesActive = false;
            //Switch to level 2
            if (score >= 50) {
                planeSpawnDelay = 1000; 
                missileChance = 30; 
                level = 2; 
            }
            //Switch to level 3
            if (score >= 100) { 
                planeSpawnDelay = 800; 
                missileChance = 40; 
                potionsActive = true; 
                coinsActive = false; 
                level = 3; 
            }
            //Switch to level 4
            if (score >= 200) { 
                planeSpawnDelay=2200;
                missileChance=10;
                explosivesActive = true; 
                potionsActive = false; 
                if (bulletAmmo == -1) bulletAmmo = 30; 
                level = 4; 
                
                // Spawn boss only once
                if (!bossActive) {
                    boss = {widthOfScreen/2, 2, "<BOSS>", 20, 'B'}; // HP of the boss = 20
                    bossActive = true;
                }
            }

            // Spawning of entities
            if (chrono::duration_cast<chrono::milliseconds>(now - lastPlane).count() > planeSpawnDelay) {
                planes.push_back({0, rand() % (heightOfScreen / 2)+1, "<==>", 2, '>'});
                lastPlane = now;
            }
            if (coinsActive && chrono::duration_cast<chrono::milliseconds>(now - lastCoin).count() > 2000) {
                coins.push_back({rand() % (widthOfScreen-3), 0, "($)", 1, '$'});
                lastCoin = now;
            }
            if (potionsActive && chrono::duration_cast<chrono::milliseconds>(now - lastPotion).count() > 5000) {
                potions.push_back({rand() % (widthOfScreen-3), 0, "[+]", 1, '+'});
                lastPotion = now;
            }
            if (explosivesActive && chrono::duration_cast<chrono::milliseconds>(now - lastExplosiveDrop).count() > 4000) {
                explosives.push_back({rand() % (widthOfScreen-5), 0, "(***)", 1, '*'});
                lastExplosiveDrop = now;
            }

            // Movement of entities
            if (chrono::duration_cast<chrono::milliseconds>(now - lastPlaneMove).count() > 300) {
                for (auto &p : planes) p.x++;
                planes.erase(remove_if(planes.begin(), planes.end(), [widthOfScreen](Entity &p){ return p.x >= widthOfScreen; }), planes.end());
                lastPlaneMove = now;
            }
            if (bossActive) {
                boss.x += bossDir;

                // bounce at screen edges
                if (boss.x <= 0) bossDir = 1;
                if (boss.x >= widthOfScreen - (int)boss.sprite.size()) bossDir = -1;
            }


            for (auto &p : planes) {
                if (rand() % 100 < missileChance && p.x == launcherPos) {
                    missiles.push_back({p.x, p.y + 1, " v ", 1, 'v'});
                }
            }
            if (chrono::duration_cast<chrono::milliseconds>(now - lastMissileMove).count() > 250) {
                for (auto &m : missiles) m.y++;
                missiles.erase(remove_if(missiles.begin(), missiles.end(), [heightOfScreen](Entity &m){ return m.y >= heightOfScreen; }), missiles.end());
                lastMissileMove = now;
            }
            for (auto &b : bullets) b.y--;
            bullets.erase(remove_if(bullets.begin(), bullets.end(), [](Entity &b){ return b.y < 0; }), bullets.end());
            
            if (chrono::duration_cast<chrono::milliseconds>(now - lastItemMove).count() > 350) {
                auto fall = [&](vector<Entity> &v){
                    for (auto &e : v) e.y++;
                    v.erase(remove_if(v.begin(), v.end(), [heightOfScreen](Entity &e){ return e.y >= heightOfScreen; }), v.end());
                };
                fall(coins); fall(potions); fall(explosives);
                lastItemMove = now;
            }

            // Collisions
            for (auto b = bullets.begin(); b != bullets.end();) {
                bool removed = false;
                for (auto p = planes.begin(); p != planes.end();) {
                    if (collides(*b, *p)) {
                        if (b->type == '*') p->hp = 0; else p->hp--;
                        if (p->hp <= 0) { 
                            p = planes.erase(p); 
                            score += 10; 
                        }else ++p;
                        removed = true;
                        break;
                    } else ++p;
                }
                for (auto m = missiles.begin(); m != missiles.end();) {
                    if (collides(*b, *m)) { 
                        m = missiles.erase(m);
                        score += 5; 
                        removed = true; 
                        break; 
                    } else ++m;            
                }
                for (auto c = coins.begin(); c != coins.end();) {
                    if (collides(*b, *c)) { c = coins.erase(c); score += 2; removed = true; break; }
                    else ++c;
                }
                if (bossActive && collides(*b, boss)) {
                    if (b->type == '*'){ 
                        boss.hp -= 3; // explosives are stronger
                        score+=10;
                    }else{ 
                        boss.hp--;
                        score+=5;
                    }                    
                    if (boss.hp <= 0) {
                        score+=20;
                        bossActive = false;                       
                        running = false;
                    }
                    removed = true;
                }

                if (removed) b = bullets.erase(b);
                else ++b;
            }

            //Picking up of explosives and potions by the launcher
            auto pickup = [&](vector<Entity> &v, function<void()> effect){
                for (auto it = v.begin(); it != v.end();) {
                    if (collides({launcherPos, heightOfScreen-1, launcherSprite, 1, '^'}, *it)) {
                        effect();
                        it = v.erase(it);
                    } else ++it;
                }
            };
            pickup(explosives, [&](){ explosivesAmmo++; });
            pickup(potions, [&](){ health++; });
           
            for (auto m = missiles.begin(); m != missiles.end();) {
                if (collides({launcherPos, heightOfScreen-1, launcherSprite, 1, '^'}, *m)) {
                    health--;
                   
                    m = missiles.erase(m);
                } else ++m;
            }
        }

        // Draw
        vector<string> screen(heightOfScreen, string(widthOfScreen, ' '));
        drawSprite(screen, launcherPos, heightOfScreen-1, launcherSprite);
        for (auto &b : bullets) drawSprite(screen, b.x, b.y, b.sprite);
        for (auto &p : planes) drawSprite(screen, p.x, p.y, p.sprite);
        if (bossActive) drawSprite(screen, boss.x, boss.y, boss.sprite);
        for (auto &m : missiles) drawSprite(screen, m.x, m.y, m.sprite);
        for (auto &c : coins) drawSprite(screen, c.x, c.y, c.sprite);
        for (auto &po : potions) drawSprite(screen, po.x, po.y, po.sprite);
        for (auto &e : explosives) drawSprite(screen, e.x, e.y, e.sprite);

        // Draw Boss HP bar
        if (bossActive) {
            string bar = "BOSS HP: ";
            int barWidth = 20; // length of the health bar
            int filled = (boss.hp * barWidth) / BOSS_MAX_HP;

            bar += "[";
            for (int i = 0; i < barWidth; i++) {
                if (i < filled) bar += "#";  // filled portion
                else bar += " ";             // empty portion
            }
            bar += "]";

            // Print at the top row
            for (size_t i = 0; i < bar.size() && i < screen[0].size(); i++) {
                screen[0][i] = bar[i];
            }
        }

        cout << "\033[2J\033[1;1H";
        cout << "Score: " << score << "  Health: " << health << "  Level: " << level
            << " Bullets: " << (bulletAmmo==-1 ? string("INF") : to_string(bulletAmmo)) 
            << "  Explosives: " << explosivesAmmo << (paused ? "  [PAUSED]" : "") << "\n";

        for (auto &line : screen) cout << line << "\n";
        cout << "Move(A/D) Fire(W) Explosive(E) Pause(P) Quit(Q)\n";

        //Losing condition
        if (health <= 0) { 
            cout << "\nGAME OVER! Final Score: " << score << "\n";
            break;
        }
        //Winning condition
        if(bossActive==false && level==4){
            cout<<"\n YOU WIN! Final Score: "<<score<<"\n";
            break;
        }

        this_thread::sleep_for(chrono::milliseconds(35)); //Delaying the FPS for better visualisation of the game
    }

    resetTermios(); //Switch back to normal terminal mode
    return 0;
}

