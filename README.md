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
  If you don't have ```node-gyp``` already installed globally, you should install it as well:
  ```bash
  $ npm install -g node-gyp
  ```

- This project uses native C++ modules exposed to Node through the Node Addon API (NAPI). Since C++ is a compiled language and is not interpreted, the native modules will have to be compiled on each fresh download. Navigate to the ```routing/``` subdirectory and run the following commands:

  ```bash
  $ cmake .
  $ make
  ```

- Finally, compile the node module by running the following command from the root project directory:

  ```bash
  $ node-gyp rebuild
  ```
  The compiled node module will be placed saved as ```native_ospf.node``` under the ```build/Release``` subdirectory and can be imported into Node.js like so:
  
  ```javascript
  const native_ospf = require('./build/Release/native_ospf.node');
  //...
  ```

### Running the project

- In two seperate terminals, run both of these commands

  ```bash
  $ npm run watch
  $ npm run start
  ```

- This will both start up the electron app as well as the babel transpiler

### C++ Testing Environment

This project uses [GTest](https://github.com/google/googletest.git) as a testing framework and CMake as a build tool. For those that are unfamiliar with these tools, you can check out a quick start guide [here.](https://google.github.io/googletest/quickstart-cmake.html) **NOTE**: You must have at least CMake v2.8.12 installed to test the routing algorithms.

You can verify that our OSPF implementation works properly by running the test suite yourself. After cloning this repo, run CMake to configure the build environment:

- Navigate to the ```routing``` subdirectory
- Run the following commands to build for the first time

  ```
  $ cmake .
  $ make
  ```

Once those commands have finished, the stand-alone executable called ```ospf_test.out``` will be in the ```routing/``` parent directory and can be run to verify proper functionality. There will also be a number of CMake files in ```routing/``` that can be ignored. The OSPF routing protocol functions are in ```ospf.cpp```, ```Router.cpp```, and ```LSDB.cpp```. The test functions can be found in ```ospf_test.cpp```. The code responsible for exposing the C++ modules as a Node.js module reside in the ```routing/native_ospf/``` subdirectory.

To recompile the test suite after making changes, navigate to the ```routing/``` directory and run ```make``` once again. You do not have to run ```cmake``` more than once, even if there are changes to CMakeLists.txt, CMakeLists.txt.gtest, or CMakeLists.txt.rapidjson in a pulled commit.