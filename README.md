# Rubik's Cube Solver

A C++ application for solving a Rubik's Cube using image processing and external controller with servo motors.

## Features

- Cube state reconstruction from image data.
- Rubik's Cube solving algorithm.
- Servo movement planning.
- TCP communication with an external controller.
- Receiving TCP packets and controlling servo motors on the external controller.
- Image processing using OpenCV.

## Build

Clone the repository:

```bash
git clone https://github.com/Kuba-Wi/rubiks-cube.git
cd rubiks-cube
```

Configure and build the project:

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

Run the application using a camera to capture images and the default controller IP address:

```bash
./rubiks-cube
```

Alternatively, provide a directory containing images of the cube faces and the controller's IP address:

```bash
./rubiks-cube ../images 192.168.1.100
```

## Directories

The `remoteSide` directory contains the code that runs on the external controller.

## Important Notes

Images of the cube faces must be captured in the following orientations:

- **Blue face:** yellow face on top.
- **Red face:** yellow face on top.
- **Green face:** yellow face on top.
- **Orange face:** yellow face on top.
- **White face:** blue face on top.
- **Yellow face:** green face on top.

When solving the cube manually using the moves printed by the program, or when using the robot, keep the **blue face facing forward** and the **yellow face facing upward**. The algorithm plans moves assuming this orientation.

## Example Output

The following is an example of the program's output when run with previously captured images and a controller at `127.0.0.1`:

```text
Kuba-Wi:~/rubiks-cube/build$ ./rubiks-cube ../images 127.0.0.1
[INFO] Loading colors for image ../images/orange.jpeg.
[INFO] Orange | Orange | Blue
[INFO] White | Orange | Orange
[INFO] White | Yellow | Red
[INFO] Loading colors for image ../images/blue.jpeg.
[INFO] Red | Red | Red
[INFO] Blue | Blue | Green
[INFO] Yellow | Green | White
[INFO] Loading colors for image ../images/green.jpeg.
[INFO] White | Red | Green
[INFO] Blue | Green | Red
[INFO] Yellow | White | Orange
[INFO] Loading colors for image ../images/white.jpeg.
[INFO] Green | Yellow | Blue
[INFO] Red | White | Orange
[INFO] Green | Green | Blue
[INFO] Loading colors for image ../images/yellow.jpeg.
[INFO] Yellow | Green | Red
[INFO] White | Yellow | Blue
[INFO] Yellow | Blue | Green
[INFO] Loading colors for image ../images/red.jpeg.
[INFO] White | Yellow | Blue
[INFO] Orange | Red | White
[INFO] Orange | Yellow | Orange
[INFO] Initial cube state:
[INFO] Corner Permutation:
[INFO] 1 6 5 3 2 4 0 7
[INFO] Corner Orientation:
[INFO] 0 1 1 0 2 1 2 2
[INFO] Edge Permutation:
[INFO] 5 0 6 11 4 7 8 9 2 3 10 1
[INFO] Edge Orientation:
[INFO] 0 1 0 0 0 0 1 1 0 1 1 1

[INFO] Solving the cube...
[INFO] Move to G1: L
[INFO] Move to G1: U
[INFO] Move to G1: F
[INFO] Move to G1: L2
[INFO] Move to G1: U2
[INFO] Move to G1: B'
[INFO] Move to G1: R
[INFO] Move to G1: F'
[INFO] Move to G1: L
[INFO] Move: D'
[INFO] Move: B2
[INFO] Move: R2
[INFO] Move: B2
[INFO] Move: U'
[INFO] Move: F2
[INFO] Move: R2
[INFO] Move: D'
[INFO] Move: F2
[INFO] Move: U
[INFO] Move: R2
[INFO] Move: D'
[INFO] Move: R2
[INFO] Connecting to the server at 127.0.0.1:5000.
[INFO] Connected.
[INFO] Put the cube in the initial position and press Enter to start solving...

[INFO] Press Enter to pause/resume solving...
[INFO] Executing move: L
[INFO] Executing move: U
[INFO] Executing move: F
[INFO] Executing move: L2
[INFO] Executing move: U2
[INFO] Executing move: B'
[INFO] Executing move: R
[INFO] Executing move: F'
[INFO] Executing move: L
[INFO] Executing move: D'
[INFO] Executing move: B2
[INFO] Executing move: R2
[INFO] Executing move: B2
[INFO] Executing move: U'
[INFO] Executing move: F2
[INFO] Executing move: R2
[INFO] Executing move: D'
[INFO] Executing move: F2
[INFO] Executing move: U
[INFO] Executing move: R2
[INFO] Executing move: D'
[INFO] Executing move: R2
[INFO] Cube state after solving:
[INFO] Corner Permutation:
[INFO] 0 1 2 3 4 5 6 7
[INFO] Corner Orientation:
[INFO] 0 0 0 0 0 0 0 0
[INFO] Edge Permutation:
[INFO] 0 1 2 3 4 5 6 7 8 9 10 11
[INFO] Edge Orientation:
[INFO] 0 0 0 0 0 0 0 0 0 0 0 0
```

## Author

[Kuba-Wi](https://github.com/Kuba-Wi)
