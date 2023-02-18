# Installs the latest version of the PU2CLR SI4735 Arduino Library from github.
# Unlike a release (installed from Arduino IDE) this method installs the current situation
# of the PU2CLR SI4535 Arduino Library (latest modifications but not yet released).  
# Download and install the arduino command line - arduino-cli
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
# It should be installed in the ./bin folder
export ARDUINO_LIBRARY_ENABLE_UNSAFE_INSTALL=true
./bin/arduino-cli lib install --git-url https://github.com/pu2clr/SI4735


