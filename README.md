# ValNumProject

## Description

This is a student project for a course in validated numerics at Uppsala
University. It consist of libraries for interval arithmetic, automatic
differentiation and Taylor arithmetic with both doubles and intervals. The
libraries aren't completely rigorous since the interval extensions of the
standard functions are not rigorously implemented. It also has capabilities for
finding zeros of functions and integrating functions. A number of solved
exercises are included. The exercises can be found here:
[http://www2.math.uu.se/~figueras/teaching/contents_course/2015_Fall_Validated_Numerics/index.html](http://www2.math.uu.se/~figueras/teaching/contents_course/2015_Fall_Validated_Numerics/index.html)

## Requirements

This project requires the following:

* `gcc` with support for `c++14`,
* `make`,
* `gnuplot`

## Building

To build the project, run `make all` in the projects root folder.

To remove object files and compiled exercises run `make clean` in the projects
root folder.

## Additional comments

The solutions come with additional comments describing the solution strategy, if
needed. These are contained in the `comments` folders inside each `ComputerLab`
folder. Some of the comments are in `LaTeX` format and will hence require
`LaTeX` to build. This is not done by `make`.
