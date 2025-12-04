# HyperDash

![Language](https://img.shields.io/badge/language-C%2B%2B17-blue) ![Library](https://img.shields.io/badge/library-SFML_2.6.2-green) ![Platform](https://img.shields.io/badge/platform-Windows-lightgrey)

**HyperDash** is a 2D infinite runner platformer developed in C++ using the SFML library. The project was designed to demonstrate the practical application of Object-Oriented Programming (OOP) principles in a real-time engine environment. The player navigates a procedurally generated environment, dodging obstacles and engaging enemies while managing resources and power-ups.

---

## Gameplay Features

* **Procedural Generation:** The game world generates coins, spikes, and enemies dynamically based on algorithms that scale with the game speed.
* **Dynamic Difficulty:** Implements a difficulty scaling system (Easy, Normal, Hard) that adjusts game speed, spawn rates, and enemy density in real-time.
* **Combat System:**
    * **Player:** Equipped with a projectile system allowing for standard and rapid-fire modes.
    * **Enemies:** Features distinct enemy types including Walkers (ground-based linear shooters) and Drones (aerial sine-wave attackers).
* **Power-Up System:**
    * **Invincibility:** Grants temporary immunity to damage.
    * **Rapid Fire:** Reduces projectile cooldown to 0.15s.
    * **Health Pack:** Restores health or awards bonus points if health is at maximum.
* **Data Persistence:** A high-score system that utilizes file I/O to persist player data between sessions.
* **Audio & Visuals:** Integrated SFML Audio for sound effects and visual feedback systems for damage and status effects.

---

## Technical Implementation & OOP Concepts

This project was built to demonstrate advanced Object-Oriented Programming principles in C++. Below is an overview of how these concepts were applied to the codebase.

### 1. Abstraction & Pure Virtual Functions
Abstraction is used to define a strict contract for game entities, ensuring that derived classes implement specific behaviors while hiding the underlying complexity.

* **Implementation:** The `Enemy` and `Obstacle` classes are defined as **Abstract Base Classes**.
* **Code Example (`hyperdash.hpp`):**
    ```cpp
    class Enemy {
        virtual void update(float deltaTime, float scrollSpeed) = 0; // Pure Virtual Function
    };
    ```
* **Application:** This prevents the instantiation of a generic "Enemy" and forces derived classes (like `Walker` or `Drone`) to define their own unique movement patterns.

### 2. Polymorphism
Runtime polymorphism is utilized to manage diverse game entities through a unified interface, simplifying the main game loop.

* **Implementation:** The `GameState` class manages enemies using a `std::vector` of smart pointers to the base class.
    ```cpp
    std::vector<std::unique_ptr<Enemy>> enemies;
    ```
* **Application:** During the update cycle, the program iterates through this list. The correct `update()` method is resolved at runtime (e.g., executing sine-wave logic for Drones vs. linear logic for Walkers) without the `GameState` needing to know the specific type of the enemy.

### 3. Inheritance
Inheritance is used to promote code reuse and establish a hierarchical relationship between entities.

* **Hierarchy:**
    * `Obstacle` (Base) -> `Spike`, `Projectile`
    * `Enemy` (Base) -> `Walker`, `Drone`
* **Application:** Common attributes such as sprites, position vectors, and rendering logic are defined once in the base class. Derived classes only contain logic specific to their behavior, keeping the code DRY (Don't Repeat Yourself).

### 4. Composition
Composition is used to model strong ownership relationships ("Has-a").

* **Implementation:** The `GameState` class maintains exclusive ownership of the `Player` object using `std::unique_ptr`.
    ```cpp
    std::unique_ptr<Player> player;
    ```
* **Application:** The lifecycle of the `Player` is strictly bound to the `GameState`. If the game state is destroyed or reset, the player object is automatically cleaned up, preventing memory leaks.

### 5. Encapsulation
Encapsulation is enforced to protect the internal state of objects and maintain data integrity.

* **Application:** All member variables (such as `health`, `speed`, and `texture`) are declared as `private` or `protected`. Interaction with these variables occurs strictly through public accessor and mutator methods (e.g., `getHealth()`, `takeDamage()`), ensuring that game logic cannot arbitrarily modify object states.

---

## Controls

### Main Menu
* **Left / Right Arrows:** Cycle Difficulty
* **Enter:** Start Game
* **Esc:** Quit Application

### In-Game
* **W / Up Arrow:** Jump
* **A / Left Arrow:** Move Left
* **D / Right Arrow:** Move Right
* **S / Down Arrow:** Fast Fall / Ground
* **Spacebar (Hold):** Shoot

### Debug Tools
* **`~` (Grave Key):** Toggle Debug Overlay
* **1 - 5:** Spawn specific entities (Coins, Spikes, Walkers, Drones, Power-Ups)
* **0:** Toggle God Mode

---

## Project Structure

````

HyperDash/
├── headers/           \# Header files (.hpp)
│   ├── hyperdash.hpp  \# Core Game Logic & Entity Definitions
│   └── screenObjects.hpp \# UI & Menu Classes
├── src/               \# Source files (.cpp)
│   ├── main.cpp       \# Entry point
│   ├── GameState.cpp  \# Main game loop & logic management
│   ├── Player.cpp     \# Player physics & input handling
│   ├── Enemy.cpp      \# Enemy AI implementations
│   ├── Projectile.cpp \# Projectile logic
│   └── ...
├── sprites/           \# Graphical Assets
├── sounds/            \# Audio Assets
├── fonts/             \# Typography
├── Makefile           \# Build configuration
└── README.md          \# Project Documentation

````

---

## Build & Run Instructions

### Prerequisites
* **Compiler:** MinGW-w64 (g++) supporting C++17 standard.
* **Library:** SFML 2.6.2 (GCC MinGW 64-bit version).
* **Build Tool:** `mingw32-make`.

### Setup
1.  Ensure `g++` and `mingw32-make` are added to your System PATH.
2.  Update the `SFML_PATH` variable in the `Makefile` to point to your local SFML installation directory.
3.  Ensure the necessary DLLs (`sfml-graphics-2.dll`, `sfml-audio-2.dll`, `openal32.dll`, etc.) are present in the build directory, or add their folder to PATH (NOT RECOMMENDED!).

### Compilation
Open a terminal in the project root and execute:
```bash
mingw32-make
````

### Execution

```bash
mingw32-make run
```
