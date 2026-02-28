#include "base.h"
#include "theme.h"

#include "cpu.h"
#include "gpu.h"

void display_cpu(const CPU* cpu)
{
  if (!cpu)
    return;

  print_header("Central Processing Unit (CPU)");

  print_field("Vendor", "%s", STR_OR_UNK(cpu->vendor_id));
  print_field("Model", "%s", STR_OR_UNK(cpu->model_name));
  print_field("Arch", "%s", STR_OR_UNK(cpu->arch));
  print_field("Cores", "%u Physical / %u Logical", cpu->total_cores, cpu->total_threads);
  print_field("Frequency", "%.2f MHz - %.2f MHz", cpu->min_MHz, cpu->max_MHz);

  print_footer();
};

void display_gpus(GPU** gpus, int count)
{
  if (!gpus || count == 0)
    return;

  for (int i = 0; i < count; i++) {
    if (count > 1) {
      char h_title[64];
      snprintf(h_title, sizeof(h_title), "Graphics Processing Unit [%d]", i);    
      print_header(h_title);
    } else {
      print_header("Graphics Processing Unit (GPU)");
    }

    if (gpus[i]->vendor && gpus[i]->device_id) {
      print_field("Vendor", "%s (%s)", STR_OR_UNK(gpus[i]->vendor), STR_OR_UNK(gpus[i]->device_id));
    } else {
      print_field("Vendor", "%s", STR_OR_UNK(gpus[i]->vendor));
    }

    print_field("Model", "%s", STR_OR_UNK(gpus[i]->model));
    print_field("PCI Slot", "%s", STR_OR_UNK(gpus[i]->pci_slot_name));
    print_field("Bus Type", "%s", STR_OR_UNK(gpus[i]->bus_type));
    print_field("Driver", "%s", STR_OR_UNK(gpus[i]->driver));
  
    if (gpus[i]->vbios && gpus[i]->vbios[0] != '\0') {
      print_field("VBIOS", "%s", gpus[i]->vbios);
    }

    print_footer();
  }
}
