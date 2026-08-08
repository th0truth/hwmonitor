/**
 * \file display.h
 * \brief Header file for display module.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "os.h"
#include "cpu.h"
#include "ram.h"
#include "gpu.h"
#include "battery.h"
#include "mainboard.h"
#include "storage.h"
#include "network.h"

/**
 * \brief Displays OS information to the console.
 * \param[in] os Pointer to the OS structure.
 */
void display_os(const OS *os);

/**
 * \brief Displays CPU information to the console.
 * \param[in] cpu Pointer to the CPU structure.
 */
void display_cpu(const CPU *cpu);

/**
 * \brief Displays RAM information to the console.
 * \param[in] ram Pointer to the RAM structure.
 */
void display_ram(const RAM *ram);

/**
 * \brief Displays information for all GPUs to the console.
 * \param[in] gpus Array of pointers to GPU structures.
 * \param[in] count Number of GPUs in the array.
 */
void display_gpus(GPU **gpus, int count);

/**
 * \brief Displays Battery information to the console.
 * \param[in] battery Pointer to the BATTERY structure.
 */
void display_battery(const BATTERY *battery);

/**
 * \brief Displays Mainboard information to the console.
 * \param[in] mainboard Pointer to the MAINBOARD structure.
 */
void display_mainboard(const MAINBOARD *mainboard);

/**
 * \brief Displays information for all Storage devices to the console.
 * \param[in] storages Array of pointers to STORAGE structures.
 * \param[in] count Number of storages in the array.
 */
void display_storages(STORAGE **storages, int count);

/**
 * \brief Displays information for all Network interfaces to the console.
 * \param[in] networks Array of pointers to Network structures.
 * \param[in] count Number of networks in the array.
 */
void display_networks(Network **networks, int count);

/**
 * \brief Clears the terminal screen and resets cursor position using ANSI codes.
 */
void display_clear(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
