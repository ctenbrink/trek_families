trek_families
=============

Project for separating trek participants into groups.

Prereqs: The project uses QT 4.8.0, so you will need to have that installed. Also, you will need the lib-yaml yaml parsing libraries. You'll need ruby and rake installed, as well as cmake.

To build the project, type 'rake cmake build'.

The project was originally developed on the Mac, but it should work on Windows as well. That said, I have never tried to build it on Windows.

You can use src/convert_data.rb to convert participant info found in XLS spreadsheets to the YAML format required by the program. For privacy reasons, I haven't included any of the participant data in the repo.
