# Intel Xeon and Intel Xeon Phi

# Compile and running for Intel Xeon

```
icc helloWorld.c -o helloWorld
./helloWorld
```

# Compile and running for Intel Xeon Phi

```
icc helloWorld.c -o helloWorld.mic -mmic
micnativeloadex helloWorld.mic
```

# Compile and running for Intel Xeon offloaing to Xeon Phi

```
icc helloWorldOffload.c -o helloWorldOffload
./helloWorldOffload
```
