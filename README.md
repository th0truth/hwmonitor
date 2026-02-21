# <div align="center">🚀 **hwmonitor**</div>
<p align="center"><strong>A High-Performance, JSON-Native Hardware Discovery Tool for Linux</strong></p>

<div align="center">

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C Standard: C11](https://img.shields.io/badge/C-C11-blue.svg)](https://en.wikipedia.org/wiki/C11_(C_standard_revision))
[![Platform: Linux](https://img.shields.io/badge/Platform-Linux-orange.svg)](https://www.linux.org/)

</div>

---

## **🌟 Why hwmonitor?**

**hwmonitor** is built for developers who need fast, structured hardware data. Unlike traditional tools that output plain text, **hwmonitor** is designed from the ground up to provide **clean, API-ready JSON** directly from the system kernel.

- ⚡ **Near-Instant Performance:** No slow external calls to `lspci` or `dmidecode`. We read directly from `/proc` and `/sys`.
- 📊 **JSON-First Architecture:** Built-in `cJSON` integration for seamless data export and automation.
- 🛠️ **Professional C Design:** Strict C11 compliance, deep-discovery GPU logic, and memory-safe architecture.
- 📦 **Zero Dependency Bloat:** All dependencies are managed via Git submodules. Just clone and build.

---

## **🚀 Quick Start**

### **1. Installation**
Since **hwmonitor** uses high-performance submodules, ensure you clone it recursively:

```bash
# Clone the repository with all dependencies
git clone --recursive https://github.com/th0truth/hwmonitor.git 

# Enter the directory and build
cd hwmonitor
make
```

### **2. Examples**
```bash
# Get a full, structured system report in JSON
./build/hwmonitor.o --json

# Export specific CPU and GPU metrics to a file
./build/hwmonitor.o --cpu --gpu --json --output report.json

# Standard human-readable summary
./build/hwmonitor.o --cpu --ram
```

---

## **🛠️ Advanced Features**

### **Dynamic GPU Discovery**
Unlike static tools, **hwmonitor** dynamically scans your hardware bus (`/sys/class/drm`) to identify all installed GPUs. It includes vendor-specific deep-parsing for **NVIDIA**, **AMD**, and **Intel**.

### **Professional CLI Interface**
Built with `getopt_long`, providing both short and long-form flags (e.g., `-c` or `--cpu`).

| Short | Long Flag | Description |
|:---:|:---|:---|
| `-j` | `--json` | Return data in serialized JSON format |
| `-o` | `--output <f>`| Save the JSON report to a file |
| `-c` | `--cpu` | Full CPU architecture and frequency analysis |
| `-g` | `--gpu` | Advanced multi-GPU discovery and driver info |
| `-r` | `--ram` | Detailed memory and swap utilization metrics |

---

## **📊 JSON Output Preview**

The primary goal of **hwmonitor** is to produce clean, usable data. Here is what your report will look like:

```json
{
  "cpu": {
    "vendor": "AuthenticAMD",
    "model_name": "AMD Ryzen 9 7950X3D",
    "arch": "x86_64",
    "online_cores": 16,
    "max_freq_mhz": 5700,
    "curr_temp": 42.5
  },
  "gpus": [
    {
      "vendor": "NVIDIA Corporation",
      "model": "GeForce RTX 4090",
      "driver": "nvidia",
      "uuid": "GPU-8f92a1..."
    }
  ]
}
```

---

## **🏗️ Architecture & Best Practices**

This project is maintained with a focus on senior-level engineering standards:
- **Encapsulated Modules:** Each component (CPU, RAM, GPU) is isolated for maximum maintainability.
- **Doxygen Documentation:** Every function is fully documented with technical context.
- **Explicit NULL Safety:** Standardized `ptr == NULL` checks and braces for ultra-reliable performance.
- **Atomic File Writing:** Professional error handling and `stderr` reporting for automated workflows.

---

## **🤝 Contributing**

We love contributions! Whether it's adding support for new hardware (like Battery or Network) or improving the JSON schema, feel free to open a Pull Request.

---

## **⚖️ License**

Distributed under the **MIT License**. See `LICENSE` for more information.
