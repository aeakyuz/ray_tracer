# Simple Ray Tracer

## How to build and run
If you don't have 'make', install it. Then in the parent directory, use the `make` command to build.
After you build, use `./build/raytracer <nameOfXml>` to run.
If you would like to use different textures for a scene, put your texture in the `textures/` folder and change the content of the `<textureimage>` tag of your scene file to `textures/<nameOfTextureFile>`.
The output will appear in the parent directory with the name `output.ppm`.

Example:

```
./build/raytracer scenes/my_scene.xml
```
