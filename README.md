# Paster++

Remote console copy/paste utility.
It allows a user to paste text-based content from clipboard into a remote console. Many remote console applications does not support standard OS copy/paste.

It supports web-based and clinet based remote console application such as VMware, ILO, IDRAC, IPMI, etc. and allows the user to paste from the normal clipboard into such windows. Remote console applications does not noramlly support the normal copy/past functions. The utility overcomes this by simulating normal keystrokes.

Use-cases:
- Paste complex passwords from a password vault into to login on console.
- Transfer code/scripts into a console window where there is no IP network attached.

## Usage

1. Start the utility and klick ok.
2. Copy any text with the normal Windows copy functions. 
3. Switch to the remote console and make sure it has the focus. Normally a mouse click within the window.
4. Hit and release CTRL+ALT+v to paste the content of clipboard into the console.
5. A progress bar will be visible and indicate the past function progress.

Notice!! Do not press anything on the keyboard or move the focus from the remote console while the progress bar is visible

## Installation

Download a pre-compiled .exe version from: https://github.com/hacke78/Paster-plus-plus/releases

## How to build

The program can be built on Windows using Visual Studio or cross-compiled on Linux. 
I prefer Ubuntu 20.04 inside Windows 10 (WSL) and this is currentlly the only fully supported setup.

The build system is hard-coded to use static linking of DLL to make the executable easy portable.

### Ubuntu
Install required build tools
```
apt install cmake git mingw-w64
```
Clone the source code
```
git clone https://github.com/hacke78/Paster-plus-plus
```
Create a build directory
```
cd Paster-plus-plus
mkdir build
cd build
```
Create a Makefile and build
```
cmake ..
make
```

You will now have a single .exe file in the build directory (Paster-plus-plus.exe)

## Known limitations and issues

- The utility is hard coded to use english keyboard layout as most remote console application assumes a fixed language layout setting. Use local english keyboard layout as a workaround.
- There is a chance keystrokes while pasting is missed or changed. This is primarilly due to hove the remote console applications are implemented and that the utility is using a low-level interface to simulate keystrokes. Use an checksum calculator to validated the content such as md5sum if this is critical.

## Contributing

Pull requests are very welcome. For major changes, please open an issue first to discuss what you would like to change.

## License
[MIT](https://choosealicense.com/licenses/mit/)
