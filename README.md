**HAL (Heuristically programmed ALgorithmic computer) is a sentient computer 
(or artificial intelligence) that controls the systems of the UrLab spacecraft 
and interacts with the Hackerspace crew.**

The HAL project is an arduino based Human - Hackerspace interface, meant to be 
easy to use and reuse. It might as well be suitable for domotic applications. 
It consists of 3 main components:

* **hal-arduino** (this repo): The arduino library defining HAL behaviour
* **hal-driver**: An Fuse interface to control arduino
* **halpy**: A high-level Python library that automates a lot of boilerplate code

# HAL Arduino library

At the moment, the HAL library defines 4 types of components:

| Type   | Analogic (0..1) | Binary (0 or 1) |
| ------:|:---------------:|:---------------:|
| Input  |     Sensors     |     Triggers    |
| Output |   Animations    |     Switchs     |

