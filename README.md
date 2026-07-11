# soVIMter

A solitaire game built with SDL3.

Dependencies (SDL3, SDL3_ttf, SDL3_image, SDL3_mixer) are managed with
[vcpkg](https://github.com/microsoft/vcpkg) in **manifest mode** (see `vcpkg.json`).
No git submodules are needed — the first build fetches and compiles SDL3 through vcpkg
and caches it, so subsequent builds are fast.

---

## 1. Install vcpkg

### Linux / macOS

```sh
git clone https://github.com/microsoft/vcpkg ~/vcpkg
~/vcpkg/bootstrap-vcpkg.sh
```

Make vcpkg discoverable (add to `~/.bashrc` / `~/.zshrc` to persist):

```sh
export VCPKG_ROOT="$HOME/vcpkg"
export PATH="$VCPKG_ROOT:$PATH"
```

### Windows

```bat
git clone https://github.com/microsoft/vcpkg C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat
```

Add vcpkg to your environment (PowerShell, persistent — run once):

```powershell
setx VCPKG_ROOT "C:\vcpkg"
setx PATH "%PATH%;C:\vcpkg"
```

Then **open a new terminal** so the variables take effect.

---

## 2. System build prerequisites

vcpkg builds SDL3 and its dependencies **from source**, so a C/C++ toolchain and a
few build tools must be present.

### Linux (Debian / Ubuntu)

```sh
sudo apt install -y build-essential cmake ninja-build pkg-config \
    autoconf autoconf-archive automake libtool python3-venv
```

> Without `autoconf-archive` and `python3-venv` some vcpkg ports
> (e.g. `libxcrypt`, `libsystemd`) fail to build.

### Windows

- **Visual Studio 2022** with the *"Desktop development with C++"* workload
  (gives you MSVC, the Windows SDK and CMake), **or** Build Tools for Visual Studio.
- CMake (bundled with VS, or install separately and add to PATH).

---

## 3. Build & run

### Linux

The `Makefile` wires the vcpkg toolchain automatically (it reads `VCPKG_ROOT`,
defaulting to `~/vcpkg`):

```sh
make build-debbug     # configure (Debug)   — or: make build-release
make compile          # build
make run              # run

# or all at once:
make fast-run-debug
```

The executable and its assets are placed in `output/`.

### Windows

Configure and build with CMake, passing the vcpkg toolchain file:

```bat
cmake -B build -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake
cmake --build build --config Release
```

The `.exe` and assets land in `output\`. On Windows the required SDL3 DLLs are copied
next to the executable automatically by the vcpkg toolchain.

You can also just open the folder in Visual Studio 2022 (*File → Open → Folder*) —
it detects `CMakeLists.txt` and `vcpkg.json` and configures everything for you.

---

## Notes

- Dependency versions and enabled features are pinned in `vcpkg.json`.
  Note that `sdl3-image` explicitly enables the `png` feature (all assets are PNG)
  and `sdl3`'s `ibus` feature is disabled on Linux to avoid a heavy `dbus`/`libsystemd`
  build.
- The first configure is slow (SDL3 is compiled from source); later builds reuse the
  vcpkg binary cache.
