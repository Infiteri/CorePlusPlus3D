# Haha some notes as I "work" on Core

### 19 October 2023.

So just a FAT memory leak happening with scene sky. So the cube map uses about 150 ish megabytes of memory, I know that is a lot but hey, it is what it is, anyways, when playing a scene the scene gets
copied for many reasons, but when it gets deleted the cube map never gets deleted so about 150 ish megabytes of memory where getting leaked when the scene got stopped so playing start and then stop 10 times results in about 100 - 200 megabytes of memory getting leaked each time, so about 1 - 1.5 GIGABYTES where getting leaked, also neither did the color nor the vertex array get destroyed so yea, memory leaks. Glad it was fast to fix so not too bad.

Oh yea I made a material editor for material asset files. So dragging a material file onto the scene viewport will open a material editor window which allows to edit material props without changing the file directly. Still WIP.

Moved the editor state into a structure called EditorState, therefore the cpp file is smaller.
