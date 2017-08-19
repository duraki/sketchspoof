## Sketchspoof
Several ways to crack and use MacOS Sketch App. Yes, Sketch is awesome, and I
think you should respect the developer and pay for the software. On the other
hand, I'm writing this for educational purposes, so I'm not responsible for
anything you do with this.

### 0x01 - Spoofing the local time
This is easy. All you have to do is compile `SketchSpoof` library I wrote
earlier today. It's a C++ code that replaces current local time to sometimes in
2016, and while injecting this as a linked library, the time Sketch is
seeing is not the real one.

This can be used to fake your time and Sketch trial will continue working.
Although this works, you'll have to inject this library at runtime. All code is
located in `src/` directory.

```sh
# Compile the lib
$ g++ -shared sketchspoof.cpp -o SketchSpoof.dylib

# Link and load lib at runtime
$ DYLD_INSERT_LIBRARIES=SketchSpoof.dylib /Applications/Sketch.app/Contents/MacOS/Sketch

# Sketch will open and trial will not be expired, enjoy.
```

### 0x02 - Spoofing read-only license

