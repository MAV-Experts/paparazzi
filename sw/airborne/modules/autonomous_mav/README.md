# Autonomous MAV Module

© 2022 Group 10 | Autonomous Flight for Micro Air Vehicles (TU Delft)

This module contains the control software to let the Bebop drone fly 
fully autonomous though space just based on the monocular camera
feed. 

## Working with synthetic self pointers

Because C does not support classes and object-orientation natively, 
we use synthetic self pointers (a bit like in Python) that are passed
in every `object function` call (= injected functions to structs). 
Make sure you don't forget to always pass the self reference to methods 
of a certain kind.

## Setup and writing own Navigators / ObstacleDetectors

In the directories `navigator` and `obstacle_detector`, you will find
`.h` files that contain the basic layout of a navigator or 
obstacle_detector. In the `(...)/module` folders you will find a first
dummy implementation of both which is called *BasicNavigator* or 
*BasicDetector*. 

When you want to create your own modules, just copy those and rename
the `Generation Function` which is called like **createBasic(...)** at 
the bottom with your name (whatever name you want to give your detector
or navigator). The generation function fulfils a few tasks in order to
mimic a class/object-oriented architecture in C: 

- Pass implementations of the function prototypes
- Do anything that would usually be in a constructor
- Create an object (at an address) and pass it back

In general, try to copy and understand the logic from the Basic(..) modules.
Copy those files, rename it to whatever kind of navigator or detector you are
creating and start from there. 

## Work in progress

Currently, ways are searched to inject more attributes to the struct objects 
while the program is already compiled. 