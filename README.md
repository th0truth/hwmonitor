<p align="center">
  <img src=".github/assets/logo.png" alt="hwmonitor logo" width="200">
</p>

# hwmonitor

> A minimalist, high-performance hardware discovery engine for Linux systems.

**hwmonitor** is a lightweight, low-dependency (C11) command-line utility designed for developers and system administrators who require structured, low-latency hardware telemetry. Rather than relying on expensive external shell calls (`lspci`, `dmidecode`, or `lshw`), **hwmonitor** interfaces directly with the Linux kernel via `/sys` and `/proc` filesystems.

It provides both beautiful, human-readable terminal output and highly structured JSON for modern monitoring stacks and API integrations.

---

## ✨ Key Features

* **Native Performance:** Written in pure C. Direct kernel filesystem parsing ensures near-instant execution times.
* **🕒 Live Watch Mode:** Real-time hardware monitoring with sub-second refreshes using `--watch`.
* **🎨 TrueColor Themes:** Rich 24-bit color support with multiple presets (`cyberpunk`, `forest`, `sunset`, etc.).
* **🤖 AI Hardware Consultant:** Built-in integration with the Groq API allows you to ask natural language questions about your hardware (e.g., bottlenecks, linux driver compatibility) directly from the terminal.
* **JSON-First Architecture:** Built-in serialization for seamless integration into dashboards, observability pipelines, or scripts.
* **Comprehensive Hardware Discovery:** Advanced logic for CPU, RAM, Multi-GPU configurations, Storage (block devices), Mainboard (DMI/SMBIOS), Battery, and OS environments.
* **Zero Bloat:** No Python, no `dbus`. Uses `libcurl` for AI network requests and a single submodule ([cJSON](https://github.com/DaveGamble/cJSON)) for reliable JSON generation.
* **Memory Safe:** Engineered with an "inside-out" deep-freeing pattern to ensure a zero-leak footprint.

---

## 🚀 Installation

The project uses Git submodules and requires `libcurl` for AI integrations.

### 1. Install Dependencies
* **Debian / Ubuntu:** `sudo apt install build-essential libcurl4-openssl-dev`
* **Arch Linux:** `sudo pacman -S base-devel curl`
* **Fedora / RHEL:** `sudo dnf install gcc make libcurl-devel`

### 2. Build and Install
Ensure you perform a recursive clone to include all necessary dependencies.

```bash
git clone --recursive https://github.com/th0truth/hwmonitor.git
cd hwmonitor
make
sudo make install
```

---

## 🛠️ Usage

### 🕒 Real-Time Monitoring
```bash
# Watch your CPU and RAM usage live with the Cyberpunk theme
hwmonitor --cpu --ram --watch --theme cyberpunk
```

### 🤖 AI Hardware Consultant
```bash
# Set your API token
export GROQ_API_KEY="gsk_your_api_token"

# Analyze specific hardware
hwmonitor --gpu -A "What is the best open-source driver branch for this GPU on Wayland?"
```

### Available Flags

| Flag | Long Flag | Description |
| :--- | :--- | :--- |
| `-a` | `--all` | Explicitly targets all hardware modules. |
| `-b` | `--battery` | Monitors capacity, voltage, and health of system batteries. |
| `-c` | `--cpu` | Collects architecture, cores, and real-time frequency data. |
| `-g` | `--gpu` | Performs dynamic bus scanning for all installed GPUs. |
| `-h` | `--help` | Displays the help menu. |
| `-j` | `--json` | Serializes hardware data into a JSON object. |
| `-m` | `--mainboard` | Shows Mainboard/System DMI information (Run with `sudo` for Serial). |
| `-n` | `--network` | Lists network interfaces, drivers, and PCI bus topology. |
| `-O` | `--os` | Retrieves Operating System and Desktop Environment details. |
| `-o` | `--output <file>`| Redirects the JSON output to a specified file. |
| `-r` | `--ram` | Reports detailed memory utilization, cache, and swap metrics. |
| `-s` | `--storage` | Discovers block devices (NVMe, SSD, HDD) and their capacities. |
| `-T` | `--theme <name>`| Sets the UI theme (`cyberpunk`, `forest`, `ocean`, `sunset`, `monochrome`). |
| `-w` | `--watch` | Enables live watch mode with 1-second refreshes. |
| `-A` | `--ai <prompt>`| Sends hardware data to Groq AI to answer your prompt. |

---

## 📊 Sample Output

### Terminal (Human-Readable)

```text
╭─ Operating System (OS) 
│  Name            : Ubuntu 24.04 LTS
│  ID              : ubuntu
╰─

╭─ Mainboard / System 
│  Sys Vendor      : ASUSTeK COMPUTER INC.
│  Product Name    : ROG STRIX Z790-E GAMING WIFI
│  Board Vendor    : ASUSTeK COMPUTER INC.
│  Board Name      : ROG STRIX Z790-E GAMING WIFI
╰─

╭─ Central Processing Unit (CPU) 
│  Vendor          : AuthenticAMD
│  Model           : AMD Ryzen 9 7950X3D 16-Core Processor
│  Arch            : x86_64
│  Cores           : 16 Physical / 32 Logical
│  Frequency       : 4200.00 MHz - 5700.00 MHz
╰─
```

### AI Hardware Analysis

```text
$ hwmonitor --gpu -A "Is this GPU good for local AI inference?"

╭─ AI Hardware Analysis (Groq) 
│ The NVIDIA GeForce RTX 4090 is currently the premier consumer GPU for local 
│ AI inference. With 24GB of VRAM and high memory bandwidth, it can comfortably
│ run large models like Llama-3-70B using 4-bit quantization, or smaller 7B/8B
│ models with extremely high throughput.
╰─
```

---

## 📄 License

This project is licensed under the **MIT License**. See the `LICENSE` file for details.
