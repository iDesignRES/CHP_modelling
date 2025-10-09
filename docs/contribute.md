# Contribute to CHP_modelling

## Overview

Contributing to `CHP_modelling` can be done in several ways, including reporting issues, requesting features, or developing new functionality.

## File a Bug Report

If you encounter unexpected behaviour, please file an issue at [GitHub Issues](https://github.com/iDesignRES/CHP_modelling/issues/new).

When reporting a bug:

1. Confirm the issue is specific to `CHP_modelling`.
2. Label the issue as a bug.
3. Provide a minimal working example.

> **Note:** Reports of ambiguities are welcome and help improve the model.

## Feature Requests

If you need additional features, you can request them by:

1. Creating an issue describing the feature.
2. Implementing the feature in a fork of the repository.

### Creating an Issue

To request a feature, open an [issue](https://github.com/iDesignRES/CHP_modelling/issues/new) and include:

1. **Reason:** Why is the feature needed?
2. **Outcome:** What is the desired result and minimum requirements?
3. **Solutions:** Suggest alternatives if possible.

### Incorporate Feature Requests Through a Fork

If you wish to contribute code directly, follow these steps:

#### Step 1: Create an Issue

Before contributing code, create an issue as described above. This allows maintainers to review and discuss your proposal.

#### Step 2: Fork the Repository

Fork `CHP_modelling` and work within your fork.

#### Step 3: Checkout a New Branch

Develop new features or fixes on a separate branch.

#### Step 4: Make Changes

Implement your changes, comment your code, and add relevant tests. Ensure all existing tests pass and update/add tests for new features.

> **Note:** Coding Style  
> `CHP_modelling` generally follows the [Google style](https://google.github.io/styleguide/cppguide.html) with minor modifications.

To automatically format your code, you can install clang-format:

```bash
sudo apt install clang-format
```

and then run the following in the main folder

```bash
find . -regex '.*\.\(cpp\|hpp\|cc\|cxx\|h\)' -exec clang-format -i {} \;
```

#### Step 5: Debugging (reccomended)

Debug the updated code by firstly installing

```bash
conan install .. --output-folder=. --build=missing -s build_type=Debug -s compiler.cppstd=17 -s arch=x86_64
```

before you compile and build the project using

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_VERBOSE_MAKEFILE=ON -DCMAKE_TOOLCHAIN_FILE="${PWD}/conan_toolchain.cmake"
cmake --build . --config Debug
```

which then enables testing through (look for warnings to improve your contribution)

```bash
ctest -C Debug
```

#### Step 6: Tests coverage (reccomended if altered)

To check coverage of the tests one can install lcov

```bash
sudo apt install lcov
```

The above debug compilation option can be extended with the `-DCMAKE_CXX_FLAGS="--coverage"` flag

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_VERBOSE_MAKEFILE=ON -DCMAKE_TOOLCHAIN_FILE="${PWD}/conan_toolchain.cmake" -DCMAKE_CXX_FLAGS="--coverage"
cmake --build . --config Debug
ctest -C Debug
```

One can then generate html report of the coverage with

```bash
lcov --capture --directory . --output-file coverage.info
lcov --remove coverage.info '/usr/*' '**/.conan2/*' --output-file coverage.info
genhtml coverage.info --output-directory coverage_report
```

The newly generated file `build/coverage_report/index.html` can then be opened in any browser.

#### Step 7: Build documentation (reccomended if altered)

The documentation is generated using [Doxygen](https://doxygen.nl/) which can be installed with

```bash
sudo apt-get install doxygen
```

You can build the updated documentation from the main folder with

```bash
doxygen docs/Doxyfile
```

#### Step 8: Create a Pull Request

When ready, submit a pull request to the main branch. Maintainers will review and assign reviewers as needed.

You may receive feedback on your changes. Please be patient during the review process.