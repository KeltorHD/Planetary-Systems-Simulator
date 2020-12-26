# Planetary Systems Simulator

A program for creating planetary systems and simulating the movement of objects in systems. 
Main system: Windows

---

# Installation

Download the latest installer in the release tab and install it

---

# Language and Libraries

Language: C++17 (MSVC)
Libs:
 - [SFML 2.5.1](https://github.com/SFML/SFML)
 - [ImGui 1.79](https://github.com/ocornut/imgui)
 - [ImGui for SFML binding](https://github.com/eliasdaler/imgui-sfml)
 
---
# Features
1) Simulation of a system of cosmic bodies
2) Adding new space bodies to the simulation (body types: star, dwarf star, planet, dwarf planet, satellite, asteroid)
3) Changing the parameters of cosmic bodies in the system (coordinates, body type, mass, velocity, radius, color, name)
4) Removal of cosmic bodies in the system
5) Change of name, description of the system of cosmic bodies
6) Ability to save the edited system of space bodies
7) Ability to load the system of space bodies
8) Ability to create a new system of space bodies
9) Control over the simulation:
9.1) Simulation with normal speed movement (does not depend on the number of frames per second) - Space key
9.2) Simulation with movement at a speed multiplied by the selected coefficient (does not depend on the number of frames per second, the coefficient is selected by the slider, from 0.1 from normal to 10.0 from normal) - Space key
9.3) Stop simulation - Space key
9.4) Restore the original state of the simulation (the last saved state is returned) - key R
9.5) Center the camera on the most massive body once - C key
9.6) Center the camera on the most massive body always - X key
10) Camera control
10.1) Move camera up - key W
10.2) Move camera down - key S
10.3) Move camera left - A key
10.4) Move camera right - D key
10.5) Increase scale - key + / PageUp
10.6) Zoom out - key - / PageDown
11) The ability to hide / open the interface of interaction with the system, control over the simulation
12) Multilingual support (2 languages are available by default: Russian, English), a convenient format for translating text into the required language
13) Ability to select the resolution of the program
14) Ability to select full screen mode
15) Ability to change the font size (from 12 to 40)
16) Sounds of pressing buttons, adding a planet
17) Menu background music and simulations
18) Drawing the trajectory of movement of objects

---
# ScreenShots

![screenshot](/screenshots/1.png)
![screenshot](/screenshots/2.png)
![screenshot](/screenshots/3.png)
![screenshot](/screenshots/4.png)
![screenshot](/screenshots/5.png)
![screenshot](/screenshots/6.png)
![screenshot](/screenshots/7.png)
![screenshot](/screenshots/8.png)
![screenshot](/screenshots/9.png)
![screenshot](/screenshots/10.png)
![screenshot](/screenshots/11.png)

---
# License

MIT

**Free Software, Hell Yeah!**
---