# PCem libretro core

A libretro core of PCem (PCem-mooch v11), the IBM PC and compatibles emulator: from the IBM 5150 PC and XT clones through the 286, 386 and 486 to Pentium (Socket 7) boards, with CGA, EGA, VGA and SVGA cards up to the S3 ViRGE and nVidia RIVA TNT, Sound Blaster, AdLib and Gravis UltraSound sound, and the 3dfx Voodoo. PCem's own manual is [readme.txt](readme.txt), with every machine and the ROM files it needs.

## Platforms

The libretro buildbot builds the core from `.gitlab-ci.yml`, including Linux aarch64. On x86 and x86-64 the CPU runs on PCem's dynamic recompiler or its interpreter (CPU Core option). The recompiler emits x86 code, so on other hosts (ARM64) every CPU runs on the interpreter and CPU Core stays on it; tested on a Snapdragon 7c (an XT, the AMI 486 and the Epox P55-VA boot), a fast PC is out of its reach there.

## Files

Everything goes under RetroArch's system directory, in `pcem/`:

- `pcem/roms/<set>/`: the machines', graphics cards' and other devices' ROM files, by the names in [readme.txt](readme.txt) (for example `pcem/roms/ami486/ami486.bin`). The case of the names does not matter, also on a case-sensitive file system.
- `pcem/pcem.cfg`: PCem's configuration, for what has no core option: the hard disk images (`hdc_fn`, `hdd_fn` with their sectors, heads and cylinders), the floppy and CD-ROM drives, memory size and the rest. Model, graphics card, sound card and CPU core come from the core options, not from this file.
- the machines' CMOS settings (`.nvr`) are kept there too.

The core starts without content: load it with Load Core, then Start Core. A machine with no disk boots to its BIOS.

## Core options

- **CPU Core**: interpreter or dynamic recompiler (x86 hosts only).
- **Model**, **Graphics card**, **Sound card** (restart): the machine, as PCem's configuration has them; auto takes the machine's own.
- **Video timing speed**, **Enable overscan**, **Enable flash**.
- **CPU Floating Point Unit**, **Gravis UltraSound**, **SSI 2001**, **GameBlaster**, **3Dfx Voodoo** (restart).

The "(restart)" options apply with Restart (RetroArch's Reset), which builds the machine again from the core options, as standalone PCem does when its settings change. If the new model lacks the CPU the old one had, its first CPU is used, and the memory is limited to what the model takes.

## Not supported

Save states, and content loaded through RetroArch (disks are set in `pcem.cfg`).

## License

GPL-2.0, see [COPYING](COPYING).
