# Contributing to GPU Canny Edge Detector

Thank you for your interest in contributing to this project! This document provides guidelines and instructions for contributing.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [Making Changes](#making-changes)
- [Coding Standards](#coding-standards)
- [Testing](#testing)
- [Submitting Changes](#submitting-changes)
- [Reporting Issues](#reporting-issues)

## Code of Conduct

### Our Pledge

We are committed to providing a welcoming and inclusive environment for all contributors.

### Expected Behavior

- Be respectful and considerate
- Welcome newcomers and help them get started
- Focus on constructive feedback
- Accept responsibility for mistakes
- Prioritize community benefit

### Unacceptable Behavior

- Harassment or discrimination
- Trolling or inflammatory comments
- Personal attacks
- Publishing others' private information
- Other unprofessional conduct

## Getting Started

### Prerequisites

Before contributing, ensure you have:

1. **Development Environment**:
   - C++17 compatible compiler
   - CMake 3.18+
   - Git

2. **Dependencies**:
   - OpenCV 4.x with CUDA support
   - CUDA Toolkit 10.2+
   - NVIDIA GPU (for testing GPU features)

3. **Knowledge**:
   - C++ programming
   - Basic CMake understanding
   - Git workflow
   - (Optional) CUDA programming for GPU features

### Fork and Clone

1. Fork the repository on GitHub
2. Clone your fork:
   ```bash
   git clone https://github.com/YOUR_USERNAME/canny_edge_detector.git
   cd canny_edge_detector
   ```
3. Add upstream remote:
   ```bash
   git remote add upstream https://github.com/bmbayad/canny_edge_detector.git
   ```

## Development Setup

### Building for Development

```bash
# Create debug build
mkdir build-debug
cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
```

### Running Tests

```bash
# Run quick test
./test.sh

# Run with your test images
./build/bin/canny_edge_detector -i path/to/test/image.jpg
```

### Development Tools

#### Recommended IDEs
- Visual Studio Code with C++ extensions
- CLion
- Visual Studio 2019+

#### Useful Extensions (VSCode)
- C/C++ (Microsoft)
- CMake Tools
- GitLens
- Markdown All in One

## Making Changes

### Branching Strategy

1. Create a feature branch:
   ```bash
   git checkout -b feature/your-feature-name
   ```

2. Branch naming conventions:
   - `feature/description` - New features
   - `bugfix/description` - Bug fixes
   - `docs/description` - Documentation updates
   - `refactor/description` - Code refactoring
   - `perf/description` - Performance improvements

### Keeping Your Fork Updated

```bash
# Fetch upstream changes
git fetch upstream

# Merge upstream main into your branch
git checkout main
git merge upstream/main

# Update your feature branch
git checkout feature/your-feature-name
git rebase main
```

## Coding Standards

### C++ Style Guide

#### File Organization

```cpp
// 1. Include guard
#ifndef MODULE_NAME_HPP
#define MODULE_NAME_HPP

// 2. System includes
#include <iostream>
#include <vector>

// 3. Third-party includes
#include <opencv2/opencv.hpp>

// 4. Project includes
#include "other_module.hpp"

// 5. Declarations
class MyClass {
    // Public interface first
public:
    // Constructor/Destructor
    MyClass();
    ~MyClass();
    
    // Public methods
    void doSomething();
    
    // Private implementation
private:
    // Private methods
    void helperMethod();
    
    // Private members
    int member_variable_;
};

#endif // MODULE_NAME_HPP
```

#### Naming Conventions

```cpp
// Classes: PascalCase
class CannyProcessor { };

// Functions/Methods: camelCase
void processImage() { }

// Variables: snake_case
int image_width = 0;

// Constants: UPPER_SNAKE_CASE
const int MAX_ITERATIONS = 100;

// Private members: trailing underscore
class Example {
private:
    int private_member_;
};

// Namespaces: lowercase
namespace edge_detection { }
```

#### Code Formatting

```cpp
// Indentation: 4 spaces (no tabs)
void exampleFunction() {
    if (condition) {
        // Code here
    }
}

// Braces: K&R style
if (condition) {
    statement;
} else {
    other_statement;
}

// Line length: Max 100 characters
// Break long lines at logical points
void functionWithManyParameters(
    int first_param,
    double second_param,
    const std::string& third_param) {
    // Implementation
}

// Pointer/Reference alignment
int* pointer;
int& reference;

// Spacing
int x = 5;              // Spaces around operators
array[index];           // No space before bracket
function(arg1, arg2);   // Space after comma
```

#### Comments

```cpp
/**
 * Class/Function documentation
 * @param input Description of parameter
 * @return Description of return value
 */

// Single-line comments for brief explanations
int value = 42;  // Inline comment

/* Multi-line comments
 * for longer explanations
 * when needed
 */

// TODO: Future improvements
// FIXME: Known issues to fix
// NOTE: Important notes
```

### CMake Style

```cmake
# Use lowercase for commands
find_package(OpenCV REQUIRED)

# Descriptive variable names
set(PROJECT_SOURCES
    src/main.cpp
    src/module.cpp
)

# Proper indentation
if(CONDITION)
    message(STATUS "Message")
endif()
```

## Testing

### Manual Testing

Before submitting:

1. **Build Testing**:
   ```bash
   # Clean build
   rm -rf build
   mkdir build && cd build
   cmake .. && make
   ```

2. **Functionality Testing**:
   ```bash
   # Test basic functionality
   ./bin/canny_edge_detector -i test_image.jpg
   
   # Test different parameters
   ./bin/canny_edge_detector -i test_image.jpg -t1 50 -t2 150
   
   # Test configuration file
   ./bin/canny_edge_detector -c config.txt
   ```

3. **Edge Cases**:
   - Non-existent input files
   - Invalid parameters
   - Very large images
   - Very small images
   - Grayscale vs color images

### Performance Testing

```bash
# Run benchmarks with different image sizes
for size in 512 1024 2048 4096; do
    ./bin/canny_edge_detector -i test_${size}.jpg
done
```

### Memory Testing (Optional)

```bash
# Check for memory leaks with valgrind
valgrind --leak-check=full ./bin/canny_edge_detector -i test.jpg
```

## Submitting Changes

### Commit Guidelines

#### Commit Message Format

```
<type>: <subject>

<body>

<footer>
```

#### Types
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting)
- `refactor`: Code refactoring
- `perf`: Performance improvements
- `test`: Adding tests
- `chore`: Build/tooling changes

#### Examples

```
feat: Add support for batch processing multiple images

Implement batch processing mode that can process multiple
images in parallel using GPU streams.

Closes #42
```

```
fix: Correct memory leak in GPU processing

Fixed issue where GPU memory was not properly released
after processing large images.

Fixes #15
```

```
docs: Update README with Windows build instructions

Added detailed instructions for building on Windows with
Visual Studio 2019 and later versions.
```

#### Commit Best Practices

- Use present tense ("Add feature" not "Added feature")
- Use imperative mood ("Move cursor" not "Moves cursor")
- First line: concise summary (50 chars or less)
- Body: detailed explanation if needed (wrap at 72 chars)
- Reference issues and PRs

### Pull Request Process

1. **Update Your Branch**:
   ```bash
   git fetch upstream
   git rebase upstream/main
   ```

2. **Run Tests**:
   ```bash
   ./build.sh
   ./test.sh
   ```

3. **Push Changes**:
   ```bash
   git push origin feature/your-feature-name
   ```

4. **Create Pull Request**:
   - Go to GitHub repository
   - Click "New Pull Request"
   - Select your feature branch
   - Fill out PR template

5. **PR Description Template**:
   ```markdown
   ## Description
   Brief description of changes
   
   ## Type of Change
   - [ ] Bug fix
   - [ ] New feature
   - [ ] Documentation update
   - [ ] Performance improvement
   
   ## Testing
   - [ ] Built successfully
   - [ ] Tested with sample images
   - [ ] No memory leaks
   - [ ] Benchmarks show improvement (if perf change)
   
   ## Screenshots (if UI changes)
   
   ## Related Issues
   Fixes #issue_number
   ```

6. **Code Review**:
   - Address reviewer comments
   - Make requested changes
   - Push updates to the same branch

### PR Checklist

Before submitting, ensure:

- [ ] Code follows style guidelines
- [ ] Comments added for complex logic
- [ ] Documentation updated
- [ ] Builds without errors or warnings
- [ ] Tested on target platform
- [ ] No unnecessary files included
- [ ] Commit messages are clear
- [ ] PR description is complete

## Reporting Issues

### Bug Reports

Use this template:

```markdown
**Describe the bug**
A clear description of what the bug is.

**To Reproduce**
Steps to reproduce:
1. Run command '...'
2. With parameters '...'
3. See error

**Expected behavior**
What you expected to happen.

**Actual behavior**
What actually happened.

**Environment:**
- OS: [e.g., Ubuntu 20.04]
- OpenCV version: [e.g., 4.5.5]
- CUDA version: [e.g., 11.4]
- GPU: [e.g., RTX 3080]
- Compiler: [e.g., GCC 9.3]

**Additional context**
Any other relevant information.

**Screenshots/Logs**
If applicable, add screenshots or error logs.
```

### Feature Requests

Use this template:

```markdown
**Feature description**
Clear description of the proposed feature.

**Use case**
Why is this feature needed? What problem does it solve?

**Proposed solution**
How do you envision this working?

**Alternatives considered**
What other approaches did you consider?

**Additional context**
Any other relevant information.
```

## Areas for Contribution

### Good First Issues

- Documentation improvements
- Example scripts
- Error message improvements
- Code comments
- Test image generation scripts

### Medium Difficulty

- Configuration format enhancements
- Additional quality metrics
- Performance optimizations
- Build system improvements
- Cross-platform compatibility

### Advanced

- Additional GPU backends (OpenCL, Vulkan)
- Custom CUDA kernels
- Multi-GPU support
- Video processing support
- Python bindings

## Community

### Getting Help

- **Questions**: Open a GitHub discussion
- **Bugs**: Open a GitHub issue
- **Feature ideas**: Open a GitHub issue with [Feature Request] tag

### Recognition

Contributors will be:
- Listed in AUTHORS file
- Credited in release notes
- Thanked in documentation

## License

By contributing, you agree that your contributions will be licensed under the same license as the project (MIT License).

## Thank You!

Your contributions make this project better. We appreciate your time and effort! 🎉
