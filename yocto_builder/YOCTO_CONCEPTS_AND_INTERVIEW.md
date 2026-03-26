# Yocto Project: Concepts & Interview Guide

## Table of Contents
1. [What is Yocto?](#what-is-yocto)
2. [Core Concepts](#core-concepts)
3. [Architecture](#architecture)
4. [Key Components](#key-components)
5. [Recipes & Layers](#recipes--layers)
6. [BitBake](#bitbake)
7. [Variables & Configuration](#variables--configuration)
8. [Build Process](#build-process)
9. [Interview Questions](#interview-questions)
10. [Best Practices](#best-practices)

---

## What is Yocto?

**Yocto Project** is a collaboration between the Linux Foundation and key industry players that provides tools and metadata for building custom Linux distributions for embedded systems.

### Key Characteristics:
- **Meta-level build system**: Builds a build system, not just software
- **Open-source**: Free and community-driven
- **Flexible**: Highly customizable for various embedded Linux distributions
- **Based on BitBake**: Uses BitBake as its core build tool
- **Layer-based**: Organized around modular layers for easy customization
- **Cross-platform**: Works on Linux, macOS, and Windows hosts

### Why Yocto?
- Reduce development time for embedded Linux
- Create minimal, optimized images
- Manage complex dependencies
- Enable reproducible builds
- Support multiple hardware platforms
- Full control over what goes into the final image

---

## Core Concepts

### 1. **Distribution (Distro)**
A collection of policies, recipes, and variables that define a specific Linux distribution.

```
poky/          # Default Yocto distribution
├── meta/      # Core metadata
├── meta-*     # Additional layers
└── conf/distro/poky.conf  # Distro configuration
```

### 2. **Metadata**
Information describing how to build software. Includes:
- **Recipes (.bb files)**: Instructions to build a package
- **Classes (.bbclass files)**: Reusable build logic
- **Configuration files (.conf)**: Build settings

### 3. **Poky**
The reference distribution provided by the Yocto Project.
- Default starting point for Yocto
- Contains essential layers (meta, meta-poky)
- Fully functional standalone distribution

### 4. **Board Support Package (BSP)**
Layer containing hardware-specific configurations:
- Device tree definitions
- Kernel configurations
- Hardware drivers
- Machine definitions

**Example:**
```
meta-raspberrypi/    # BSP for Raspberry Pi
├── conf/machine/raspberrypi4.conf
├── recipes-bsp/u-boot/
└── recipes-kernel/linux/
```

### 5. **Image**
The final output: a complete Linux filesystem for a device.

**Types:**
- **core-image-minimal**: Minimal image with just the kernel
- **core-image-base**: Basic filesystem with package management
- **core-image-full**: Full-featured image
- **Custom images**: User-defined images

---

## Architecture

### High-Level Yocto Architecture

```
Host Machine (Build Machine)
    │
    ├─ Yocto Build System
    │   ├─ BitBake Build Engine
    │   ├─ Metadata (Layers)
    │   └─ Configuration Files
    │
    ├─ Inputs:
    │   ├─ Recipes (.bb)
    │   ├─ Layers
    │   ├─ Configuration (local.conf, bblayers.conf)
    │   └─ Source Code
    │
    └─ Outputs:
        ├─ Root Filesystem
        ├─ Kernel Image
        ├─ Bootloader
        └─ Device Tree Blob
```

### Build System Components

```
┌─────────────────────────────────────┐
│     Yocto Project Environment       │
└─────────────────────────────────────┘
        │
        ├─ BitBake (Task Scheduler)
        │   ├─ Parses recipes
        │   ├─ Resolves dependencies
        │   └─ Executes build tasks
        │
        ├─ Layers (Metadata Organization)
        │   ├─ meta/ (Core)
        │   ├─ meta-bsp/ (Hardware)
        │   └─ meta-custom/ (User-defined)
        │
        └─ Configuration
            ├─ local.conf
            ├─ bblayers.conf
            └─ machine/distro configs
```

---

## Key Components

### 1. **BitBake**
The task scheduler and execution engine for Yocto.

**Functions:**
- Parses recipes and configuration
- Resolves dependencies
- Executes build tasks in correct order
- Generates build cache
- Manages parallel builds

**Key Commands:**
```bash
bitbake core-image-minimal    # Build target image
bitbake -c fetch recipe-name  # Download sources
bitbake -c compile recipe-name # Compile only
bitbake -c clean recipe-name  # Clean build artifacts
bitbake -g core-image-minimal # Generate dependency graph
bitbake-layers show-recipes   # List available recipes
```

### 2. **Recipes (.bb files)**
Build instructions for a single package.

**Basic Recipe Structure:**
```makefile
# meta-custom/recipes-apps/myapp/myapp_1.0.bb

SUMMARY = "My Custom Application"
DESCRIPTION = "A simple application"
LICENSE = "MIT"
AUTHOR = "Your Name"

SRC_URI = "https://example.com/myapp-1.0.tar.gz"
SRC_URI[sha256sum] = "abc123..."

S = "${WORKDIR}/myapp-1.0"

inherit autotools

DEPENDS = "libxml2 openssl"

EXTRA_OEMAKE = "CFLAGS='${CFLAGS}' LDFLAGS='${LDFLAGS}'"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${B}/myapp ${D}${bindir}/
}

FILES_${PN} = "${bindir}/myapp"
```

### 3. **Classes (.bbclass)**
Reusable code and common functionality.

**Common Classes:**
```makefile
inherit autotools    # Autotools build system
inherit cmake        # CMake build system
inherit python       # Python recipe
inherit native       # Native tool for build machine
inherit cross        # Cross-compilation
inherit kernel       # Linux kernel recipe
inherit module       # Kernel module
```

### 4. **Configuration Files**

#### **local.conf**
User-specific build settings:

```bash
# Build configuration
MACHINE = "qemux86-64"          # Target machine
DISTRO = "poky"                 # Distribution
TMPDIR = "${TOPDIR}/tmp"        # Build directory

# Build options
PARALLEL_MAKE = "-j 4"           # Parallel jobs
BB_NUMBER_THREADS = "4"         # BitBake threads

# Package format
PACKAGE_CLASSES = "package_ipk"  # IPK, DEB, RPM

# Image features
IMAGE_FEATURES = "debug-tweaks ssh-server-openssh"

# Size optimization
IMAGE_ROOTFS_MAXSIZE = "1048576" # 1GB limit
```

#### **bblayers.conf**
Layer configuration:

```bash
BBLAYERS ?= " \
    ${TOPDIR}/../meta \
    ${TOPDIR}/../meta-poky \
    ${TOPDIR}/../meta-custom \
    ${TOPDIR}/../meta-raspberrypi \
"

BBLAYERS_NON_REMOVABLE ?= " \
    ${TOPDIR}/../meta \
    ${TOPDIR}/../meta-poky \
"
```

---

## Recipes & Layers

### Recipe Naming Convention

```
meta-custom/recipes-apps/myapp/myapp_1.0.bb
                      │       │    │
                      │       │    └─ Version
                      │       └────── Package name
                      └───────────── Recipe category
```

### Recipe Variables & Functions

**Common Variables:**

```makefile
PN = "${BPN}"                    # Package name
PV = "1.0"                       # Package version
PR = "r0"                        # Package revision
PF = "${PN}-${PV}"               # Full package name

S = "${WORKDIR}/${BPN}-${PV}"    # Source directory
B = "${WORKDIR}/build"           # Build directory
D = "${WORKDIR}/image"           # Destination (staging)

SRC_URI = "https://.../${PF}.tar.gz"
SRCREV = "abc123def456"          # Git commit hash

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=..."

DEPENDS = "dep1 dep2"            # Build-time dependencies
RDEPENDS = "dep1 dep2"           # Runtime dependencies
PROVIDES = "virtual/package"     # Virtual package

FILES_${PN} = "${bindir}/* ${libdir}/*"
CONFFILES_${PN} = "${sysconfdir}/myapp.conf"
```

**Common Functions:**

```makefile
# Download sources
do_fetch() { }

# Unpack sources
do_unpack() { }

# Patch sources
do_patch() { }

# Configure build
do_configure() { }

# Compile
do_compile() { }

# Install to staging area
do_install() { }

# Package installation
do_package() { }

# Create package feed
do_package_write_ipk/deb/rpm() { }

do_install_append() {
    # Additional installation steps
    install -d ${D}${sysconfdir}
    install -m 0644 myapp.conf ${D}${sysconfdir}/
}
```

### Layers

**Layer Purpose:** Organize metadata logically (BSP, distro, features, custom).

**Standard Layer Structure:**

```
meta-custom/
├── conf/
│   ├── layer.conf          # Layer configuration
│   ├── distro/custom.conf  # Distro definition
│   └── machine/            # Machine definitions
├── recipes-core/
│   ├── images/
│   |   └── core-image-custom.bb
│   └── glib/
├── recipes-kernel/
│   ├── linux/
│   └── linux-firmware/
├── recipes-apps/
│   └── myapp/
│       └── myapp_1.0.bb
├── recipes-bsp/
│   └── u-boot/
├── classes/
│   └── myclass.bbclass
└── README.md
```

**layer.conf Example:**

```makefile
# meta-custom/conf/layer.conf

BBPATH .= ":${LAYERDIR}"

BBFILES += "${LAYERDIR}/recipes-*/*/*.bb \
            ${LAYERDIR}/recipes-*/*/*.bbappend"

BBFILE_COLLECTIONS += "custom"
BBFILE_PATTERN_custom = "^${LAYERDIR}/"
BBFILE_PRIORITY_custom = "7"

LAYERDEPENDS_custom = "core"
LAYERSERIES_COMPAT_custom = "kirkstone langdale"
```

---

## BitBake

### BitBake Workflow

```
1. Parse Configuration
   └─ local.conf, bblayers.conf, machine/*.conf

2. Parse Recipes
   └─ Collect all .bb and .bbappend files

3. Dependency Analysis
   └─ Calculate build order

4. Execute Tasks
   └─ do_fetch → do_unpack → do_patch → do_configure 
      → do_compile → do_install → do_package

5. Generate Output
   └─ Root filesystem, packages, images
```

### BitBake Task Classes

```bash
# List all available tasks
bitbake -c listtasks core-image-minimal

# Common tasks
do_fetch      # Download source code
do_unpack     # Extract archives
do_patch      # Apply patches
do_configure  # Configure source
do_compile    # Compile source
do_install    # Install to stage directory
do_package    # Create package files
do_build      # Complete build (default)
```

### Task Dependencies

**Task names format:** `recipe:task_name`

```bash
# A task depends on recipe:do_populate_sysroot
do_configure[depends] = "libxml2:do_populate_sysroot"

# Task runs before another
do_configure[deptask] = "do_populate_sysroot"

# Use after directive
do_configure[postdeps] = "do_unpack"
```

---

## Variables & Configuration

### Variable Assignment Operators

```makefile
VAR = "value"           # Direct assignment
VAR += "value"          # Append with space
VAR .= "value"          # Append without space
VAR =+ "value"          # Prepend with space
VAR =. "value"          # Prepend without space
VAR ?= "value"          # Conditional (not set yet)
VAR ??= "value"         # Conditional (weak default)
VAR_append = "value"    # Append (older style)
VAR_prepend = "value"   # Prepend (older style)
```

### Expansion and Substitution

```makefile
# Package-specific variable override
CFLAGS_append_pn-gcc = "-O3"

# Machine-specific override
KERNEL_DEVICETREE_append_machine-rpi4 = " \
    broadcom/bcm2711-raspberrypi-4-b.dtb"

# Variable substitution
FILES = "${bindir}/* ${libdir}/*"
INSTALL_PATH = "${exec_prefix}/bin"

# Conditional variables
KERNEL_IMAGETYPE = "${@bb.utils.contains('DISTRO_FEATURES', \
    'systemd', 'Image', 'zImage', d)}"
```

### Important Variables

```makefile
# Build machine info
BUILD = "x86_64-linux"
NATIVELSBSTRING = "ubuntu-20.04"

# Target machine
TARGET_ARCH = "arm"
TARGET_CPU = "cortex-a53"
TARGET_VENDOR = "rpi"

# Image features
IMAGE_FEATURES = "debug-tweaks ssh-server-openssh package-management"
DISTRO_FEATURES = "systemd x11 opengl"

# Package formats
PACKAGE_CLASSES = "package_ipk"  # IPK, DEB, or RPM

# Optimization
INHERITED_IMAGE_BASENAME = "core-image-minimal"
EXTRA_IMAGE_FEATURES = "ssh-server-openssh"
```

---

## Build Process

### Step-by-Step Build Flow

```
1. Environment Setup
   └─ source oe-init-build-env build_directory

2. Configuration
   └─ Edit conf/local.conf and conf/bblayers.conf

3. BitBake Invocation
   └─ bitbake core-image-minimal

4. Parsing Phase
   ├─ Parse configuration files
   ├─ Parse recipes from all layers
   └─ Resolve dependencies

5. Execution Phase
   ├─ do_fetch     (Download sources)
   ├─ do_unpack    (Extract)
   ├─ do_patch     (Apply patches)
   ├─ do_configure (Configure)
   ├─ do_compile   (Build)
   ├─ do_install   (Stage artifacts)
   ├─ do_package   (Create packages)
   ├─ do_rootfs    (Build filesystem)
   └─ do_image     (Create final image)

6. Output Generation
   └─ tmp/deploy/images/machine/
      ├─ core-image-minimal-...qemux86-64.ext4
      ├─ zImage
      ├─ Image
      └─ device-tree-blobs/
```

### Build Output Structure

```
tmp/
├── build/                    # Work directory
│   ├── recipe-name/          # Per-recipe build dir
│   │   ├── git/              # Git sources
│   │   ├── 1.0-r0/
│   │   │   ├── git/
│   │   │   ├── source/
│   │   │   ├── build/
│   │   │   └── image/
│   │   └── temp/
│   └── ...
├── sysroots/
│   ├── x86_64-linux/         # Native tools
│   ├── qemux86-64/           # Cross-compiled binaries
│   └── qemux86-64-tclibc/    # Cross-compile environment
├── cache/
│   └── bitbake-cookiedir/    # Task cache
├── deploy/
│   ├── images/qemux86-64/    # Final images
│   │   ├── core-image-minimal-qemux86-64.ext4
│   │   ├── kernel/
│   │   └── devicetree/
│   ├── rpm/ deb/ ipk/        # Package feeds
│   └── licenses/             # License info
├── log/
│   └── cooker/qemux86-64/    # Build logs
└── pickled-data/             # Cache data
```

---

## Interview Questions

### Basic Level

#### Q1: What is Yocto Project and what are its main advantages?
**Answer:**
Yocto Project is an open-source collaboration providing tools and metadata for building custom Linux distributions for embedded systems.

**Advantages:**
- **Customization**: Full control over what's included
- **Size optimization**: Create minimal images
- **Reproducibility**: Same build produces same output
- **Cross-compilation**: Build for different architectures
- **Dependency management**: Automatic handling of complex dependencies
- **Layer-based**: Modular and reusable components

#### Q2: Explain the difference between Poky and Yocto Project.
**Answer:**
- **Yocto Project**: Meta-project providing tools, specifications, and guidelines
- **Poky**: Reference implementation/distribution of Yocto Project
  - Contains core metadata layers (meta, meta-poky)
  - Default starting point
  - Fully functional standalone distribution

#### Q3: What is metadata in Yocto?
**Answer:**
Metadata is information describing how to build software:
- **Recipes (.bb)**: Build instructions for packages
- **Classes (.bbclass)**: Reusable build logic
- **Configuration files (.conf)**: Machine/distribution settings
- **Patches**: Code modifications
- **Append files (.bbappend)**: Extend existing recipes

#### Q4: What is a recipe in Yocto?
**Answer:**
A recipe is a .bb file containing build instructions for a single package:
- Source location (SRC_URI)
- Dependencies (DEPENDS, RDEPENDS)
- Build tasks (do_configure, do_compile)
- Installation instructions (do_install)
- Package metadata (LICENSE, AUTHOR)

#### Q5: Explain layers in Yocto.
**Answer:**
Layers organize metadata logically:
- **Organization**: Separate concerns (BSP, distro, features)
- **Reusability**: Share between projects
- **Priority**: Control override precedence
- **Common layers**:
  - meta: Core metadata
  - meta-bsp: Hardware-specific
  - meta-custom: User-defined features

### Intermediate Level

#### Q6: What is BitBake and how does it work?
**Answer:**
BitBake is Yocto's task scheduler and execution engine:
- **Parse phase**: Reads recipes and configuration
- **Analysis phase**: Resolves dependencies and build order
- **Execution phase**: Runs tasks in dependency order
- **Caching**: Stores build artifacts for reuse

#### Q7: Explain the difference between DEPENDS and RDEPENDS.
**Answer:**
- **DEPENDS**: Build-time dependencies
  - Required during compilation
  - Example: gcc, cmake, libxml2-dev
  - Note: `DEPENDS = "libxml2:do_populate_sysroot"`
  
- **RDEPENDS**: Runtime dependencies
  - Required in final image
  - Example: libxml2 (not dev files)
  - Propagates to package_ipk/deb/rpm

#### Q8: What is the purpose of do_populate_sysroot task?
**Answer:**
Prepares the staging/sysroot directory for cross-compilation:
- Copies headers and libraries to sysroot
- Makes them available for dependent packages
- Typically runs after do_install
- Dependency: Other recipes depend on `recipe:do_populate_sysroot`

#### Q9: Explain BitBake variables with package-specific overrides.
**Answer:**
Package-specific variables modify behavior for specific recipes:

```makefile
# Pattern: VARIABLE_append_pn-PackageName = "value"
CFLAGS_append_pn-gcc = "-O3"
EXTRA_OEMAKE_append_pn-kernel = "LOCALVERSION=-custom"

# Machine-specific
KERNEL_DEVICETREE_append_machine-rpi4 = "bcm2711-rpi4.dtb"

# Distribution-specific
DISTRO_FEATURES_append = "systemd"
```

#### Q10: What is a Board Support Package (BSP)?
**Answer:**
A BSP layer contains hardware-specific metadata:
- **Machine definitions**: Target board settings
- **Device tree**: Hardware description
- **Kernel recipes**: Board-specific kernel config
- **Bootloader**: U-boot or other bootloader
- **Drivers**: Hardware-specific drivers

Example structure:
```
meta-raspberrypi/
├── conf/machine/raspberrypi4.conf
├── recipes-kernel/linux/
├── recipes-bsp/u-boot/
└── recipes-bsp/device-tree/
```

### Advanced Level

#### Q11: How would you create a custom Linux distribution?
**Answer:**
Steps to create custom distro:

```bash
# 1. Create distro layer
mkdir -p meta-custom/conf/distro

# 2. Create distro configuration
# meta-custom/conf/distro/custom.conf
DISTRO = "custom"
DISTRO_NAME = "Custom Linux"
DISTRO_VERSION = "1.0"
DISTRO_FEATURES = "systemd pam x11"
PREFERRED_PROVIDER_virtual/kernel = "linux-custom"
PREFERRED_VERSION_linux-custom = "5.15%"

# 3. Create custom image
# meta-custom/recipes-core/images/core-image-custom.bb
require recipes-core/images/core-image-base.bb
IMAGE_FEATURES += "dev-pkgs ssh-server-openssh"
IMAGE_INSTALL += "vim curl wget"

# 4. Add to bblayers.conf
echo 'BBLAYERS += "${TOPDIR}/../meta-custom"' >> conf/bblayers.conf

# 5. Build
bitbake -c build core-image-custom
```

#### Q12: Explain how to optimize image size in Yocto.
**Answer:**
Multiple optimization techniques:

```makefile
# 1. Minimal image recipe
require recipes-core/images/core-image-minimal.bb

# 2. Disable unnecessary features
DISTRO_FEATURES_remove = "x11 opengl wayland"

# 3. Use musl instead of glibc (smaller)
TCLIBC = "musl"

# 4. Strip binaries
IMAGE_FEATURES_append = "read-only-rootfs"

# 5. Remove debug symbols
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
INHIBIT_PACKAGE_STRIPFILES = "1"

# 6. Remove documentation
DEPLOY_DIR_IMAGE_remove = "usr/share/doc usr/share/man"

# 7. Limit rootfs size
IMAGE_FSTYPES = "cpio.gz"

# 8. Remove unused packages
IMAGE_INSTALL_remove = "unnecessary-package"

# 9. Use module-based kernel (only needed features)
KERNEL_FEATURES_append = "-cfg=modules.scc"
```

#### Q13: How do you handle version conflicts in Yocto?
**Answer:**
Multiple resolution strategies:

```makefile
# 1. Preferred provider (priority)
PREFERRED_PROVIDER_virtual/kernel = "linux-yocto"

# 2. Preferred version
PREFERRED_VERSION_gcc = "11.%"
PREFERRED_VERSION_gcc-runtime = "11.%"

# 3. Append file to override
# Create: meta-custom/recipes-libs/openssl/openssl_%.bbappend
SRCREV = "new-commit-hash"
PREFERRED_VERSION = "1.1.1"

# 4. Layer priority in bblayers.conf
# Higher priority overrides lower priority
BBFILE_PRIORITY_meta = "5"
BBFILE_PRIORITY_meta-custom = "10"  # Higher = takes precedence

# 5. Runtime dependency resolution
RDEPENDS_append_myapp = " specific-library-version"
```

#### Q14: Explain patch management in Yocto recipes.
**Answer:**
Patches are applied during do_patch task:

```makefile
# Patch specification in recipe
SRC_URI = "https://example.com/source.tar.gz \
           file://fix-build-error.patch \
           file://add-feature.patch"

# Patches in recipe directory structure
meta-custom/recipes-apps/myapp/
├── myapp_1.0.bb
├── files/
│   ├── fix-build-error.patch
│   └── add-feature.patch
└── myapp-1.0/
    └── <source files>

# Patch file format (git diff or unified diff)
--- a/src/main.c
+++ b/src/main.c
@@ -10,5 +10,5 @@
     return 0;
-    printf("old");
+    printf("new");
 }

# Apply patches with striplevel
SRC_URI += "file://fix.patch;striplevel=1"

# Apply only for specific condition
SRC_URI += "file://conditional.patch;apply=${@'1' if d.getVar('DEBUG') else '0'}"
```

#### Q15: How do you debug a failed Yocto build?
**Answer:**
Debugging strategies:

```bash
# 1. Check build logs
cat tmp/work/qemux86-64/recipe-name/1.0-r0/temp/log.do_compile
cat tmp/work/qemux86-64/recipe-name/1.0-r0/temp/log.do_install

# 2. Enable verbose output
bitbake -v recipe-name
bitbake -vv recipe-name  # Even more verbose

# 3. Build single task
bitbake -c compile -f -v recipe-name
bitbake -c install -f -v recipe-name

# 4. Keep work directory after failure
INSANE_SKIP:${PN} = "file-rdeps"

# 5. Debug do_fetch issues
bitbake -c fetch -v recipe-name

# 6. Check downloaded sources
ls tmp/work/qemux86-64/recipe-name/*/git/

# 7. Use devshell for interactive debugging
bitbake -c devshell recipe-name
# Opens shell in build directory

# 8. Check dependencies
bitbake -g core-image-minimal
# Generates task-depends.dot and pn-depends.dot

# 9. Examine parsed metadata
bitbake -e recipe-name | grep "^VAR_NAME="

# 10. Check for QA warnings
WARN_QA = "ldflags"
ERROR_QA = "ldflags"
```

#### Q16: Explain cross-compilation in Yocto.
**Answer:**
Cross-compilation compiles for a different target architecture:

```makefile
# Machine configuration specifies target
MACHINE = "qemux86-64"  # x86-64 target
# or
MACHINE = "qemuarm64"   # ARM64 target

# Sysroot setup for cross-compilation
SYSROOT_DIR = "${TMPDIR}/sysroots/${MACHINE}"

# Cross-compile compiler settings
CC = "${CROSS_COMPILE}gcc"
CXX = "${CROSS_COMPILE}g++"
LD = "${CROSS_COMPILE}ld"

# Where CROSS_COMPILE is set based on MACHINE
CROSS_COMPILE = "aarch64-oe-linux-"

# Recipe handles cross-compilation automatically
# No need for special handling in most cases

# For recipes that don't support cross-compilation
do_configure_append() {
    # Set proper paths
    export CC="${BUILD_CC}"
    export CFLAGS="${BUILD_CFLAGS}"
    ./configure --target=${TARGET_SYS}
}
```

---

## Best Practices

### 1. **Layer Organization**
```
meta-custom/
├── README.md              # Layer documentation
├── COPYING                # License file
├── conf/
│   ├── layer.conf        # Layer metadata
│   ├── machine/          # Machine definitions
│   └── distro/           # Distro definitions
├── recipes-core/
├── recipes-apps/
├── recipes-kernel/
├── recipes-bsp/
├── recipes-extended/
└── classes/              # Custom classes
```

### 2. **Recipe Best Practices**
```makefile
# Always specify license
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${S}/LICENSE;md5=..."

# Use PV, PR consistently
PV = "1.0.0"
PR = "r1"

# Proper source handling
SRC_URI = "https://example.com/${PF}.tar.gz"
SRC_URI[sha256sum] = "abc123def456..."

# Clear dependencies
DEPENDS = "libxml2 openssl"
RDEPENDS_${PN} = "libc libxml2"

# Proper installation
do_install() {
    install -d ${D}${bindir} ${D}${sysconfdir}
    install -m 0755 ${B}/app ${D}${bindir}/
    install -m 0644 config.conf ${D}${sysconfdir}/
}

# Package properly
FILES_${PN} = "${bindir}/* ${sysconfdir}/*"
FILES_${PN}-dev = "${includedir}/* ${libdir}/*.a"
CONFFILES_${PN} = "${sysconfdir}/app.conf"
```

### 3. **Variable Naming**
- Uppercase for variables: `MY_VAR`
- Underscores for word separation: `MY_BUILD_VAR`
- Use standard Yocto variables where possible
- Document custom variables

### 4. **Performance Optimization**
```bash
# Use parallel builds
PARALLEL_MAKE = "-j $(nproc)"
BB_NUMBER_THREADS = "$(nproc)"

# Enable caching
BB_HASHSERVE = "localhost:8008"
BB_SIGNATURE_HANDLER = "OEEquivHash"

# Pre-fetch sources
bitbake core-image-minimal:do_fetch

# Use sstate cache
SSTATE_DIR = "/shared/sstate-cache"
```

### 5. **Version Control**
```bash
# Track metadata changes
git init
git add .
git commit -m "Initial custom layer"

# Use git for BSP layers
# meta-custom/ should be separate git repo
# Allows tracking hardware-specific changes

# Tag releases
git tag -a v1.0 -m "Release 1.0"
```

### 6. **Documentation**
```markdown
# README.md for meta-custom/

## Layer Dependencies
- meta (Yocto core)
- meta-openembedded (optional)

## Supported Machines
- MACHINE = "qemux86-64"
- MACHINE = "qemuarm64"

## Building
```bash
MACHINE=qemux86-64 bitbake core-image-custom
```

## Contributing
[contribution guidelines]
```

### 7. **Configuration Management**
```bash
# Use local.conf.sample for defaults
cp conf/local.conf.sample conf/local.conf

# Document build environment
cat > BUILD_ENV.txt << EOF
Yocto: kirkstone
Machine: qemux86-64
Distro: poky
Date: $(date)
EOF

# Version control non-sensitive configs
git add conf/bblayers.conf
git add conf/*.conf.sample
git add .gitignore
```

---

## Common Commands Reference

```bash
# Environment setup
source oe-init-build-env build/

# Build operations
bitbake core-image-minimal              # Build image
bitbake -c compile myapp                # Compile only
bitbake -c clean myapp                  # Clean artifacts
bitbake -c cleanall myapp               # Remove all artifacts

# Information
bitbake -e myapp | grep ^VAR_NAME=     # Check variable value
bitbake -g core-image-minimal           # Generate dependency graph
bitbake-layers show-recipes             # List recipes
bitbake-layers show-layers              # List layers
bitbake -c listtasks myapp              # List tasks

# Debugging
bitbake -v myapp                        # Verbose output
bitbake -c devshell myapp               # Interactive shell
bitbake -f -c compile myapp             # Force recompile

# Performance
time bitbake core-image-minimal         # Measure build time
```

---

## Summary Table

| Concept | Purpose | Example |
|---------|---------|---------|
| **Yocto** | Build system framework | Meta-project |
| **Poky** | Reference distribution | Default implementation |
| **BitBake** | Task scheduler | Executes builds |
| **Recipe** | Build instructions | myapp_1.0.bb |
| **Layer** | Metadata organization | meta-custom |
| **BSP** | Hardware support | meta-raspberrypi |
| **Class** | Reusable logic | autotools.bbclass |
| **Image** | Final filesystem | core-image-minimal |
| **DEPENDS** | Build dependencies | libxml2 |
| **RDEPENDS** | Runtime dependencies | libc libxml2 |

---

## Key Takeaways

1. **Modularity**: Layers enable reusable, modular designs
2. **Flexibility**: Customize every aspect of the build
3. **Efficiency**: Dependency tracking prevents unnecessary rebuilds
4. **Reproducibility**: Same configuration produces identical images
5. **Cross-compilation**: Native build system for target architectures
6. **Documentation**: Maintain clear layer and recipe documentation

---

## References

- Official Yocto Project: https://www.yoctoproject.org/
- BitBake Manual: https://docs.yoctoproject.org/bitbake/
- Yocto Mega-Manual: https://docs.yoctoproject.org/
- Silicon Labs Embedded Systems: Yocto training
- Community forums and documentation
