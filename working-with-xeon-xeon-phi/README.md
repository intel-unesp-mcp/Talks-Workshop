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

# Compile and running for Intel Xeon offloading to Xeon Phi

```
icc helloWorldOffload.c -o helloWorldOffload
./helloWorldOffload
```
# Function offloading

```
icc offloadFunction.c -o offloadFunction
./offloadFunction
```

# Function offloading

```
icc offloadFunction2.c -o offloadFunction2
./offloadFunction2
```
