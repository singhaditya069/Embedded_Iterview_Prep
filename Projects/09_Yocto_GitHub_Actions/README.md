# Yocto Build System & GitHub Actions - Complete Implementation Guide

## Project Overview

Yocto Project is a Linux build system for embedded devices. GitHub Actions provides CI/CD automation. This project covers integrating both for continuous integration and deployment.

---

## Yocto Build System Basics

### Yocto Architecture

```
┌──────────────────────────────────────────┐
│     Recipes & Configurations            │
│     (Custom layers, patches)             │
└────────────────┬─────────────────────────┘
                 │
┌────────────────▼─────────────────────────┐
│  Yocto Build Engine (BitBake)            │
│  • Dependency resolution                 │
│  • Source retrieval                      │
│  • Compilation                           │
│  • Packaging                             │
└────────────────┬─────────────────────────┘
                 │
        ┌────────┴────────┐
        │                 │
    ┌───▼────┐       ┌───▼────┐
    │ Kernel │       │Root FS  │
    └────────┘       └────────┘
        │                 │
        └────────┬────────┘
                 ▼
        ┌──────────────────┐
        │ Device Image    │
        │ (*.img)         │
        └──────────────────┘
```

### Layer Structure

```
yocto-project/
├── meta/                    # Core Yocto layer
├── meta-bsp/               # Board support layer
├── meta-custom/            # Custom applications
│   ├── conf/
│   │   └── layer.conf
│   └── recipes-apps/
│       └── myapp/
│           └── myapp_1.0.bb
└── build/
    └── conf/
        ├── local.conf
        └── bblayers.conf
```

### Recipe Example (.bb file)

```bitbake
# recipes-apps/myapp/myapp_1.0.bb

DESCRIPTION = "Custom embedded application"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=abc123"

SRC_URI = "git://github.com/user/myapp.git;branch=master"
SRCREV = "abc123def456"

S = "${WORKDIR}/git"

# Build dependencies
DEPENDS = "virtual/kernel libc"

# Runtime dependencies
RDEPENDS = "libgpiod"

do_compile() {
    # Custom compilation
    make -C ${S} CFLAGS="-O2 -Wall"
}

do_install() {
    # Install to staging area
    install -d ${D}${bindir}
    install -m 0755 ${S}/myapp ${D}${bindir}/
    
    # Install configuration
    install -d ${D}${sysconfdir}
    install -m 0644 ${S}/myapp.conf ${D}${sysconfdir}/
}

FILES = "${bindir}/myapp ${sysconfdir}/myapp.conf"

COMPATIBLE_MACHINE = "beaglebone|rpi"
```

### Yocto Build Process

```bash
# Initialize build environment
source oe-init-build-env build

# Configure for target machine
echo "MACHINE = 'am335x-evm'" >> conf/local.conf

# Build target image
bitbake core-image-minimal

# Results
# build/tmp/deploy/images/am335x-evm/
#  ├── uImage
#  ├── core-image-minimal-am335x-evm.tar.gz
#  └── core-image-minimal-am335x-evm.ext4
```

---

## GitHub Actions CI/CD Pipeline

### GitHub Actions Workflow

```yaml
# .github/workflows/build.yml
name: Yocto Build & Test

on:
  push:
    branches: [ main, develop ]
  pull_request:
    branches: [ main ]

jobs:
  build:
    runs-on: ubuntu-latest
    
    steps:
    - name: Checkout repository
      uses: actions/checkout@v3
      with:
        submodules: recursive
    
    - name: Setup build environment
      run: |
        sudo apt-get update
        sudo apt-get install -y \
          gawk help2man g++ \
          diffstat chrpath \
          socat libsdl1.2-dev \
          xterm cpio python3 \
          python3-pexpect git
    
    - name: Initialize Yocto
      run: |
        source oe-init-build-env build
        
    - name: Configure Yocto
      run: |
        cd build
        echo "MACHINE = 'qemux86-64'" >> conf/local.conf
        echo "DISTRO = 'poky'" >> conf/local.conf
        echo "BB_NUMBER_THREADS = '4'" >> conf/local.conf
        echo "PARALLEL_MAKE = '-j 4'" >> conf/local.conf
    
    - name: Build image
      run: |
        cd build
        bitbake core-image-minimal 2>&1 | tee build.log
    
    - name: Generate SBOM
      run: |
        cd build
        bitbake -e core-image-minimal | grep "^IMAGE_ROOTFS" | cut -d= -f2 | \
        xargs -I {} find {} -type f -executable | sort > sbom.txt
    
    - name: Run static analysis
      run: |
        # Run SonarQube on source
        sonar-scanner \
          -Dsonar.projectKey=embedded_firmware \
          -Dsonar.sources=. \
          -Dsonar.host.url=${{ secrets.SONAR_HOST }} \
          -Dsonar.login=${{ secrets.SONAR_TOKEN }}
    
    - name: Upload artifacts
      if: success()
      uses: actions/upload-artifact@v3
      with:
        name: yocto-images
        path: |
          build/tmp/deploy/images/
          sbom.txt
    
    - name: Notify on failure
      if: failure()
      run: |
        echo "Build failed!"
        tail -100 build/build.log
```

### Advanced GitHub Actions

**Matrix Builds** (Test multiple configurations):
```yaml
strategy:
  matrix:
    machine: [qemux86, qemux86-64, qemuarm]
    distro: [poky, poky-tiny]

steps:
  - name: Build for ${{ matrix.machine }}/${{ matrix.distro }}
    run: |
      export MACHINE=${{ matrix.machine }}
      export DISTRO=${{ matrix.distro }}
      bitbake core-image-minimal
```

**Artifact Management**:
```yaml
- name: Download base artifacts
  uses: actions/download-artifact@v3
  with:
    name: yocto-images
    path: ./images

- name: Generate release
  if: startsWith(github.ref, 'refs/tags/')
  uses: actions/create-release@v1
  with:
    files: |
      images/uImage
      images/*.ext4
```

---

## Troubleshooting Common Issues

### Issue 1: Yocto Build Timeout

```bash
# Problem: Build takes too long
# Solution: Optimize BB_NUMBER_THREADS and PARALLEL_MAKE

echo "BB_NUMBER_THREADS = '$(nproc)'" >> conf/local.conf
echo "PARALLEL_MAKE = '-j $(nproc)'" >> conf/local.conf

# Use pre-built shared state cache
bitbake core-image-minimal --setscene-only
```

### Issue 2: Recipe Not Found

```bash
# Problem: bitbake can't find recipe
# Solution: Check layer configuration

cat conf/bblayers.conf  # Verify layer included
bitbake-layers show-recipes | grep myapp

# Add missing layer
bitbake-layers add-layer ../meta-custom
```

### Issue 3: Dependency Resolution Failures

```bash
# Problem: Conflicting package versions
# Solution: Use bitbake's dependency analysis

bitbake -g core-image-minimal
dot -Tpng pn-buildlist.dot -o dependencies.png

# Review and resolve version conflicts
grep DEPENDS recipes-apps/myapp/myapp_1.0.bb
```

---

## Interview Q&A

### Q1: Explain Yocto layer system and custom layer creation.

**Answer**:
Yocto uses layered approach for modularity:

```
Base layers: meta (Yocto core), meta-bsp (hardware)
Custom layers: meta-custom (application-specific)

Each layer contains:
- conf/layer.conf (layer metadata)
- recipes-*/package/ (package recipes)
- classes/ (custom build classes)
- conf/machine/ (board configurations)
```

**Creating Custom Layer**:
```bash
bitbake-layers create-layer meta-custom
cd meta-custom
mkdir -p recipes-apps/myapp
# Create recipe: myapp_1.0.bb
bitbake-layers add-layer ../meta-custom
```

**Best Practices**:
- Keep layers focused (don't mix unrelated recipes)
- Version control layer separately
- Document dependencies clearly
- Use layer compatibility matrix

### Q2: CI/CD pipeline design for firmware updates.

**Answer**:
```yaml
# Full pipeline: Build → Test → Artifact → Deploy

on: [push, pull_request]

jobs:
  build:
    # Compile firmware
  test:
    needs: build
    # Run unit tests
  analyze:
    needs: build
    # SonarQube, security scan
  flash:
    needs: [test, analyze]
    if: github.ref == 'refs/heads/main'
    # Flash to test devices
  deploy:
    needs: flash
    if: github.event_name == 'push'
    # Release artifacts
```

### Q3: Handle external dependencies in Yocto recipes.

**Answer**:
```bitbake
# External dependency: fetch from git

SRC_URI = "git://github.com/external/lib.git;protocol=https;branch=main"
SRCREV = "abc123def456789"

# With patches
SRC_URI += "file://0001-fix-build.patch"

# Add to dependencies
DEPENDS = "external-lib libconfig"

# Verify sources
do_verify_upstream_signatures() {
    gpg --verify ${S}/signature.asc ${S}/archive.tar.gz
}

addtask verify_upstream_signatures before do_compile
```

---

**End of Yocto & GitHub Actions Guide**
