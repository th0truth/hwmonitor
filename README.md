# **hwmonitor**

hwmonitor is a C-library that provides easy way to retrieve hardware information of your device components such as CPU, GPU (dGPU or iGPU), RAM, Battery, OS, etc.

## **Installation**

The current way to install hwmonitor is from source.

### **From source**

```bash
git clone https://github.com/th0truth/hwmonitor.git 
cd hwmonitor
make
```

## **Usage**

```bash
./build/hwmonitor.o # Pass arguments (-h | --help)
```


# Supported Components

| Component |  Info  | Linux | Windows |
|:-----|:--------:|------:|------:|
| CPU | Architecture | ✔️ | ❌  |
|     | Vendor      | ✔️ |  ❌  |
|     | Model        | ✔️️️ | ❌  |
|     | Mode name    | ✔️ | ❌  |
|     | Stepping     | ✔️ | ❌  |
|     | Total threads | ✔️| ❌  |
|     | Total cores | ✔️ | ❌   |
|     | Total processors | ✔️ | ❌ |
|     | Max CPU frequency | ✔️ | ❌ |
|     | Min CPU frequency | ✔️ | ❌ |
|     | Temperature | ✔️ | ❌ |
|     | Online processors | ✔️ | ❌ |
| GPU | Vendor | ✔️ | ❌ |
|     | Device | ✔️ | ❌ |
|     | Driver | ✔️ | ❌ |
|      | Model name | ✔️ | ❌ |❌
|     | Subsys Vendor | ✔️ | ❌ |
|     | PCI ID | ✔️ | ❌ |
|     | PCI subsys | ✔️ | ❌ | 
|     | PCI SLOT NAME | ✔️ | ❌ | 
| MEM | Total | ✔️ | ❌ |
|     | Free | ✔️ | ❌ |
|     | Available | ✔️ | ❌ |
|     | Buffers | ✔️ | ❌ |
|     | Cached | ✔️ | ❌ |
|     | SwapTotal | ✔️ | ❌ |
|     | SwapFree | ✔️ | ❌ |
|     | Zswap | ✔️ | ❌ |
|     | Dirty | ✔️ | ❌ |
|     | perCPU | ✔️ | ❌ |
| BAT | Capacity | ✔️| ❌ |
|     | Voltage min design | ✔️ | ❌ |
|     | Voltage now | ✔️ | ❌ |
|     | Supply energy full design | ✔️ | ❌ |
|     | Supply energy full | ✔️ | ❌ |
|     | Supply name | ✔️ | ❌ |
|     | Supply type | ✔️ | ❌ |
|     | Supply capacity normal | ✔️ | ❌ |
|     | Supply status | ✔️ | ❌ |
|     | Supply technology | ✔️ | ❌ |
|     | Model name | ✔️ | ❌ |
|     | Manufacturer | ✔️ | ❌|
|     | Serial | ✔️ |  ❌ |
| BIOS | Vendor | ✔️ | ❌ |
|      | Version | ✔️| ❌ |
|      | Release date | ✔️ | ❌ |
| OS  | Distro | ✔️ | ❌ |
|     | Version | ✔️ | ❌|
|     | DE | ✔️ | ❌ |
|     | DE ID | ✔️ | ❌|
|     | RELEASE TYPE | ✔️ | ❌ |
| Motherboard | Manufacturer | ✔️ | ❌ |
|     | Model | ✔️ | ❌ |
|     | Version | ✔️ | ❌ |
| Mainboard | Model | ✔️ | ❌|
|     | Family | ✔️ | ❌ |
|     | Serial | ✔️ | ❌| 
|     | SKU | ✔️ | ❌ |