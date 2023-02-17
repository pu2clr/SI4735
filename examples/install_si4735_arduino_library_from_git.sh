# Install the latest version of Arduino CLI from github.
# Download and install the arduino command line - arduino-cli
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
# It should be installed in the ~/bin folder
export ARDUINO_LIBRARY_ENABLE_UNSAFE_INSTALL=true
curl -fsSL https://github.com/pu2clr/SI4735/archive/refs/heads/master.zip --output pu2clr_si4735_lib.zip
./bin/arduino-cli lib install --zip-path ./pu2clr_si4735_lib.zip


