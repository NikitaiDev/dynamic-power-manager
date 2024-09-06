# Dynamic Power Manager for Peripheral Devices in Ubuntu

## Overview
This project implements a dynamic power manager that monitors the activity of peripheral devices (such as USB) in an Ubuntu environment and adjusts power usage based on the selected profile. The daemon can be used to optimize energy consumption by automatically disabling unused devices or balancing performance.

## Features
- Monitor USB device activity.
- Automatically disable inactive devices.
- Three power profiles: Maximum Power Saving, Balanced, and Performance.
  
## How to Install
1. Clone the repository:
    ```bash
    git clone https://github.com/your-username/dynamic-power-manager.git
    cd dynamic-power-manager
    ```

2. Build and install the project:
    ```bash
    make
    sudo make install
    ```

3. To uninstall:
    ```bash
    sudo make uninstall
    ```

## Power Profiles
- **Maximum Power Saving**: Disables all unused devices for maximum energy saving.
- **Balanced**: Balances between power saving and performance.
- **Performance**: Keeps all devices powered for maximum performance.

## How to Run
After installation, you can run the dynamic power manager by executing:
```bash
dynamic-power-manager
