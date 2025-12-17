
# Project Title

🎮 C++ Terminal Based Space Shooter 🎮


##  📖 Basic introduction of the game
**C++ Terminal Based Space Shooter** is a console-based shooting game written in **C++**.  
You play as a launcher at the bottom of the screen that can move horizontally and shoot bullets or explosives to destroy enemy planes.  

The game features:
- Multiple levels of increasing difficulty
- Score tracking
- Health and ammo management
- Random collectible items (coins, potions, explosives)

##  🕹️ Controls of the game
- **Move Left** → `A` or `a`  
- **Move Right** → `D` or `d`
- **Shoot Bullet** → `W` or `w` 
- **Shoot Explosive** → `E` or `e`(if ammo available)  
- **Pause / Resume** → `P` or `p` 
- **Quit Game** → `Q` or `q` 

##  ⚙️ Game Mechanics
- **Planes** move left → right, dropping missiles when aligned with the launcher.  
- **Missiles** target the launcher; destroy them with bullets or explosives.  
- **Coins** give extra points when shot.  
- **Potions** restore health when collected.  
- **Explosives** add ammo for powerful attacks. 
- **Boss Plane** spawns in level 4 only. 

## Scoring system
- Plane destroyed → +10 points  
- Missile destroyed → +5 points  
- Coin destroyed → +2 points  
- Boss Plane → 
  - If hit by 1 bullet, score increases by 5. 
  - If hit by 1 explosive, score increases by 10.
  - If Boss Plane is destroyed, score increases by 20.

## Health system of Player
- Launcher starts with 3 health points.  
- Each missile hit reduces health by 1.  
- Potions restore health (+1).  
- Game over when health reaches 0. 

## Health system of planes
- Normal planes' get destroyed by 1 explosive or by 2 bullets.
- Boss plane's health get reduced by 3 by 1 explosive and reduced by 1 by 1 bullet. It gets completely destroyed when its total health (initially set to 20 becomes less than or equal to 0).

## Levels
1. **Level 1** → Planes, missiles, coins (basic difficulty).  
2. **Level 2** → Faster planes & missiles after **50 points**.  
3. **Level 3** → Potions drop, coins stop after **100 points**.  
4. **Level 4** → Explosives drop, no potions after **200 points**, Boss Plane spawns. 

## Building and running the game

### 1. Download and extract the project files in the same folder. Open a terminal in that folder.

### 2. Install build tools ✅
Make sure you have `g++` and `make` installed:
```
sudo apt update
sudo apt install build-essential 
```

### 3. Compile the game 💻
```
make
```

### 4. Run the game 🚀
```
./game
```

### 5. Clean build files
```
make clean
```

### 6.Profile performance
```
make profile
```

## Author
Sudipto Ghosh
Roll: 240150042
