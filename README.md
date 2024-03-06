# Fan-Controlled Ball Positioning Table Project

## Project Description

Welcome to the repository for the Fan-Controlled Ball Positioning Table Project, conducted by students on the robotic pathway at ECAM. This project integrates hands-on experience from applied internships into a practical robotics assignment. Our task was to create a responsive table system that could adjust the position of a ball using an array of fans. The goal was to centralize the ball or guide it along a pre-defined path, starting from any position on the table.
![image](https://github.com/Waranika/Fan-project-control-table/assets/58334072/099868c4-85e5-43ef-814e-47ca61ce484a)


## Repository Structure

- `.vscode`: Contains configuration files for Visual Studio Code, including setup for code formatting, debugging, and task running specifics.

- `Arduino`: This directory houses all the Arduino code used to control the hardware aspects of the project. It includes scripts for sensor data acquisition and fan control logic.

- `PC`: Here, you will find the code that runs on the computer, including algorithms for processing sensor data, calculating fan speed adjustments, and user interface code.

- `build`: Contains compiled binaries and any necessary files generated as part of the build process. This folder is useful for quickly deploying the software on new hardware.

- `README.md`: The file you are currently reading, designed to provide an overview of the project and instructions on how to navigate the repository.

## Features

The codebase is organized into distinct modules for ease of understanding and collaboration:

- **Sensor Data Acquisition**: The Arduino code includes scripts for collecting data from sensors placed around the table to detect the position of the ball.
  
- **Control Logic**: The algorithms that process sensor data and determine the appropriate response from the fans are located within the PC directory.

- **Debugging and Communication Tools**: Within the `.vscode` folder, there are custom utilities that facilitate efficient communication between the PC and the Arduino, as well as aid in debugging.

- **Explicit Casting and Data Structures**: Updates have been made to ensure type safety and organized data management across the codebase.


![image](https://github.com/Waranika/Fan-project-control-table/assets/58334072/b7a43099-2d80-4d52-a26d-268094bac173)

## Getting Started

To get started with this project, clone the repository to your local system. You will need the Arduino IDE for the Arduino code and an appropriate C++ development environment set up on your PC.

1. Install the required tools and libraries as specified in the `docs/` folder.
2. Open the Arduino code within the Arduino IDE and upload it to your Arduino hardware.
3. Compile the PC code using the build instructions provided in the `build/` folder.

## Usage

The project is intended to be used as a platform for further research and development in robotic controls and systems integration. It serves as a practical example for students and enthusiasts looking to understand the intricacies of hardware-software interaction in robotics.

![image](https://github.com/Waranika/Fan-project-control-table/assets/58334072/8891b965-ee8f-4e9e-9d27-f202d81f758e)


## Contributing

We welcome contributions from the community. To contribute, please fork the repository, make your changes, and submit a pull request for review.

## Questions

For questions or support, please open an issue in the repository, and one of our team members will get back to you.


---

We hope this README helps you navigate and understand the structure and intent behind our Fan-Controlled Ball Positioning Table Project. Enjoy exploring the code and contributing to the advancements of educational robotics!
