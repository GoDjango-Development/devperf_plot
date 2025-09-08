# CSV Plotter with SDL2 and Cairo

This program reads a CSV file with four lines of numerical data and plots them
as colored lines. It displays the result in an SDL2 window with horizontal
scrolling and saves the output as a PNG file (`plot.png`).

---

## Features

- Plots **DevTime**, **ReviewTime**, and **LeadTime** from a CSV file.
- Compute at-fly and plots **MeanTAT**, a.k.a Mean Turnarround Time.
- Y-axis ranges from 0 to 30, shown on the left.
- X-axis shows months starting from **September 2025**.
- Horizontal scroll bar is visible if the data exceeds the window width.
- Supports scrolling with **arrow keys** or **mouse dragging**.
- Output PNG file has a width proportional to the number of months (minimum 1600
px).

---

## Requirements

- GCC or compatible C compiler
- SDL2 and SDL2_gfx libraries
- Cairo graphics library
- pkg-config

### Install on Linux (Debian/Ubuntu)

```bash
sudo apt-get install build-essential libsdl2-dev libsdl2-gfx-dev libcairo2-dev
pkg-config
````

---

## Build

Use the provided Makefile or compile manually:

```bash
make release
```

Or manually:

```bash
gcc plot.c -o plot $(pkg-config --cflags --libs sdl2) -lSDL2_gfx $(pkg-config
--cflags --libs cairo)
```

---

## Usage

```bash
./plot data.csv
```

Where `data.csv` has **exactly four lines**, each representing a different
metric:

1. **Plot EPOCH** first line
2. **DevTime** – second line
3. **ReviewTime** – third line
4. **LeadTime** – fourth line

### CSV format

* EPOCH must separated by /
* Numbers must be separated by commas.
* **Important:** Each line, except first one, must end with a comma.
* Example:

```
9/2025
5,8,6,7,9,10,8,7,6,5,7,8,
4,6,5,6,7,8,7,6,5,4,6,7,
12,14,13,15,16,18,17,16,14,13,15,16,
```

---

## Notes

* X-axis increments by months starting from **September 2025**.
* Y-axis shows values from 0 to 30.
* Horizontal scroll allows viewing all months if there are more than fit the
window.
* The program saves a PNG (`plot.png`) with the same width as the total plotted
months.

---

## Example

Suppose your `data.csv` contains:

```
9/2025
10,12,8,15,
5,6,4,7,
20,18,15,25,
```

Running:

```bash
./plot data.csv
```

* Shows a window with the plotted lines for 4 months.
* PNG file `plot.png` is generated with the same plot.
* Scroll horizontally if necessary to see all months.
* There is a CSV data example at config/ folder.
---

## License

MIT License
This project is free to use and modify.

