# About User Data

(This is not a tutorial this is a explanation)

User Data is a small feature that allows for a structure / class to be shared in different places in the user code. It uses "void\*" type and casting.

## Use

Lets say that there is a structure called "MyGameData" that has a player position and other state of the application.

```cpp

struct MyGameData {
    float positionX; // 4 bytes
    float positionY; // 4 bytes
    float positionZ; // 4 bytes
};

```

Now this is probably pretty important for the game so the Core::Engine class can take care of it.

```cpp

MyGameData data;
data.positionX = 0;
data.positionY = 0;
data.positionZ = 0;

Core::Engine::GenerateUserData(&data, sizeof(data)); // Copies the data so

// IMPORTANT: The "data" structure will be destroyed when out of scope, so the engine copies the data.
```

To be able to access the data there are multiple ways. The best is "(MyGameData \*)Engine::GetUserData()"

```cpp

MyGameData* data = (MyGameData*)Engine::GetUserData();

// Do things.
data.positionX += 10;

```

Also copying the data is allowed but it creates and deletes data dynamically and can be slow.
Copying is best used when it isn't on frame by frame and is used rarely.

```cpp

MyGameData* data = (MyGameData*)Engine::GetCopyOfUserData();

// Do things.
data.positionX += 10;

// FREE DATA SO NO MEMORY LEAK
Core::CeMemory::Free(data);

```

## Important

If the data needs to be reset to another thing use "Engine::RegenerateUserData", this will reset the user data to the thing thats passed in.

BTW: If the user data is read and written to, Engine::RegenerateUserData is not necessary, in fact is actually bad. As long as the structure stays the same (doesn't change from "MyGameData" to something else), then Engine::RegenerateUserData is pointless and slow/
