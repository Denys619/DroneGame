# 🛸 FPV Drone Game - UE5 Mini Project

**FPV Drone Game** is a mini-game created on **Unreal Engine 5** using **C++**, where the player controls a drone in FPV style, shoots at turrets, avoids attacks, picks up first aid kits and ammo, and monitors health through the HUD.

> ⚠️ Make shure you have installed Git LFS, before cloning this repository (There is large file)

https://github.com/user-attachments/assets/882c665e-f12f-4803-9982-138b4ca3c5ea

## Gameplay

You control a small FPV-drone equipped with a front-facing camera and a limited number of explosive projectiles.  
Your mission: destroy enemy turrets before they destroy you.

### 🕹️ Controls**

| Action         | Key / Mouse      |
|----------------|------------------|
| Move Forward   | W                |
| Move Backward  | S                |
| Strafe Left    | A                |
| Strafe Right   | D                |
| Ascend         | Space            |
| Descend        | C                |
| Look Around    | Mouse Movement   |
| Shoot          | Left Mouse Button|

**The game contains pickups:**
- 🧰 **Health Kit** - restores part of the drone's health.

![Записування з екрана 2025-04-21 191521](https://github.com/user-attachments/assets/b9cd4b62-1928-45b7-8d21-990afe9773d4)
  
- 🔫 **Ammo Crate** - replenishes the supply of shells.

Pickups are scattered around the map - look for them to survive longer and fight turrets more effectively.

![Записування з екрана 2025-04-21 191346](https://github.com/user-attachments/assets/672f71c3-fddb-4716-8e91-84c54030d1ac)

## Features

- [x] FPV drone movement
- [x] Shooting with a kill switch and ammo count
- [x] Turrets with auto-aiming and firing
- [x] Drone health and HP UI
- [x] Ammo pickup + display of the amount under the sights
- [x] Pickups: First aid kits and ammo
- [x] Sight and vignette
- [x] Visual feedback on hit
- [x] Turrets can be destroyed
- [x] Turret color change when taking damage

## Planned:

- [ ] Soundtracks and sound effects
- [ ] Turret explosion animation
- [ ] Main menu and restart
- [ ] UI for win/lose
- [ ] More levels or scenarios
- [ ] Integration into build (Windows exe)

## ⚙️ How to run

1. Open the `.uproject' file through Unreal Engine 5.
2. Launch the `MainLevel` map (it will automatically open).
3. Click “Play.”

---

## Build (optional)

> Build has not yet been created. We plan to add `.exe' and instructions for running without UE5 later.

