#include "base.h"
#include "theme.h"
#include "util.h"

#include "cpu.h"
#include "ram.h"
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

void display_ram(const RAM* ram)
{
  if (!ram)
    return;

  // Calculation for actual "used" application memory
  uint64_t used = ram->total - ram->free - ram->buffers - ram->cached;

  // Prevent underflow just in case kernel reports weird numbers during race conditions
  if (ram->total < (ram->free + ram->buffers + ram->cached))
    used = 0;
  
  uint64_t swap_used = ram->swap_total > ram->swap_free ? (ram->swap_total - ram->swap_free) : 0;

  // Calculate percentages
  double ram_pct = ram->total > 0 ? ((double)used / ram->total) * 100.0 : 0.0;
  double swap_pct = ram->swap_total > 0 ? ((double)swap_used / ram->swap_total) * 100.0 : 0.0;

  // Format the numbers into strings
  char total_str[32];
  char free_str[32];
  char used_str[32];
  char avail_str[32];
  char buffers_str[32];
  char cached_str[32];
  char cache_str[32];

  char swap_tot_str[32];
  char swap_free_str[32];
  char swap_used_str[32];
  char zswap_str[32];
  char zswapped_str[32];
  char dirty_str[32];
  char per_cpu_str[32];

  format_size("GiB", ram->total * 1024ULL, total_str, sizeof(total_str));
  format_size("GiB", ram->free * 1024ULL, free_str, sizeof(free_str));
  format_size("GiB", used * 1024ULL, used_str, sizeof(used_str));
  format_size("GiB", ram->available * 1024ULL, avail_str, sizeof(avail_str));
  format_size("GiB", ram->buffers * 1024ULL, buffers_str, sizeof(buffers_str));
  format_size("GiB", ram->cached * 1024ULL, cached_str, sizeof(cached_str));
  format_size("GiB", (ram->buffers + ram->cached) * 1024ULL, cache_str, sizeof(cache_str));

  format_size("GiB", ram->swap_total * 1024ULL, swap_tot_str, sizeof(swap_tot_str));
  format_size("GiB", ram->swap_free * 1024ULL, swap_free_str, sizeof(swap_free_str));
  format_size("GiB", swap_used * 1024ULL, swap_used_str, sizeof(swap_used_str));
  
  format_size("MiB", ram->zswap * 1024ULL, zswap_str, sizeof(zswap_str));
  format_size("MiB", ram->zswapped * 1024ULL, zswapped_str, sizeof(zswapped_str));
  format_size("MiB", ram->dirty * 1024ULL, dirty_str, sizeof(dirty_str));
  format_size("MiB", ram->per_cpu * 1024ULL, per_cpu_str, sizeof(per_cpu_str));

  print_header("Random Access Memory (RAM)");

  print_field("Total", "%s", total_str);
  print_field("Free", "%s", free_str);
  print_field("Used", "%s (%.1f%%)", used_str, ram_pct);
  print_field("Available", "%s", avail_str);
  print_field("Buffers", "%s", buffers_str);
  print_field("Cached", "%s", cached_str);
  print_field("Buff/Cache", "%s", cache_str);
  print_field("Swap Total", "%s", swap_tot_str);
  print_field("Swap Free", "%s", swap_free_str);
  print_field("Swap Used", "%s", swap_used_str);
  print_field("ZSwap", "%s", zswap_str);
  print_field("ZSwapped", "%s", zswapped_str);
  print_field("Dirty", "%s", dirty_str);
  print_field("Per-CPU", "%s", per_cpu_str);

  print_footer();
}

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
