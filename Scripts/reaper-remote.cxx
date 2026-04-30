/** A remote control for Reaper.
* https://www.bluecataudio.com/Blog/tip-of-the-day/free-remote-control-plug-in-app-for-reaper
* Reaper remote does nothing in the DSP (remote control commands are in the user interface).
* Only declaring the Reaper root URL string so that it is stored with the plug-in in the session.
*/
string name = "Reaper Remote";
array<string> inputStringsNames = { "Reaper URL" };
array<string> inputStrings(inputStringsNames.length);