<!-- Copyright 2017 Dan Lilja -->

<!-- Licensed under the Apache License, Version 2.0 (the "License"); -->
<!-- you may not use this file except in compliance with the License. -->
<!-- You may obtain a copy of the License at -->

<!-- http://www.apache.org/licenses/LICENSE-2.0 -->

<!-- Unless required by applicable law or agreed to in writing, software -->
<!-- distributed under the License is distributed on an "AS IS" BASIS, -->
<!-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. -->
<!-- See the License for the specific language governing permissions and -->
<!-- limitations under the License. -->

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
