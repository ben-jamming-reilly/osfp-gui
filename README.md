### Setting up the project

- Install node.js for debian based linux distributions, type into the terminal

  ```bash
  $ sudo apt-get update
  $ sudo apt-get install nodejs
  ```

- Install all npm dependencies, type into the terminal

  ```bash
  $ npm install
  ```

### Running the project

- In two seperate terminals, run both of these commands

  ```bash
  $ npm run watch
  $ npm run start
  ```

- This will both start up the electron app as well as the babel transpiler

### C++ Testing Environment

This project uses [GTest](https://github.com/google/googletest.git) as a testing framework and CMake as a build tool. For those that are unfamiliar with these tools, you can check out a quick start guide [here.](https://google.github.io/googletest/quickstart-cmake.html) **NOTE**: You must have CMake installed to test the routing algorithms.

You can verify that our OSPF implementation works properly by running the test suite yourself. After cloning this repo, run CMake to configure the build environment:

- Navigate to the ```routing``` subdirectory
- Run the following commands to build for the first time

  ```
  $ cmake -S . -B build
  $ cd build/
  $ make
  ```
Once those commands have finished, the stand-alone executable called ```ospf_test.out``` will be in the ```routing``` parent directory and can be run to verify proper functionality. The OSPF routing protocol functions can ```ospf.cpp``` and the test functions can be found in ```ospf_test.cpp```.

To recompile the test suite after making changes, navigate to the ```build/``` directory and run ```make``` once again.