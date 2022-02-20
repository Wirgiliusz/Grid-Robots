# Grid Robots
Simulation of storage house - graphical visualisation of swarm of robots steered by event-based controller.

---
## Project Description
The goal of the project was to create event-based controller for a swarm of robots moving in a grid-like environment and visualisation of the simulation. The project was divided into two main parts:
1. Creating an graphical visualisation of the system – grid containers, moving robots, items, storage points.
2. Designing an event-based control for the system which will command the robots in optimal way without causing any collisions.

The main elements of the simulation are:
- Cells - placed in a way that form grid-like environment for the simulation.
- Items - objects that must be picked-up and delivered.
- Storage points - cells where items can be delivered.
- Robots - can move and deliver items.

### Program operation
The controller program uses an text input file in proper format that defines all the parameters for the simulation: grid size and coordinates of all the objects. 
The simulation program uses data received from the controller to draw the visualisation which shows current state of the simulation.
The communication between two programs is based on FIFOs.

---

## Results
The Grid Robots project consists of two parts:
1. The simulation and visualisation part written in Python and pygame module ([Simulator directory](/Simulator))
2. The event-based controller part written in C ([Controller directory](/Controller))
<img src=/docs/imgs/grid_robots.png width="700">

### Working simulation
<img src=/docs/imgs/grid_robots_gif.gif width="700">
