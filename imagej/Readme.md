#Digiholo2D FIJI (ImageJ) plugin#
This is an [FIJI](http://fiji.sc/Fiji) plugin which provides a graphical user interface for interaction with the command line tool. Please refer to the documentation of the command line tool in the **dist** directory for a description of the algorithms and their parameters.

Next to the GUI, a set of plugins for generating a synthetic noisy wrapped phase map are provided (see examples).

The FIJI plugin does not provide an interface to the full functionality of the command line tool (i.e batch execution and experimental algorithms). However it provides enough functionality to test the program without having to bother with the command line interface.

The plugin also works with ImageJ, however some functionality requires the [RandomJ](http://www.imagescience.org/meijering/software/randomj/) package to work. FIJI already provides this package.

##Installation (win64)##
1. Download the zip archive **digiholo2D.zip** provided in this directory. It contains a copy of the command line tool for Windows 64-bit environments.
2. Unzip the contents of the archive to your FIJI plugins directory so that you end up with [FIJI DIRECTORY]\plugins\digiholo2D. It is necessary that the folder has this exact name, since the macro expects the command line tool to be located at '[FIJI DIRECTORY]\plugins\digiholo2D\digiholo2d_v2.exe'.
3. The collection of plugins will now be accessible to you using the FIJI Plugin bar.

##Example##
After completing the installation try the following steps:

1. Generate a synthetic phase map using the 'Create synthetic phase map' Plugin. It will let you choose the size and noise level of a synthetic phase map. Just click OK for default settings.
2. Wrap the phase map using the 'wrap to pi' macro.
3. Unwrap the phase map using the 'unwrap digiholo' macro. Just click OK for an unwrap using Strand's algorithm.
4. Experiment by using different combinations of tile unwrwapping and tile merging algorithms.

For further information on the unwrapping and merging algorithms and their parameters navigate to the **dist** directory.