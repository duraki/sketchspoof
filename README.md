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

The code is easy, basically takes current time and replace `tv_sec` value from
`timeval` struct using `settimeofday`.

### 0x02 - Spoofing read-only license
Spoofing the license was way easier back in the days. But SketchApp license
spoofing is also easy, it's just a matter of permissions. See, to use Sketch
with fake license, all you have to do is replace some license information.

There is a hidden file located in AppSupport folder:

```sh
# Hidden .license file for SketchApp
$ sudo vim $HOME/Library/Application\ Support/com.bohemiancoding.sketch3/.license
```

**Edit license information & replace:**

* "sign" key value to "==" instead of Base64 chars
* "type" key value from "trial" to "license"
* "expiration" key value from whatever number to "999999999"

Save file (`:w`)

Once you made these changes, make the file readonly. For security reasons,
SketchApp generates new license with every start. To bypass this, simply change
file flags on `.license` file.

```sh
# Change file to readonly 
$ chflags uchg $HOME/Library/Application\ Support/com.bohemiancoding.sketch3/.license

# If for some reason you want to make it editable again then
$ chflags nouchg $HOME/Library/Application\ Support/com.bohemiancoding.sketch3/.license
```

Thats it. Run the SketchApp again and enjoy designing.

xxx: will add new ways if these don't work anymore, just make a new issue
