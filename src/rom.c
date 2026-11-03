#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ibm.h"
#include "mem.h"
#include "rom.h"

#if !defined(_WIN32)
#include <dirent.h>
#include <strings.h>

/* The ROM sets are made on Windows: the code asks for "ami486.BIN" where the
 * file is ami486.bin. Each part of the path after pcempath is matched without
 * regard to case when the exact name is not there. */
static void rom_path(char *s, const char *fn)
{
        char rest[512], *part, *save;

        strcpy(s, pcempath);
        put_backslash(s);
        strcpy(rest, fn);
        for (part = strtok_r(rest, "/", &save); part; part = strtok_r(NULL, "/", &save))
        {
                size_t len = strlen(s);
                DIR *d = opendir(s);
                const char *name = part;
                struct dirent *e;

                while (d && (e = readdir(d)))
                {
                        if (!strcmp(e->d_name, part))
                        {
                                name = part;
                                break;
                        }
                        if (!strcasecmp(e->d_name, part))
                                name = e->d_name;
                }
                snprintf(s + len, 512 - len, "%s%s", name, save && *save ? "/" : "");
                if (d)
                        closedir(d);
        }
}
#else
static void rom_path(char *s, const char *fn)
{
        strcpy(s, pcempath);
        put_backslash(s);
        strcat(s, fn);
}
#endif

FILE *romfopen(char *fn, char *mode)
{
        char s[512];
        rom_path(s, fn);
        return fopen(s, mode);
}

int rom_present(char *fn)
{
        FILE *f;
        char s[512];
        
        rom_path(s, fn);
        f = fopen(s, "rb");
        if (f)
        {
                fclose(f);
                return 1;
        }
        return 0;
}

static uint8_t rom_read(uint32_t addr, void *p)
{
        rom_t *rom = (rom_t *)p;
//        pclog("rom_read : %08x %08x %02x\n", addr, rom->mask, rom->rom[addr & rom->mask]);
        return rom->rom[addr & rom->mask];
}
uint16_t rom_readw(uint32_t addr, void *p)
{
        rom_t *rom = (rom_t *)p;
//        pclog("rom_readw: %08x %08x %04x\n", addr, rom->mask, *(uint16_t *)&rom->rom[addr & rom->mask]);
        return *(uint16_t *)&rom->rom[addr & rom->mask];
}
uint32_t rom_readl(uint32_t addr, void *p)
{
        rom_t *rom = (rom_t *)p;
//        pclog("rom_readl: %08x %08x %08x\n", addr, rom->mask, *(uint32_t *)&rom->rom[addr & rom->mask]);
        return *(uint32_t *)&rom->rom[addr & rom->mask];
}

int rom_init(rom_t *rom, char *fn, uint32_t address, int size, int mask, int file_offset, uint32_t flags)
{
        FILE *f = romfopen(fn, "rb");
        
        if (!f)
        {
                pclog("ROM image not found : %s\n", fn);
                return -1;
        }
        
        rom->rom = malloc(size);
        fseek(f, file_offset, SEEK_SET);
        fread(rom->rom, size, 1, f);
        fclose(f);
        
        rom->mask = mask;
        
        mem_mapping_add(&rom->mapping, address, size, rom_read,
                                                      rom_readw,
                                                      rom_readl,
                                                      mem_write_null,
                                                      mem_write_nullw,
                                                      mem_write_nulll,
                                                      rom->rom,
                                                      flags,
                                                      rom);

        return 0;
}

int rom_init_interleaved(rom_t *rom, char *fn_low, char *fn_high, uint32_t address, int size, int mask, int file_offset, uint32_t flags)
{
        FILE *f_low  = romfopen(fn_low, "rb");
        FILE *f_high = romfopen(fn_high, "rb");
        int c;
        
        if (!f_low || !f_high)
        {
                if (!f_low)
                        pclog("ROM image not found : %s\n", fn_low);
                else
                        fclose(f_low);
                if (!f_high)
                        pclog("ROM image not found : %s\n", fn_high);
                else
                        fclose(f_high);
                return -1;
        }
        
        rom->rom = malloc(size);
        fseek(f_low, file_offset, SEEK_SET);
        fseek(f_high, file_offset, SEEK_SET);
        for (c = 0; c < size; c += 2)
        {
                rom->rom[c]     = getc(f_low);
                rom->rom[c + 1] = getc(f_high);
        }
        fclose(f_high);
        fclose(f_low);
        
        rom->mask = mask;
        
        mem_mapping_add(&rom->mapping, address, size, rom_read,
                                                      rom_readw,
                                                      rom_readl,
                                                      mem_write_null,
                                                      mem_write_nullw,
                                                      mem_write_nulll,
                                                      rom->rom,
                                                      flags,
                                                      rom);

        return 0;
}
