# 3D_Viewer

Implementation of software for viewing wireframe 3D objects.

<br><img src="../readme/amogus.jpg">

## Build

### Preparation

To start build you need to move to `./src` dir.

```Bash
cd ./src
```

### Build

- Build program

  ```
  make all
  ```

  or

  ```
  make install
  ```

- Build and run tests
  ```
  make tests
  ```
- Clean project
  ```
  make clean
  ```
- Unistall Project
  ```
  make uninstall
  ```
- Open HTML file
  ```
  make dvi
  ```
- Create archive with program
  ```
  make dist
  ```

## Editable settings

| Camera             | Object      | Object.Transform | Object.Mesh                     |
| ------------------ | ----------- | ---------------- | ------------------------------- |
| Position           | Line type   | Translation      | Standart buffer (turned off)    |
| Orientation        | Line color  | Rotation         | Wireframe buffer                |
| Focus point        | Line width  | Scale            | Triangle buffer                 |
| Camera mode        | Point type  |                  | Vertex only buffer (turned off) |
| View mode          | Point size  |
| View plane offsets | Point color |
| Parallel view box  |             |
| FOV                |             |

## Program lifecycle diagram

[Diagram view site (Figma)](https://www.figma.com/file/CAc22HrlBMQGsTtFFkmBTz/3DViewer_2_1_-diagram?type=whiteboard&node-id=0-1&t=m957eFr8aUnyU9aF-0)
