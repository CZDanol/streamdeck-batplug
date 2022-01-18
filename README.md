# Stream Deck Bat Plugin Core
This is a plugn core for Elgato Stream Deck that allows easy and fast plugin development without need to compile anything.

It provides an exe file that simply launches a file in the `cmd` folder with the name of the action pressed. The file can be a `.bat` file, `.exe` file or anything.
Additionally, it provides a few basic commands that can be executed from the launched application using through `stdout`.

## How to make a plugin
1. See the official Elgato documentation [here](https://developer.elgato.com/documentation/stream-deck/sdk/plugin-architecture/).
2. Download a release of this plugin core, copy the `bin` directory into your `xx.sdPlugin` plugin folder.
3. Create your own `manifest.json` file and other stuff.
   1. Set `CodePathWin` to `bin/streamdeck-batplug.exe`.
   2. You can see the `manifest_base.json` in this repo for you to start off.
4. Create a `cmd` directory in your `.sdPlugin` folder.
   1. Create a `bat`/`executable` file for each action in the manifest file.
   2. For example if your action is `cz.danol.example.button1`, create a `cz.danol.example1.button.bat` batch file in there with the functionality you want.
5. Release the plugin, don't forget to put Danol and Krabs into the credits!
6. Profit.

The program writes to `log.txt` in the `bin` folder for debugging purposes. If the `log.txt` file doesn't exist, the logging is disabled. 

## Supported commands
The plugin core reads `stdout` of the launched commands and reads commands from that.
* A command always starts on a new line, trailing and leading withespaces are trimmed.
* First word on the line is interpreted as a command, the rest of the line after the space is considered to be an argument.
* `\n` in the text is interpreted as newline.

If you're writing a bat file, you fan for example do
`echo setTitle This is a new title.`

#### `setTitle Title text`
Sets the title of the button to a spefixied string.

#### `setState ix`
Sets the state of the button to a given value (integer).

#### `switchProfile profileName`
Switches the stream deck the button is on to a given profile (same limitations as in the stream deck plugin api appy - the profile has to be added by the plugin itself).

#### `showAlert`
Shows an alert icon over the button.

## Action settings
* The plugin core passes all action settings as `--key value` argument pairs into the launched application.
* Additionally, the settings are also passed as environment variables prefixed with `BPS_`.

So for example if an action has a setting `errorMessage: "This is an error!"`, it is passed to the program as `-errorMessage "This is an error"` and the environment variable `BPS_errorMessage` has the value of the setting stored.

## Authors
* Idea by Krabs
* Coded by Danol