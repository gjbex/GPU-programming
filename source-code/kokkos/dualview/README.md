# DualView

`Kokkos::DualView` is a Kokkos container that provides a view of data that can
be shared between host and device memory spaces. It is a container that is
similar to `Kokkos::View` but with the added feature that it can be
synchronized between host and device memory spaces. This is useful when you
want to share data between the host and device memory spaces, and you want to
avoid the overhead of copying data between the two memory spaces.

It is mainly intended to help existing code being ported to Kokkos, where 
it is not clear whether the data should reside on the host or device memory.

Note that it is the programmer's responsibility to ensure that the data is
is marked as modified on the correct memory space, and syncing it to the
other memory space when needed.


## What is it?

1. `dual_view.cpp`: C++ code that demonstrates the use of `Kokkos::DualView`.
1. `CMakeLists.txt`: CMake file to build the applications.
