# Interactive 2D Dynamic Rain Simulation (C++ / WinAPI / GDI)

A lightweight, low-level 2D interactive graphics application built natively on the Windows API using Raw Win32 and GDI (Graphics Device Interface). 

The project demonstrates low-level systems programming, manual Windows memory architecture management, custom physics loops, and double-buffered hardware rendering optimized to maintain a constant 60 FPS.

## 🚀 Key Technical Highlights
* **Pure Win32 API Foundation:** Zero external engine dependencies (like Unreal or Unity) or heavyweight frameworks. Built using raw `WNDCLASS` registration, message pump routing, and explicit `WndProc` callback management.
* **Double-Buffered Smooth Rendering:** Implements advanced memory-device contexts (`CreateCompatibleDC`, `CreateCompatibleBitmap`) to prevent screen flickering and artifact tearing, ensuring 60 FPS real-time interpolation.
* **Custom Procedural Physics Engine:** Handles custom math loops for vector coordinates calculation, linear transformation velocities, boundary colliders checking, and pseudo-random generational drops positioning.
* **Low-Level Native Diagnostics:** Developed with aggressive hardware profiling and memory-leak checks, debugged using static/dynamic execution flow analyzers like OllyDbg.

## 🛠️ Tech Stack & Tooling
* **Language:** C++ (C++17 standard features)
* **API Architecture:** Win32 API, GDI (Graphics Device Interface)
* **IDE:** Microsoft Visual Studio
* **Reverse Engineering / Debugging:** OllyDbg

## 📊 Application Preview

![Project Preview](preview.gif)

*Custom interactive graphical environment demonstrating real-time weather rendering cycles.*

## 💾 Project Architecture
* `main.cpp` — Application core bootstrap containing `WinMain`, the continuous realtime rendering loop, and low-level Window Proc input interceptors.
* `Simulation.cpp` / `.h` — Object-Oriented state engine processing coordinate vectors transformations, drops generation, and performance metrics synchronization.
* `Renderer.cpp` — Dedicated GDI graphics pipelines mapping system memory device canvases directly to active Win32 screen frames.

## ⚡ Core Engine Snippets

### Real-time Double-Buffered Canvas Pipeline
This explicit GDI buffer management completely eliminates visual screen tearing by painting vectors inside an unmapped memory region before flipping the complete context directly onto the hardware screen canvas:

```cpp
void RenderFrame(HWND hwnd, HDC hdcDevice) {
    HDC hdcBuffer = CreateCompatibleDC(hdcDevice);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcDevice, screenWidth, screenHeight);
    SelectObject(hdcBuffer, hBitmap);

    // [System Memory Layer] Execute raw procedural vectors rendering
    DrawRainDrops(hdcBuffer);

    // [Hardware Blitting Layer] Instant synchronous frame swap
    BitBlt(hdcDevice, 0, 0, screenWidth, screenHeight, hdcBuffer, 0, 0, SRCCOPY);

    DeleteObject(hBitmap);
    DeleteDC(hdcBuffer);
}
