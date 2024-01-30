# Our binary file formats

## Binary Mesh (.msh)

### Vertex

Vec3 of float(3 * 4 bytes): Position

Vec2 of float(2 * 4 bytes): TexCoord

### Actual file data layout

0 -> 4 bytes (uint32_t): Version number of this file.

4 -> 4 bytes (uint32_t): Number of Verticies = Nv.

8 -> Nv * 20 bytes (floats): Vertex array data.

Nv * 20 -> 4 bytes (uint32_t): Number of Indicies = Ni.

(Nv * 20) + 4 -> Ni * 2 bytes (uint16_t): Index array data.

Example:

1 1 0.3 -0.3 -0.6 0.0 1.0 3 1 2 0

1 2 0.3 -0.3 -0.6 0.0 1.0 0.3 -0.3 -0.6 0.0 1.0 6 1 2 0 1 2 0

// TODO: Skinned Mesh