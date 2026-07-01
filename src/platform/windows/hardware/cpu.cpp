extern "C" {
#include "base.h"
#include "cpu.h"
}

#include <windows.h>

namespace {

constexpr const wchar_t kCpuKeyPath[] = L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0";

char* dup_utf8(const wchar_t* value)
{
  if (!value)
    return NULL;

  int len = WideCharToMultiByte(CP_UTF8, 0, value, -1, NULL, 0, NULL, NULL);
  if (len <= 0)
    return NULL;

  char* out = (char*)calloc((size_t)len, sizeof(char));
  if (!out)
    return NULL;

  if (!WideCharToMultiByte(CP_UTF8, 0, value, -1, out, len, NULL, NULL)) {
    free(out);
    return NULL;
  }

  return out;
}

char* dup_ascii(const char* value)
{
  if (!value)
    return NULL;

  size_t len = strlen(value) + 1;
  char* out = (char*)calloc(len, sizeof(char));
  if (!out)
    return NULL;

  memcpy(out, value, len);
  return out;
}

char* reg_read_string(const wchar_t* value_name)
{
  HKEY key = NULL;
  LONG status = RegOpenKeyExW(HKEY_LOCAL_MACHINE, kCpuKeyPath, 0, KEY_READ, &key);
  if (status != ERROR_SUCCESS)
    return NULL;

  DWORD type = 0;
  DWORD bytes = 0;
  status = RegQueryValueExW(key, value_name, NULL, &type, NULL, &bytes);
  if (status != ERROR_SUCCESS || (type != REG_SZ && type != REG_EXPAND_SZ)) {
    RegCloseKey(key);
    return NULL;
  }

  wchar_t* buffer = (wchar_t*)calloc((size_t)(bytes / sizeof(wchar_t)) + 1, sizeof(wchar_t));
  if (!buffer) {
    RegCloseKey(key);
    return NULL;
  }

  status = RegQueryValueExW(key, value_name, NULL, &type, (LPBYTE)buffer, &bytes);
  RegCloseKey(key);

  if (status != ERROR_SUCCESS) {
    free(buffer);
    return NULL;
  }

  char* result = dup_utf8(buffer);
  free(buffer);
  return result;
}

bool reg_read_dword(const wchar_t* value_name, uint32_t* out)
{
  HKEY key = NULL;
  LONG status = RegOpenKeyExW(HKEY_LOCAL_MACHINE, kCpuKeyPath, 0, KEY_READ, &key);
  if (status != ERROR_SUCCESS)
    return false;

  DWORD type = 0;
  DWORD bytes = sizeof(DWORD);
  DWORD value = 0;
  status = RegQueryValueExW(key, value_name, NULL, &type, (LPBYTE)&value, &bytes);
  RegCloseKey(key);

  if (status != ERROR_SUCCESS || type != REG_DWORD)
    return false;

  *out = (uint32_t)value;
  return true;
}

uint16_t bit_count(ULONG_PTR mask)
{
  uint16_t count = 0;
  while (mask) {
    count += (uint16_t)(mask & 1u);
    mask >>= 1u;
  }
  return count;
}

void parse_identifier(const char* identifier, uint16_t* family, uint16_t* model, uint16_t* stepping)
{
  if (!identifier)
    return;

  const char* family_pos = strstr(identifier, "Family ");
  if (!family_pos)
    return;

  unsigned int fam = 0;
  unsigned int mod = 0;
  unsigned int step = 0;
  if (sscanf(family_pos, "Family %u Model %u Stepping %u", &fam, &mod, &step) == 3) {
    *family = (uint16_t)fam;
    *model = (uint16_t)mod;
    *stepping = (uint16_t)step;
  }
}

char* arch_to_string(void)
{
  SYSTEM_INFO info;
  GetNativeSystemInfo(&info);

  switch (info.wProcessorArchitecture) {
    case PROCESSOR_ARCHITECTURE_INTEL:
      return dup_ascii("x86");
    case PROCESSOR_ARCHITECTURE_AMD64:
      return dup_ascii("x86_64");
    case PROCESSOR_ARCHITECTURE_ARM:
      return dup_ascii("ARM");
    case PROCESSOR_ARCHITECTURE_ARM64:
      return dup_ascii("ARM64");
    case PROCESSOR_ARCHITECTURE_IA64:
      return dup_ascii("Itanium");
    default:
      return dup_ascii("<unknown>");
  }
}

void get_topology(uint16_t* total_cores, uint16_t* total_threads)
{
  bool collected = false;
  DWORD size = 0;
  GetLogicalProcessorInformationEx(RelationAll, NULL, &size);

  if (size > 0) {
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX info =
      (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)calloc(1, (size_t)size);
    if (info && GetLogicalProcessorInformationEx(RelationAll, info, &size)) {
      BYTE* cursor = (BYTE*)info;
      BYTE* end = (BYTE*)info + size;
      uint16_t cores = 0;
      uint16_t threads = 0;

      while (cursor < end) {
        PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX item =
          (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)cursor;

        if (item->Relationship == RelationProcessorCore) {
          cores++;
          for (WORD i = 0; i < item->Processor.GroupCount; i++)
            threads += bit_count(item->Processor.GroupMask[i].Mask);
        }

        cursor += item->Size;
      }

      if (cores > 0)
        *total_cores = cores;
      if (threads > 0)
        *total_threads = threads;

      collected = (*total_threads > 0);
    }

    free(info);
  }

  if (!collected) {
    SYSTEM_INFO sys_info;
    GetSystemInfo(&sys_info);
    if (sys_info.dwNumberOfProcessors > 0) {
      *total_threads = (uint16_t)sys_info.dwNumberOfProcessors;
      if (*total_cores == 0)
        *total_cores = (uint16_t)sys_info.dwNumberOfProcessors;
    }
  }
}

}  // namespace

CPU* cpu_get_info(void)
{
  CPU* cpu = (CPU*)calloc(1, sizeof(CPU));
  if (!cpu)
    return NULL;

  cpu->vendor_id = reg_read_string(L"VendorIdentifier");
  cpu->model_name = reg_read_string(L"ProcessorNameString");
  cpu->flags = reg_read_string(L"Identifier");
  cpu->arch = arch_to_string();

  uint32_t value = 0;
  if (reg_read_dword(L"~MHz", &value)) {
    cpu->max_MHz = (float)value;
    cpu->min_MHz = (float)value;
  }

  parse_identifier(cpu->flags, &cpu->cpu_family, &cpu->model, &cpu->stepping);

  get_topology(&cpu->total_cores, &cpu->total_threads);
  cpu->online_cores = cpu->total_threads;

  return cpu;
}

void free_cpu(CPU* cpu)
{
  if (!cpu)
    return;

  free(cpu->vendor_id);
  free(cpu->model_name);
  free(cpu->flags);
  free(cpu->arch);
  free(cpu);
}

cJSON* cpu_to_json_obj(const CPU* cpu)
{
  cJSON* obj = cJSON_CreateObject();
  if (!cpu)
    return obj;

  cJSON_AddStringToObject(obj, "vendor", STR_OR_UNK(cpu->vendor_id));
  cJSON_AddStringToObject(obj, "model_name", STR_OR_UNK(cpu->model_name));
  cJSON_AddStringToObject(obj, "flags", STR_OR_UNK(cpu->flags));
  cJSON_AddStringToObject(obj, "arch", STR_OR_UNK(cpu->arch));

  cJSON_AddNumberToObject(obj, "max_freq_mhz", cpu->max_MHz);
  cJSON_AddNumberToObject(obj, "min_freq_mhz", cpu->min_MHz);
  cJSON_AddNumberToObject(obj, "online_cores", cpu->online_cores);
  cJSON_AddNumberToObject(obj, "cpu_family", cpu->cpu_family);
  cJSON_AddNumberToObject(obj, "model", cpu->model);
  cJSON_AddNumberToObject(obj, "stepping", cpu->stepping);
  cJSON_AddNumberToObject(obj, "total_cores", cpu->total_cores);
  cJSON_AddNumberToObject(obj, "total_threads", cpu->total_threads);

  return obj;
}
