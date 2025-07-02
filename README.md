# MosquitoDefenseGLUT

A C++ OpenGL/GLUT-based 2D simulation that visualizes a smart mosquito control system using an aerator mechanism. This educational project demonstrates how mosquito larvae in stagnant water (tubs) can be eliminated through an automatic aeration system triggered by larval presence.

## 🐞 Overview

In this simulation:
- You control a **mosquito** that moves around the screen.
- Pressing `Enter` drops **larvae** into a **water tub**.
- A smart **aerator system** detects larvae inside the tub and activates to destroy them with a rotating blade.
- The simulation includes **moving clouds, cars, humans, flowerpots, buildings**, and a **sun** to create an urban environment.

This project combines **interactive graphics**, **environmental awareness**, and **basic AI-like logic** to visualize a potential vector control strategy.

---

## 🚀 Features

- 🎮 Keyboard-controlled mosquito movement (`W`, `A`, `S`, `D` or arrow keys)
- 🔽 Drop larvae using `Enter`
- 💨 Smart aerator activates automatically when larvae are detected
- 🧠 Random chance-based larvae destruction logic
- 🌤️ Animated sky, clouds, cars, and pedestrians
- 🏙️ Urban scenery with flower pot, buildings, and road traffic

---

## 🛠️ Technologies Used

- **Language**: C++
- **Graphics Library**: OpenGL with GLUT (FreeGLUT or similar)
- **Rendering**: 2D graphics using primitive shapes (circles, polygons)
- **IDE**: Compatible with Code::Blocks, Visual Studio, etc.

---

## 🎮 Controls

| Key         | Action                      |
|-------------|-----------------------------|
| `W / ↑`     | Move mosquito up            |
| `S / ↓`     | Move mosquito down          |
| `A / ←`     | Move mosquito left          |
| `D / →`     | Move mosquito right         |
| `Enter`     | Drop larva                  |
| `ESC`       | Exit program                |

---

## 📸 Screenshots

>![{B80F1199-E17D-4F87-B893-B410DCF6349A}](https://github.com/user-attachments/assets/19e3a87c-5560-4157-bea5-b7643392119e)

> ![{38F47EB8-E154-4F62-81C9-194308C48564}](https://github.com/user-attachments/assets/515910cd-2f9f-4de5-8a15-7d2a30a1b3ae)


---

## 🧪 How It Works

1. The mosquito moves freely around the screen.
2. When the user presses `Enter`, a larva is dropped beneath the mosquito.
3. If the larva lands in the water tub, it's marked as “landed”.
4. Once larvae are in the tub, the aerator starts spinning for a duration.
5. During aeration, larvae in the tub have a chance of being destroyed (simulated probabilistically).
6. Destroyed larvae are removed from the tub and scene.

---

## 🔧 Setup & Installation

1. Make sure you have **C++** compiler and **GLUT** (FreeGLUT or GLUT32) installed.
2. Clone this repository:
   ```bash
   git clone https://github.com/your-username/MosquitoDefenseGLUT.git
   cd MosquitoDefenseGLUT
