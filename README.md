# Paster++

Remote console copy/paste utility.
It allows the user to paste the text-based content from clipboard into a remote console.

It supports web-based and clinet based remote console application such as VMware, ILO, IDRAC, IPMI, etc. and allows the user to paste from the normal clipboard into such windows. Remote console applications does not noramlly support the normal copy/past functions. This utulity overcomes this by simulating normal keystrokes.

## Usage

1. Start the utulity and klick ok.
2. Copy any text with the normal Windows copy functions. 
3. Switch to the remote console and make sure it has the focus. Normally a mouse click within the window.
4. Hit and release CTRL+ALT+v to paste the content of clipboard into the console.
5. A progress bar will be visible and indicate the past function progress.

Notice!! Do not press anything on the keyboard or move the focus from the remote console while the progress bar is visible

## Installation

Download a pre-compiled .exe version from: https://fob.bar/keypaster++.exe

## Known issues

- The utility is hard coded to use english keyboard layout as most remote console application assumes a fixed language layout setting.
- There is a chance keystrokes while pasting is missed or changed. This is primarilly due to hove the remote console applications are implemented and that the utuility is using a low-leve interface to simulate keystrokes.

## Contributing

Pull requests are very welcome. For major changes, please open an issue first to discuss what you would like to change.
This utility


## License
[MIT](https://choosealicense.com/licenses/mit/)
