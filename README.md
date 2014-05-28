The Degree Driven Algorithm Design Library
-------------------------------------------------------------------------------

Algorithm designers try to minimize use of resources of time and memory space.
Our work considers arithmetic precision as another resource, and minimizes the
degree of polynomials used in the geometric tests or predicates that are
applied. This repository contains all code built as part of the Degree Driven
Geometric Algorithm Design project at UNC Chapel Hill.

You probably want to...

  * [Get the source code and build it on your machine](.#clone-compile-and-run).
  * [Figure out what each of the projects are](.#what-is-each-project).
  * [View the source documentation to understand implementation details](.#source-documentation).
  * [View the user documentation to learn how to use one of the applications](.#user-documentation).

#### Clone, Compile and Run


Since we are using Qt to create any GUI elements, the project is able to
compile on all three major operating systems, Windows, Mac, and Linux.
Currently, I only provide instructions that I know to work on Windows 7, but
the procedure is essentially the same on other systems.

##### Install Prerequisite Software and Clone Repository

  1. If you're new to Git and Github, read through the help to get set up,
     starting [here](https://help.github.com/articles/set-up-git).
  2. Download and install [Visual Studio 2010 Express](http://www.microsoft.com/visualstudio/eng/downloads).
     The important part here is that you somehow have the 2010 Visual C++
     compiler.
  3. Download and install the [Qt 5.1.0 MSVC 2010 OpenGL SDK](http://download.qt-project.org/official_releases/qt/5.1/5.1.0/qt-windows-opensource-5.1.0-msvc2010_opengl-x86-offline.exe).
  4. Check out the source code using `git clone https://github.com/freemancw/DDAD.git`
     (or the SSH url if you prefer, it doesn't matter).

##### Understand the Directory Structure

> * build
> * projects
>   * DDAD.pro
>   * DDAD.pri
>   * mathlib
>       * mathlib.pro
>       * lib
>           * mathlib.lib
>   * rationalcad
>       * rationalcad.pro
>       * bin
>           * rationalcad.exe

Inside of the folder into which you cloned the project, there is the `projects`
folder. This folder contains all of the source code, which is in turn organized
into sub-folders; it exists because
[qmake](http://qt-project.org/doc/qt-5.0/qtdoc/qmake-manual.html), which we use
for creating builds, requires the build folder to reside at least one level
above the project's main directory.

Momentarily, when you compile a project, a `build` folder will be created
alongside the `projects` folder, and will contain intermediate files (e.g.
.obj) used to create the final executables and libraries (e.g. .exe and .lib).
These final files are ultimately placed not in the `build` folder, but inside
of `bin` or `lib` folders beneath their respective project folders.

The `projects` folder contains `DDAD.pro` alongside several other folders such
as `mathlib` and `rationalcad`. `DDAD.pro` is the main project file, and
double-clicking should open Qt Creator, which is an IDE similar to Visual
Studio or Eclipse. Each folder also contains a project file, which is loaded by
`DDAD.pro`. You can open these individual project files directly, but I cannot
guarantee that everything will work correctly.

##### Compile and Run

Open `DDAD.pro` if you've not already; you will be prompted to specify Debug
and Release build directories. Both of these should point at `DDAD\build`, as
shown below. Press `Configure Project` to continue, then use `Build->Build All`
to compile all projects. If you run into trouble, [open an
issue](https://github.com/freemancw/DDAD/issues/new) or [send me an
email](mailto:freeman@cs.unc.edu) describing what went wrong.

![Configure Project](https://cs.unc.edu/~freeman/DDAD/images/configure_project.png) 

#### What is each Project?

| Project       | Description                                                               |
|---------------|---------------------------------------------------------------------------|
| `base`        | Original geometry library written by Dave Millman.                        |
| `mathlib`     | Extended geometry library currently under development.                    |
| `depend`      | Dependencies that aren't modified (e.g. MPIR).                            |
| `logging`     | Fork of RLog that contains various fixes.                                 |
| `misc`        | Programs that use `base`. Includes Sajal Dash's `boolop` code.            |
| `rationalcad` | Both a geometric modeler and the visualizer for DDAD.                     |
| `unittest`    | Unit tests for any of the other projects.                                 |
| `visualizer`  | Original visualization project built over Summer 2012, currently defunct. |

#### Source Documentation

#### User Documentation

#### Compiling OSG on Windows

1. Clone osg into `OpenSceneGraph/osg`
2. Create build folder `OpenSceneGraph/build`
3. Create dependency folder `OpenSceneGraph/3rdParty`
4. Download and unzip dependencies into aforementioned folder such that the
folders `bin`, `lib`, etc. are directly under `3rdParty`
5. Open CMake GUI and point it at `OpenSceneGraph/osg` for source and
`OpenSceneGraph/build` for build.
6. Press configure.
7. Change `CMAKE_PREFIX_PATH` to `C:/Program Files/Windows Kits/8.0/Lib/win8/um/x86`


