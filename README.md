# CarND-Controls-PID
Self-Driving Car Steering control

---

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

There's an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

The project uses Google's coding style [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project details

The repo contains code to keep an autonomous vehicle on track by controlling the steering angle. It uses a Proportional (P), Integral (I), Differential (D) - PID controller approach to do so. The PID class in the repo stores values for each of the terms and calculcates the total error based on the differential and integral. The main.cpp code has an option to run the simulator using the plain PID approach or using the Twiddle algorithim.

To keep the vehicle on track right off the start, PID is initialized with values for each of the three parameters. The values tried for each were -

P: tried values from 0.09 to 0.01 with a lot of intermediate values. It gives the best performance with P set to 0.05
I: values tried ranged from 0.001 all the way up to 1.0. A value of 0.005 had the best error rate
D: a range of values from 1 to 10 were experimented with and values between 4.5-5.5 gave the best results

The throttle was set to a constant 0.5. With these values the error is under control and the vehicle stays on course consistently 


