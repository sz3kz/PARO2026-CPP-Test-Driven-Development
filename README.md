# Cash Register
Cash Register Functionality Implementation via Test Driven Development.

## Usage

1. Clone the repository
```bash
git clone git@github.com:sz3kz/tdd-dummy.git
cd tdd-dummy/workspace
# direnv allow                              # NixOS - activate environment (cmake, clang, make)
```
2. Depending how you want to use it:
#### Raw
If you have everything installed already and don't really care about versions of compiler etc. then:
```bash
# pre-commit install                        # Set up clang-format and cppcheck hook via .yaml
# pre-commit install --hook-type pre-push   # Set up clang-tidy hook via .yaml
cmake -S . -B build/ -G "Ninja"
cd build/
ninja run_tests
```
### Docker/Podman
**UNTESTED**
If you want a different (for example: never) compiler or don't want to install additional software on your system beyond Podman or Docker

1. Get inside repo
> cd MyAwesomeProject
2. Get the gcc image with cmake and ninja ready:
> podman build . -t=gcc_latest
3. Start interactive shell in the container and mount the workspace. Build folder will live and die with the container
> podman run --mount type=bind,source=`pwd`/workspace,destination=/workspace -p 2222:2222 --rm -it gcc_latest
4. Run Cmake
> cmake -G"Ninja" /workspace

### Compilation + tests
To compile and run the tests just
> make run_tests

or

> ninja run_tests

depending which generator was used earlier.

Initially there should be a single test failing, named testFailedSuccesfully

You can remove `src/sampleTests.cpp` form `sample_tests` binary definition in `workspace/CMakeLists.txt` to get rid of unnecessary noise.

Happy coding!
