This Package contains native samples for Blue Cat's plug'n Script (http://www.bluecataudio.com/Products/Product_PlugNScript). They are distributed in binary form with the software.

Layout:
- build: contains the Visual Studio or Xcode projects to build the samples on your platform.
- src: the source files for the samples (one cpp file for each sample), and the reference for c and cpp implementation (containing all variable and functions definitions that can be used to build native audio processors).
- include: the includes files that contain the definitions of the audio dsp api, and C and C++ utilities ("helpers") to simplify dsp script writing.

Getting Started:
Open the project on your favorite platform and build the samples. You can then create your own project, starting with one of the samples projects (you can make a copy and start from there). The project called "YourScript" can be used to start a script from scratch.

When you need to add new functions or variables from the API, you can open the script_reference.cpp or script_reference.c files to copy and paste from there into your own project.