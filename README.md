# **hwmonitor**

A high-performance C library and CLI tool for retrieving detailed hardware information on Linux systems. **hwmonitor** provides a clean, modular way to access component data (CPU, GPU, RAM, etc.) and export it in professional JSON format.

## **Key Features**
- **JSON Support:** Built-in `cJSON` integration for easy data serialization.
- **Modern CLI:** Professional argument parsing with `getopt_long` support.
- **Zero External Dependencies:** Uses Git submodules for a "clone and build" experience.
- **Linux Native:** Directly interfaces with `/proc` and `/sys` for maximum speed.

---

## **Installation**

Since **hwmonitor** uses Git submodules for its dependencies, ensure you clone it recursively:

```bash
# Clone the repository with all dependencies
git clone --recursive https://github.com/th0truth/hwmonitor.git 

# Enter the directory and build
cd hwmonitor
make
```

---

## **Usage**

**hwmonitor** supports both short and long-form flags for flexible system monitoring.

### **Basic Examples**
```bash
# Show CPU information in plain text
./build/hwmonitor.o --cpu

# Generate a full system report in formatted JSON
./build/hwmonitor.o --json

# Export specific hardware data to a file
./build/hwmonitor.o --cpu --gpu --json --output report.json
```

### **Available Options**
| Short | Long | Description |
|:---:|:---|:---|
| `-h` | `--help` | Show this help message |
| `-c` | `--cpu` | Display CPU architecture, model, and frequency |
| `-g` | `--gpu` | Display GPU vendor and model information |
| `-j` | `--json` | Output data in JSON format |
| `-o` | `--output <file>` | Save the JSON report to a specific file |

---

## **JSON Output Example**
When using the `--json` flag, **hwmonitor** produces a structured report. Here is an example of a full system analysis:

```json
{
  "cpu": {
    "vendor": "AuthenticAMD",
    "model_name": "AMD Ryzen 9 7950X3D 16-Core Processor",
    "arch": "x86_64",
    "max_freq_mhz": 5700,
    "total_cores": 16,
    "total_threads": 32,
    "curr_temp": 54.2
  },
  "gpu": {
    "vendor": "NVIDIA Corporation",
    "model": "GeForce RTX 4080",
    "driver": "535.113.01",
    "vram_mb": 16384
  },
  "memory": {
    "total_gb": 64,
    "used_gb": 12.4,
    "type": "DDR5"
  }
}
```

---

## **Supported Components**

| Component | Information | Linux | Windows |
|:---|:---:|:---:|:---:|
| **CPU** | Architecture, Vendor, Model, Cores, Frequency, Temp | ✔️ | ❌ |
| **GPU** | Vendor, Device, Driver, PCI ID | ✔️ | ❌ |
| **MEM** | Total, Free, Available, Cached, Swap | ✔️ | ❌ |
| **BAT** | Capacity, Voltage, Health, Manufacturer | ✔️ | ❌ |
| **OS** | Distro, Version, Desktop Environment | ✔️ | ❌ |
| **BIOS** | Vendor, Version, Release Date | ✔️ | ❌ |

---

## **Development**

### **Architecture**
- **`include/`**: Clean header files with `#pragma once` guards.
- **`src/`**: Modular implementation (one `.c` file per component).
- **`deps/`**: External libraries managed via Git Submodules.

### **Coding Standards**
This project follows modern C standards (C11+). Variables are declared near first use to reduce cognitive load and improve performance.

## **License**
This project is licensed under the [MIT License](LICENSE).
