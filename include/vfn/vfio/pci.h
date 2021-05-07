/* SPDX-License-Identifier: LGPL-2.1-or-later */

/*
 * This file is part of libvfn.
 *
 * Copyright (C) 2022 The libvfn Authors. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */

#ifndef LIBVFN_VFIO_PCI_H
#define LIBVFN_VFIO_PCI_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * struct vfio_pci_state - vfio pci device state
 */
struct vfio_pci_state {
	/**
	 * @vfio: vfio handle (see &struct vfio_state)
	 */
	struct vfio_state vfio;

	/**
	 * @bdf: pci device identifier ("bus:device:function")
	 */
	const char *bdf;

	/**
	 * @config_region_info: pci configuration space region information
	 */
	struct vfio_region_info config_region_info;

	/**
	 * @bar_region_info: pci BAR region information
	 */
	struct vfio_region_info bar_region_info[6];
};

/**
 * vfio_pci_open - open and initialize pci device
 * @pci: &struct vfio_pci_state to initialize
 * @bdf: pci device identifier ("bus:device:function") to open
 *
 * Open the pci device identified by @bdf and initialize @pci.
 *
 * Return: On success, returns ``0``. On error, returns ``-1`` and sets
 * ``errno``.
 */
int vfio_pci_open(struct vfio_pci_state *pci, const char *bdf);

/**
 * vfio_pci_map_bar - map a vfio device region into virtual memory
 * @pci: &struct vfio_pci_state
 * @idx: the vfio region index to map
 * @len: number of bytes to map
 * @offset: offset at which to start mapping
 * @prot: what accesses to permit to the mapped area (see ``man mmap``).
 *
 * Map the vfio device memory region identified by @idx into virtual memory.
 *
 * Return: On success, returns the virtual memory address mapped. On error,
 * returns ``NULL`` and sets ``errno``.
 */
void *vfio_pci_map_bar(struct vfio_pci_state *pci, unsigned int idx, size_t len, uint64_t offset,
		       int prot);

/**
 * vfio_pci_unmap_bar - unmap a vfio device region in virtual memory
 * @pci: &struct vfio_pci_state
 * @idx: the vfio region index to unmap
 * @mem: virtual memory address to unmap
 * @len: number of bytes to unmap
 * @offset: offset at which to start unmapping
 *
 * Unmap the virtual memory address, previously mapped to the vfio device memory
 * region identified by @idx.
 */
void vfio_pci_unmap_bar(struct vfio_pci_state *pci, unsigned int idx, void *mem, size_t len,
			uint64_t offset);

/**
 * vfio_pci_read_config - Read from the PCI configuration space
 * @pci: &struct vfio_pci_state
 * @buf: buffer to store the bytes read
 * @len: number of bytes to read
 * @offset: offset at which to read
 *
 * Read a number of bytes at a specified @offset in the PCI configuration space.
 *
 * Return: On success, returns the number of bytes read. On error, return ``-1``
 * and set ``errno``.
 */
static inline ssize_t vfio_pci_read_config(struct vfio_pci_state *pci, void *buf, size_t len,
					   off_t offset)
{
	return pread(pci->vfio.device, buf, len, pci->config_region_info.offset + offset);
}

/**
 * vfio_pci_write_config - Write into the PCI configuration space
 * @pci: &struct vfio_pci_state
 * @buf: buffer to write
 * @len: number of bytes to write
 * @offset: offset at which to write
 *
 * Write a number of bytes at a specified @offset in the PCI configuration space.
 *
 * Return: On success, returns the number of bytes written. On error, return
 * ``-1`` and set ``errno``.
 */
static inline ssize_t vfio_pci_write_config(struct vfio_pci_state *pci, void *buf, size_t len,
					    off_t offset)
{
	return pwrite(pci->vfio.device, buf, len, pci->config_region_info.offset + offset);
}

#ifdef __cplusplus
}
#endif

#endif /* LIBVFN_VFIO_PCI_H */
