This folder contains sample projects that you can build

## space_invaders

Build the project by using any of these commands:
```
scons --project=space_invaders

```
## x86_freertos

This is the FreeRTOS "simulator" on your host machine. For example, you can run a program with multiple FreeRTOS tasks on your Mac or Linux machine (or maybe even windows?)

Use this project to:

* Learn FreeRTOS API
* Experiment with multiple FreeRTOS tasks or queues


```
# Compile
scons --project=x86_freertos

# Run the executable
_build_x86_freertos/./x86_freertos.exe
```

## x86_sandbox

This is to compile a program on your host machine. For example, you can compile an executable that runs on your Mac or Linux machine (or maybe even windows?)

```
# Compile
scons --project=x86_sandbox

# Run the executable
_build_x86_sandbox/./x86_sandbox.exe
```

Use this project to:

* Compile a program for your host machine
* Run unit-tests for code modules
