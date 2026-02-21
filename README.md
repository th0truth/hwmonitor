# hwmonitor

A minimalist, high-performance hardware discovery engine for Linux systems.

**hwmonitor** is designed for developers and system administrators who require structured, low-latency hardware data. Unlike traditional tools that rely on external shell calls (like `lspci` or `dmidecode`), **hwmonitor** interfaces directly with the Linux kernel via `sysfs` and `procfs`, providing a JSON-native output ready for modern automation and monitoring stacks.

---

## Core Features

*   **Native Performance:** Directly parses kernel filesystems for near-instant execution.
*   **JSON-First Architecture:** Built-in serialization for seamless integration with APIs and web dashboards.
*   **Deep Hardware Discovery:** Advanced logic for multi-GPU identification (NVIDIA, AMD, Intel) and battery metrics.
*   **Modern C Design:** Strict C11 compliance with a focus on memory safety and modularity.
*   **Zero Dependencies:** Managed via Git submodules for a lightweight, "clone-and-build" workflow.

---

## Installation

The project uses Git submodules for its core libraries. Ensure you perform a recursive clone to include all necessary dependencies.

```bash
git clone --recursive https://github.com/th0truth/hwmonitor.git
cd hwmonitor
make
```

---

## Usage

**hwmonitor** supports a flexible command-line interface with both short and long-form arguments.

### Examples

```bash
# Generate a full system report in structured JSON
./build/hwmonitor.o --json

# Export specific CPU and GPU metrics to a file
./build/hwmonitor.o --cpu --gpu --json --output report.json

# Display a standard human-readable summary
./build/hwmonitor.o --cpu --ram --battery
```

### Options

| Flag | Argument | Description |
| :--- | :--- | :--- |
| `-j, --json` | None | Serializes hardware data into a JSON object. |
| `-o, --output` | `<file>` | Redirects the JSON output to a specified file. |
| `-c, --cpu` | None | Collects architecture, model, and real-time frequency data. |
| `-g, --gpu` | None | Performs dynamic bus scanning for all installed GPUs. |
| `-r, --ram` | None | Reports detailed memory utilization and swap metrics. |
| `-b, --battery` | None | Monitors capacity, voltage, and health of system batteries. |

---

## Data Schema

The tool produces a highly structured schema. Below is a representative output of a full system analysis:

```json
{
  "cpu": {
    "vendor": "AuthenticAMD",
    "model_name": "AMD Ryzen 9 7950X",
    "arch": "x86_64",
    "online_cores": 16,
    "max_freq_mhz": 5700
  },
  "gpus": [
    {
      "vendor": "NVIDIA Corporation",
      "model": "GeForce RTX 4080",
      "driver": "nvidia",
      "uuid": "GPU-8f92a1..."
    }
  ],
  "battery": {
    "status": "Discharging",
    "capacity_percent": 84,
    "model_name": "Standard-L1",
    "manufacturer": "SAMSUNG"
  }
}
```

---

## Technical Architecture

The codebase is engineered with a focus on modularity and transparency.

*   **Discovery Engine:** Implements a dynamic directory-scanning pattern to identify hardware nodes without hardcoded limits.
*   **Memory Management:** Strictly follows an "inside-out" deep-freeing pattern to ensure a zero-leak footprint.
*   **Explicit Safety:** Employs explicit NULL checks and Doxygen-style documentation across all modules to ensure reliability in production environments.

## License

This project is licensed under the **MIT License**. See the `LICENSE` file for details.
