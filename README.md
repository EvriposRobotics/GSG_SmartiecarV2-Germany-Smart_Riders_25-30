
# WRO World Finals 2023 by Team Smart Riders 

![logo](https://github.com/Nezar187/GSG_SmartiecarV2/assets/131591590/ee130cae-55c6-4d7b-ba02-26129456f831)

# !! Please note: this repository is still under construction!! 
## Table of Contents 📚👇

1. [Overview](#overview) 🌟
2. [Design Process](#Design-Process)🔧
    - [Mobility Management](#Mobility-Management)
    - [Chassis](#Chassis)
    - [Assembly Instructions](#assembly-instructions) 🛠
    - [Schematics](#schematics) 📐👀
    - [Driving Motor and Gearing](#Driving-Motor-and-Gearing)
    - [Steering Mechanism](#Steering-Mechanism)
    - [Power and Sense Management](#Power-and-Sense-Management)  
    - [Power supply](#Power-supply)
    - [Controllers](#Controllers)
    - [Sensors](#Sensors)
    - [Components List](#components-list) 🔍
3. [Software Design](#software) 💻👨‍💻
    - [Setup Instructions](#setup-instructions) 🚀
    - [Programming Languages](#programming-languages) 👩‍💻
    - [Dependencies](#dependencies) 
4. [Utilities](#utilities) 🛠
    - [Debugging Tools](#debugging-tools) 🐞🔍 
5. [Team Photos](#team-photos) 📸
6. [Demonstration Videos](#demonstration-videos) 🎥
7. [Contributors](#contributors) 👥
8. [sources](#sources) ℹ

   
<a name="overview"></a>


## Overview 🌟

![Foto 01 10 23, 13 22 02](https://github.com/Nezar187/GSG_SmartiecarV2/assets/131591590/60bbc2ea-8552-432e-a204-dabbc420be3d)

Welcome to the official GitHub repository for Team Smart Riders from Germany, participating in the WRO World Finals 2023. This repository contains all the code, documentation, and resources for our project, SmartiecarV2. We have used scoring criteria from the ruleblock as headings in our documentation to make it easy to navigate our repository.

<img src="https://github.com/Nezar187/GSG_SmartiecarV2/assets/131591590/7b0c5513-870f-475c-9993-cfca6aa5659f" width="75%">

For the wold final we decided to redesign our future engineers car.
Now our car consists of an arduino which controls the sensors and the motors and a raspbarry pi which does all the image prossesing in the obstacle race.


<a name="Design-Process"></a>


### Design Process

For the World Final, we ran a throurogh analysis of our German Final car version to identify, what worked well and what did not works so well.
From that, we compiled a list what to keep and what we want to improve..

Results of analysis:
ok:
- arduino work with ultrasonic and gyro
- rgb lcd display for feedback
- gyro sensor precision

inprove:
- image processing unstable
- curve detection in obsticle race
- precision
- stopping in the start area
- hardware repairs too time consuming due to complex chassis
- car track width to large (17cm)
- driving low speed
- breaking distance to long

  For the WRO Final car, we designed 3 modular layers to seperate funktions from each other and make it much easier to repair.
  We adress the detailed solutions we found to the identified issues in detail in the following chapters.

<img src="https://github.com/Nezar187/GSG_SmartiecarV2/assets/131591590/40d8d0fe-efb5-4642-bb60-c71df9333944" width="75%">


<a name="Mobility-Management"></a>


### Mobility Management 


<a name="Chassis"></a>


### Chassis

We used the Totemmaker construction system to design and build our own car chassis. This set consits of plastic beams and plates as structural
elements and aluminium connectors. For specific parts as the front of the wheel barrel and the motor bracket, totemmaker.net offers a free STL-Libary of 3D-printable parts.
To get a small but strong structure, we used rectangular grids as the basic element of each layer.



Tires:

In our building process we tested different tires from different manufacturers. We tested the tires that were included in the Totemmaker contruction
system, standard racing tires we bought from Funduino and onother set of tires with no tread pattern.
During the initial test drives, we noticed that the tires we took from the Totem Maker construction system were too large and were rubbing against the frame. The second set of tires from Funduino were smaller and harder, but they often caused oversteering when the car was turning sharply. Now, for the final setup, we are using softer tires without any tread pattern, which maximizes ground contact. 

<img src="https://github.com/Nezar187/GSG_SmartiecarV2/assets/131177565/5ae149b4-5e5d-40b0-987d-e4460998b52e" width="75%">


<a name="assembly-instructions"></a>


### Assembly Instructions 🛠

Detailed assembly instructions can be found in the [Assembly Guide](./hardware/README.md).


<a name="schematics"></a>


### Schematics 📐

Circuit schematics and hardware layouts are available in the [Schematics folder](./hardware/schemes).


<a name="Driving-Motor-and-Gearing"></a>


### Driving Motor and Gearing

We tested two different motors. One which operates on 6V and another which does on 12V. In our last version of our car we used a 12V motor. We wanted to use the 6V motor so that we do not have so many different voltages in our car. In our testing the 6V motor broke quite often. The 12V motor was more reliable. That is why we decided to use the 12V motor instead of the 6V.


<a name="Steering-Mechanism"></a>


### Steering Mechanism
Because of the layers in our car, the servo motor for the steering had to be mounted on the first (lowest) layer. Because of that it was difficult to connect the servo with the steering mechanism. The plastic beams from the layer around the servo limited its range of motion. That is why we had to extend the steering rod to succesfully connect the servo to the steering mechanism.

(Picture steering rod)

Steering Trapeze:

<img src="https://github.com/Nezar187/GSG_SmartiecarV2/assets/131177565/0348c67c-ff02-438a-8701-6f1718773c30" width="50%">


<a name="Power-and-Sense-Management"></a>


### Power and Sense Management


<a name="Power-supply"></a>


### Power supply


<a name="Controllers"></a>


### Controllers


<a name="Sensors"></a>


### Sensors


<a name="components-list"></a>


### Components List 📦

A list of all hardware components used in the project can be found [here](./hardware/README.md).


<a name="software"></a>


## Software 💻


<a name="setup-instructions"></a>


### Setup Instructions 🚀

Follow the [Software Setup Guide](./software/README.md) to set up the development environment.


<a name="programming-languages"></a>


### Programming Languages 👩‍💻

- Python
- C++ (Arduino)
- C


<a name="dependencies"></a>


### Dependencies 📚

- Python Libraries: List here
- C++ Libraries: List here
- C Libraries: List here


<a name="utilities"></a>


## Utilities 🛠


<a name="debugging-tools"></a>


### Debugging Tools 🔍

Debugging tools:
for python = visual studio code
for arduino = arduino IDE


<a name="team-photos"></a>


## Team Photos 📸

Meet the team behind this project in the [Team Photos section](./Teamphotos).


<a name="demonstration-videos"></a>


## Demonstration Videos 🎥

- [opening race](./videos/demo1.mp4)
- [obstacle race](./videos/demo2.mp4)


<a name="safety-guidelines"></a>


## Safety Guidelines ⚠️

For safety measures, especially when dealing with electrical components, please read our [Safety Guidelines](./SafetyGuidelines.md).


<a name="contributors"></a>


## Contributors 👥

- [Nezar187](https://github.com/Nezar187)
- [NoahPX44](https://github.com/NoahPX44)
- [LuciusFisch](https://github.com/LuciusFisch)


<a name="sources"></a>


## Sources
- Help with code Errors [Stackoverflow](https://stackoverflow.com/)
- 
-
