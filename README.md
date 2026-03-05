CMake 3.23+ and Ruby 3+ are required to build the project.

This is a C++ extension to DragonRuby. Currently, only the physics bindings
are functional, but more are currently underway. 

You should only need to pass -DEULER_DRAGONRUBY_PATH=... to CMake to build the
project. The other options are for features in current development that do not
work yet.

In the near future, I am working on adding math, networking, and GUI libraries.
