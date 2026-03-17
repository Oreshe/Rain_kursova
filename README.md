# Rain Project 

Interactive desktop visualization developed as a course project for the "System Programming" discipline at KNEU.

## Project Goal
The main objective is to create an interactive "rain" effect that reacts to user input directly on the Windows desktop.

## Key Features
  **Interactive Ripples**: Generates expanding waves at the cursor's location upon a left mouse click.
  **Performance**: Optimized for smooth animation at 60 FPS using system timers.
  **UI/UX**: Runs in a borderless, full-screen transparent mode, creating an immersive experience over the desktop wallpaper.
  **Cleanup Logic**: Uses `std::vector` and `std::remove_if` to efficiently manage the lifecycle of active drops.

## Tech Stack
  **Language**: C++ 
  **API**: Windows GDI / WinAPI 
  **Environment**: Visual Studio 2022
  **Debug**: Verified with OLLYDBG for memory and message handling

## Usage
  **Left Click**: Create a new ripple.
  **ESC**: Exit the application.
