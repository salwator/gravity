## What is it?

It is a simple celestial body simulator written from the beginning using newer C++11/14 features.
Purpose of this project is to test some ideas in practice and have fun seeing planets moving around ;)

## Want to play around?

It is not really a much usefull project right now but if you really want to see it working here are some hints.

### Instalation

You need [GLFW3](http://www.glfw.org/docs/latest/) library and its headers installed in your system. Linux machines supported only.
It uses experimental [Pake](https://github.com/podusowski/pake) build system.
You will also need Clang >3.5 or GCC >4.9.

```bash
$ git clone https://github.com/salwator/gravity.git
$ cd gravity
$ ./pake.py gravity
$ __build/__default/gravity
```

Use  `./pake.py -c clang gravity` or `./pake.py -c gcc gravity` for explicit compiler configuration

### Configuration

Currently it is configured only in main function code, however importing from csv file is already implemented.
Will be used soon.

## How it works

Simulation is written in functional style for cleaner code and easier parallel computation. It uses to standard async to spawn calculation.
OpenCL is going to be an option soon, hopefuly.

