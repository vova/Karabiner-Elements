[![Build Status](https://travis-ci.org/tekezo/Karabiner-Elements.svg?branch=master)](https://travis-ci.org/tekezo/Karabiner-Elements)
[![License](https://img.shields.io/badge/license-Public%20Domain-blue.svg)](https://github.com/tekezo/Karabiner-Elements/blob/master/LICENSE.md)

# Karabiner-Elements

Karabiner-Elements is the subset of the next generation Karabiner for macOS Sierra.

## Project Status

Karabiner-Elements works fine.

You can download the latest Karabiner-Elements from https://pqrs.org/latest/karabiner-elements-latest.dmg

## System requirements

* OS X 10.11.*
* OS X 10.12.*

# Usage

Detailed usage instructions are available [here](usage/README.md).

## Features

* Simple key modification (change keys to another keys)
* Support Secure Keyboard Entry (eg. Terminal, Password prompt)
* Modifier flags sharing with all connected keyboards.

## Limitations

* Karabiner-Elements cannot modify eject key due to the limitation of macOS API.
* Karabiner-Elements ignores the `System Preferences > Keyboard > Modifier Keys...` configuration.

## How to build

System requirements:

* OS X 10.11+
* Xcode 8+
* Command Line Tools for Xcode
* Boost 1.61.0+ (header-only) http://www.boost.org/

To install the Boost requirement, [download the latest Boost release](http://www.boost.org/), open the `boost` folder inside of it, and move all of the files there into `/opt/local/include/boost/`.

(For example, the version.hpp should be located in `/opt/local/include/boost/version.hpp`)


### Step 1: Getting source code

Clone the source from github.

```
git clone --depth 1 https://github.com/tekezo/Karabiner-Elements.git
```

### Step 2: Building a package

```
cd Karabiner-Elements
make
```

The `make` script will create a redistributable **Karabiner-Elements-VERSION.dmg** in the current directory.

# Donations

If you would like to contribute financially to the development of Karabiner Elements, donations can be made via https://pqrs.org/osx/karabiner/donation.html.en.
